#ifdef JS_V8
#include <functional>
#include "V8CpuProfile.h"
#include "debug-agent.h"
#include <util/JCJson.h>
#include "../JSInterface/V8/JSEnv.h"
#include "../../JCScrpitRuntime.h"
#include <util/JCIThreadCmdMgr.h>

using v8::CpuProfile;
using v8::Handle;
using v8::Local;
using v8::Object;
using v8::Array;
using v8::String; 

namespace laya {

	std::string encodeStrForJSON(const char* pStr) {
		std::string ret = "";
		const char* st = pStr;
		unsigned char stv;
		int len = 0;
		while (stv=*st++) {
			switch (stv) {
			case '\\':
				if (len > 0) {
					ret.append(st - len-1, len); len = 0;
				}
				ret += "\\\\"; break;
			case '\"':
				if (len > 0) {
					ret.append(st - len-1, len); len = 0;
				}
				ret += "\\\""; break;
			case '\t':
				if (len > 0) {
					ret.append(st - len - 1, len); len = 0;
				}
				ret += "\\t"; break;
			case '\r':
				if (len > 0) {
					ret.append(st - len-1, len); len = 0;
				}
				ret += "\\r"; break;
			case '\n':
				if (len > 0) {
					ret.append(st - len-1, len); len = 0;
				}
				ret += "\\n"; break;
			default:len++; break;
			}
		}
		if (len > 0) {
			ret.append(st - len-1, len); len = 0;
		}
		return ret;
	}

	/*
		返回一个对象数组
		[{line:l,hitCount:h},{}...]
	*/
	char* GetLineTicks_(v8::CpuProfileNode* node) {
		uint32_t count = node->GetHitLineCount();
		v8::CpuProfileNode::LineTick *entries = new v8::CpuProfileNode::LineTick[count];
		bool result = node->GetLineTicks(entries, count);
		if (result) {
			for (int i = 0; i < count; i++) {
				//line:entries[i].line
				//hitCount:entries[i].hit_count
			}
		}
		delete[] entries;
		return NULL;
	}

	char* V8CpuProfileNode::toJSON(const v8::CpuProfileNode* node,StrBuff& buf) {
		int32_t count = node->GetChildrenCount();
		v8::String::Utf8Value funcname(node->GetFunctionName());
		v8::String::Utf8Value url(node->GetScriptResourceName());
		std::string rep1 = encodeStrForJSON(*funcname);
		buf << "\"functionName\":\"" << (char*)rep1.c_str() << "\","
			<< "\"url\":\"" << *url << "\","
			<< "\"lineNumber\":" << node->GetLineNumber() << ","
			<< "\"callUID\":" << node->GetCallUid() << ","
			<< "\"bailoutReason\":\"" << (char*)(node->GetBailoutReason()) << "\","
			<< "\"id\":" << node->GetNodeId() << ","
			<< "\"scriptId\":" << node->GetScriptId() << ","
			<< "\"hitCount\":" << node->GetHitCount() << ","
			<< "\"children\":[";
		for (int i = 0; i < count; i++) {
			if (i > 0)buf << ",";
			buf << "{";
			StrBuff bufc(1024,1024);
			V8CpuProfileNode::toJSON(node->GetChild(i), bufc);
			buf.append(bufc.getBuffer(), bufc.getDataSize());
			buf << "}";
		}
		buf << "]";
		//line ticks
		unsigned int linetickCount = node->GetHitCount();
		v8::CpuProfileNode::LineTick *entries = new v8::CpuProfileNode::LineTick[linetickCount];
		bool result = node->GetLineTicks(entries, linetickCount);
		/*
		if(linetickCount>0 && result ){
			buf << ",\"lineTicks\":[";
			for (int i = 0; i < linetickCount; i++) {
				if (i > 0)buf << ",";
				buf << "{\"line\":" << entries[i].line << ",\"hitCount\":" << entries[i].hit_count << "}";
			}
			buf << "]";
		}
		*/
		delete [] entries;
		/*
		functionName:node->GetFunctionName();
		url:node->GetScriptResourceName()
		lineNumber:node->GetLineNumber()
		callUID:node->GetCallUid()
		bailoutReason:node->GetBailoutReason()
		id:node->GetNodeId()
		scriptId:node->GetScriptId()
		hitCount:node->GetHitCount()
		children:[CpuProfileNode,CpuProfileNode...]
		lineTicks:GetLineTicks_(node)
		*/
		return buf.getBuffer();
	}

