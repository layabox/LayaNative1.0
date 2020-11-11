#ifdef JS_V8

#include "V8HeapProfiler.h"
#include "debug-agent.h"
#include <util/JCJson.h>
#include "../JSInterface/V8/JSEnv.h"
#include <util/JCCommonMethod.h>
#include <util/JCIThreadCmdMgr.h>
#include "../../JCScrpitRuntime.h"

namespace laya {
	using v8::Isolate;
	using v8::ActivityControl;
	using v8::Array;
	using v8::Function;
	using v8::Handle;
	using v8::HeapSnapshot;
	using v8::Integer;
	using v8::Local;
	using v8::Object;
	using v8::SnapshotObjectId;
	using v8::String;
	using v8::TryCatch;
	using v8::Value;
	
	
	char V8HeapProfiler::sTmpBuff[];
	const char* TakeSnapshort = "takeHeapSnapshot";
	const char* startHeapTrack = "startTrackingHeapObjects";	//一旦发起这个请求，就要每隔一段时间发送 HeapProfiler.lastSeenObjectId 事件
	const char* stopHeapTrack = "stopTrackingHeapObjects";
	//"_lookupHeapObjectId";
	//"heapStatsUpdate";
	//"lastSeenObjectId";
	//"reportHeapSnapshotProgress";
	const char* addHeapSnapshotChunk = "addHeapSnapshotChunk";
	const char* getObjByHeapObjID = "getObjectByHeapObjectId";

	//进度提示。必须在js线程执行
	class ActivityControlAdapter : public ActivityControl {
	public:
		ActivityControlAdapter(V8HeapProfiler* pobj){
			pObj = pobj;
		}

		ControlOption ReportProgressValue(int done, int total) {
			return pObj->ReportProgressValue(done, total)==0? ControlOption::kContinue: ControlOption::kAbort;
		}
	private:
		V8HeapProfiler*		pObj;
	};

	void V8HeapProfiler::EndOfStream() {
		StrBuff buf(1024,1024);
		if (trackHeap) {

		}
		else {
			buf << "{\"seq\":" << DebuggerAgent::g_nSeqNum++ << ",\"type\":\"response\",\"request_seq\":" << curRequest_ << ",\"command\":\"HeapProfiler.takeHeapSnapshot\",\"success\":true,\"options_\":{\"maxStringLength\":-1}}";
		}

		if (agent_->session_)
			agent_->session_->DebuggerMessage(buf.getBuffer(), buf.getDataSize());
	}


	v8::OutputStream::WriteResult V8HeapProfiler::WriteAsciiChunk(char* data, int size) {
		static char buf[51200*2 + 512];
		//chunk后面就是一个字符串（可能是一部分json），客户端自己组装成完整的json对象
		int l = snprintf(buf, sizeof(buf), "{\"seq\":%d,\"type\":\"event\",\"success\":true,\
\"running\":true,\"event\":\"HeapProfiler.addHeapSnapshotChunk\",\"body\":{\"chunk\":\"", DebuggerAgent::g_nSeqNum++ );
		//然后开始复制chunk的内容，因为要转换所以自己写,剩下的是 %s\"}}
		char *st = buf + l;
		char *st1 = st;
		for (int i = 0; i < size; i++) {
			char cv = *data++;
			if (cv == '"') { *st++ = '\\'; *st++ = '"'; }
			else if (cv == 0x0a) { *st++ = '\\'; *st++ = 'n';}
			else if (cv == '\\') { *st++ = '\\'; *st++ = '\\'; }
			else *st++ = cv;
		}
		*st++ = '"'; *st++ = '}'; *st++ = '}';
		if (agent_->session_)
			agent_->session_->DebuggerMessage(buf, l+(st-st1));

		return kContinue;
	}

	//与 Date.now 等效的时间
	int64_t jsnow() {
		return tmGetCurms();
		/*
		// According to ECMA-262, section 15.9.1, page 117, the precision of
		// the number in a Date object representing a particular instant in
		// time is milliseconds. Therefore, we floor the result of getting
		// the OS time.
		double millis;
		if (FLAG_verify_predictable) {
			millis = 1388534400000.0;  // Jan 1 2014 00:00:00 GMT+0000
			millis += Floor(isolate->heap()->synthetic_time());
		}
		else {
			millis = Floor(base::OS::TimeCurrentMillis());
		}
		*/
	}

	v8::OutputStream::WriteResult V8HeapProfiler::WriteHeapStatsChunk(v8::HeapStatsUpdate* data, int count) {
		v8::HeapProfiler* pProf = v8::Isolate::GetCurrent()->GetHeapProfiler();
		//发送 HeapProfiler.heapStatsUpdate 事件
		//{"seq":28,"type":"event","success":true,"running":true,"event":"HeapProfiler.heapStatsUpdate","body":{"statsUpdate":[6,45,11960,7,1,1352,8,159,14536]}}
		StrBuff buf(512,128);
		buf << "{\"seq\":" << DebuggerAgent::g_nSeqNum++ << ",\"type\":\"event\",\"success\":true,\"running\":true,\"event\":\"HeapProfiler.heapStatsUpdate\",\"body\":{\"statsUpdate\":[";
		for (int i = 0; i < count; i++) {
			if (i > 0)buf << ",";
			buf << data[i].index << "," << data[i].count << "," << data[i].size;
		}
		buf<< "]}}";
		agent_->session_->DebuggerMessage(buf.getBuffer(), buf.getDataSize());
		//发送 HeapProfiler.lastSeenObjectId 事件
		//{"seq":13,"type":"event","success":true,"running":true,"event":"HeapProfiler.lastSeenObjectId","body":{"lastSeenObjectId":null,"timestamp":1449392312736}}
		StrBuff buf1(512, 128);
		buf1 << "{\"seq\":" << DebuggerAgent::g_nSeqNum++ << ",\"type\":\"event\",\"success\":true,\"running\":true,\"event\":\"HeapProfiler.lastSeenObjectId\",\"body\":{\"lastSeenObjectId\":";
		if (lastSeenObj == 0) buf1 << "null";
		else buf1 << lastSeenObj;
		buf1 << ",\"timestamp\":" << jsnow() << "}}";
		agent_->session_->DebuggerMessage(buf1.getBuffer(), buf1.getDataSize());
		//下一次
		//lastSeenObj = pProf->GetHeapStats(this);
		return kContinue;
	}

	V8HeapProfiler::V8HeapProfiler(DebuggerAgent* agent, V8Socket* client) {
		agent_ = agent;
		client_ = client;
		curRequest_ = 0;
		lastSeenObj = 0;
		trackHeap = false;
	}

	void V8HeapProfiler::findObjectById(SnapshotObjectId id) {
		v8::Isolate* pIso = agent_->isolate();
		if (!pIso)
			return;
		v8::HeapProfiler* pHeapProf = pIso->GetHeapProfiler();
		Local<Value> obj = pHeapProf->FindObjectById(id);
		if (obj.IsEmpty())
			return;
		else if (obj->IsObject()
			|| obj->IsNumber()
			|| obj->IsString()
			|| obj->IsSymbol()
			|| obj->IsBoolean()) {
			/*
			int l = snprintf(sTmpBuff, sizeof(sTmpBuff), "{\"seq\":%d,\"type\":\"event\",\"success\":true,\"running\":true,\"event\":\"HeapProfiler.reportHeapSnapshotProgress\",\"body\":{\"done\":%d,\"total\":%d,\"finished\":%s}}",
				gnSeqNum++, done, total, done >= total ? "true" : "false");
			if (agent_->session_)
				agent_->session_->DebuggerMessage(sTmpBuff, l);
			*/
			int a = 0;
		}
	}

	void V8HeapProfiler::takeHeapSnapshot(bool needProg) {
		v8::Isolate* pIso = agent_->isolate();
		if (!pIso)
			return;
		trackHeap = false;
		class NR :public v8::HeapProfiler::ObjectNameResolver{
		public:
			virtual const char* GetName(Local<Object> object) {
				int a = 0;
				return "Global(window)";
			}
		};
		v8::HeapProfiler* pHeapProf = pIso->GetHeapProfiler();
		ActivityControlAdapter ctrl(this);
		NR nr;
		//执行的过程中会调用 ReportProgressValue， 执行完了后，才会退出
		v8::HeapSnapshot* pSnapshort = (v8::HeapSnapshot*)pHeapProf->TakeHeapSnapshot(needProg?(&ctrl):NULL,&nr);
		pSnapshort->Serialize(this);
        pSnapshort->Delete();
	}