	V8CpuProfile::V8CpuProfile(DebuggerAgent* agent, V8Socket* client) {
		agent_ = agent;
		client_ = client;
		nReqSeq = 0;
	}

	bool V8CpuProfile::cmdHandler(int cmdseq, const char* pCmd, JsonNode* pParams) {
		static const char* cmdStart = "start";
		static const char* cmdStop = "stop";
		if (!pCmd)
			return false;
		nReqSeq = cmdseq;
		switch (pCmd[0]) {
		case 's':
			if (strcmp(cmdStart, pCmd) == 0) {
                JCScriptRuntime::s_JSRT->m_pPoster->postToJS(std::bind(&V8CpuProfile::startProfiling, this));
				return true;
			}
			else if (strcmp(cmdStop, pCmd) == 0) {
				JCScriptRuntime::s_JSRT->m_pPoster->postToJS(std::bind(&V8CpuProfile::stopProfiling, this,false));
				return true;
			}
			break;
		default:
			break;
		}
		return false;
	}

	void V8CpuProfile::startProfiling() {
		v8::Isolate* pIso = v8::Isolate::GetCurrent();
		bool recsamples = true;
		const char* t = "";
		Local<String> title =  v8::String::NewFromUtf8(pIso,t, v8::String::kNormalString);
		pIso->GetCpuProfiler()->StartProfiling(title, recsamples);
	}

	void V8CpuProfile::stopProfiling(bool isExit) {
		v8::Isolate* pIso = v8::Isolate::GetCurrent();
		const char* t = "";
		Local<String> title = v8::String::NewFromUtf8(pIso, t, v8::String::kNormalString);
		CpuProfile* pNode = pIso->GetCpuProfiler()->StopProfiling(title);
		if (!pNode || isExit)
			return;
		StrBuff buf(50*1024,10*1024);
		StrBuff NodeBuf(50 * 1024, 10 * 1024);
		buf << "{\"seq\":" << DebuggerAgent::g_nSeqNum++
			<< ",\"type\":\"response\",\"request_seq\":" << nReqSeq
			<< ",\"command\":\"Profiler.stop\",\"success\":true,\"running\":true,\"body\":{";
			toJSON(pNode, NodeBuf);
			buf.append(NodeBuf.getBuffer(), NodeBuf.getDataSize());
			buf << "}}";

		agent_->session_->DebuggerMessage(buf.getBuffer(), buf.getDataSize());
	}

	char* V8CpuProfile::toJSON(v8::CpuProfile* pNode, StrBuff& buf) {
		/*
		typeId:CPU
		uid:id++
		title:Profile i++
		head:{rootNode}
		startTime:sttm
		endTime:edtm
		samples:[nodeid,nodeid,nodeid...]
		timestamps:[timestamp,timestamp..]
		*/
		static int uid = 1;
		//Local<String> title = pNode->GetTitle();
		unsigned int sttm = (unsigned int)(pNode->GetStartTime() / 1000000);
		unsigned int edtm = (unsigned int)(pNode->GetEndTime() / 1000000);
		int count = pNode->GetSamplesCount();
		StrBuff headbuf(20*1024,20*1024);
		V8CpuProfileNode::toJSON(pNode->GetTopDownRoot(), headbuf);

		buf << "\"profile\":{\"typeId\":\"CPU\",\"uid\":" << uid << ",\"title\":\"Profile " << uid++ << "\",\"head\":{";
		buf.append(headbuf.getBuffer(), headbuf.getDataSize());
		buf	<< "},\"startTime\":" << sttm << ",\"endTime\":" << edtm << ",\"samples\":[";
		for (int i = 0; i < count; i++) {
			if (i > 0)buf << ",";
			buf << pNode->GetSample(i)->GetNodeId();
		}
		buf << "],\"timestamps\":[";
		for (int i = 0; i < count; i++) {
			if (i > 0)buf << ",";
			buf << pNode->GetSampleTimestamp(i);
			//Local<String> funcname = pNode->GetSample(i)->GetFunctionName();
		}
		buf << "]}";
		return buf.getBuffer();
	}
}
#endif