	int V8HeapProfiler::ReportProgressValue(int done, int total) {
		int l = snprintf(sTmpBuff,sizeof(sTmpBuff),"{\"seq\":%d,\"type\":\"event\",\"success\":true,\"running\":true,\"event\":\"HeapProfiler.reportHeapSnapshotProgress\",\"body\":{\"done\":%d,\"total\":%d,\"finished\":%s}}",
			DebuggerAgent::g_nSeqNum++, done, total, done>=total?"true":"false");
		if(agent_->session_)
			agent_->session_->DebuggerMessage(sTmpBuff, l);
		//"HeapProfiler.reportHeapSnapshotProgress"
		return 0;
	}

	void V8HeapProfiler::startTrackingHeapObjects() {
		trackHeap = true;
		v8::HeapProfiler* pProf = v8::Isolate::GetCurrent()->GetHeapProfiler();
		pProf->StartTrackingHeapObjects();
		//这个会用到 OutputStream 接口的 WriteHeapStatsChunk 函数。
		lastSeenObj = pProf->GetHeapStats(this);
	}

	void V8HeapProfiler::stopTrackingHeapObjects(bool isExit) {
		v8::Isolate::GetCurrent()->GetHeapProfiler()->StopTrackingHeapObjects();
		lastSeenObj = 0;
		trackHeap = false;
		if(!isExit)
			takeHeapSnapshot(true);
	}

	void V8HeapProfiler::onjsupdate() {
		if (!trackHeap)
			return;
		static int id = 0;
		if (id++ % 6 != 0)return;	//临时

		v8::HeapProfiler* pProf = v8::Isolate::GetCurrent()->GetHeapProfiler();
		lastSeenObj = pProf->GetHeapStats(this);
		//pProf->GetHeapStats()
	}

	bool V8HeapProfiler::cmdHandler(int cmdseq, const char* pCmd, JsonNode* pParams) {
		if (!pCmd)
			return false;
        IConchThreadCmdMgr* pPoster = JCScriptRuntime::s_JSRT->m_pPoster;
        if (!pPoster)
            return false;
		curRequest_ = cmdseq;
		int st = (int)*pCmd;
		switch (st) {
		case 'g':
			if (strcmp(getObjByHeapObjID, pCmd) == 0) {
				if (pParams) {
					JsonObject* pars = (JsonObject*)pParams;
					JsonValue* objid = (JsonValue*)pars->getNode("objectId");
					if (objid) {
						int nobjid = atoi(objid->m_sValue);
						pPoster->postToJS(std::bind(&V8HeapProfiler::findObjectById, this, nobjid));
					}
				}
				return true;
			}
			break;
		case 's':
			if (strcmp(startHeapTrack, pCmd) == 0) {
				pPoster->postToJS(std::bind(&V8HeapProfiler::startTrackingHeapObjects,this));
			}
			else if (strcmp(stopHeapTrack, pCmd) == 0) {
				pPoster->postToJS(std::bind(&V8HeapProfiler::stopTrackingHeapObjects, this,false));
			}
			break;
		case 't':
			if (strcmp(TakeSnapshort, pCmd) == 0) {
				bool bReportProg = false;
				if (pParams) {
					JsonObject* pars = (JsonObject*)pParams;
					JsonValue* reportProg = (JsonValue*)pars->getNode("reportProgress");
					bReportProg = reportProg ? strcmp(reportProg->m_sValue, "true") == 0 : false;
				}
				pPoster->postToJS(std::bind(&V8HeapProfiler::takeHeapSnapshot,this,bReportProg));
				//takeHeapSnapshot(bReportProg);
				return true;
			}
			break;
		case '_':
			break;
		case 'h':
			break;
		}
		return false;
	}
}
#endif