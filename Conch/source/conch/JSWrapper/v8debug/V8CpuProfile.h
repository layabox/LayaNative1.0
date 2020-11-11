
#ifndef _V8_CPU_PROFILE_H__
#define _V8_CPU_PROFILE_H__
#ifdef JS_V8
#include <v8.h>
#include <v8-profiler.h>
#include "buffer/JCBuffer.h"
#include <string>

namespace laya {
	class DebuggerAgent;
	class V8Socket;
	class JsonNode;
	class strIter {
	public:
		//返回空就表示没有了。
		virtual char* get(int& len) = 0;
	};
	class StrBuff:public JCMemClass {
	public:
		StrBuff(int sz, int adsz) :JCMemClass(sz, adsz) {}
		StrBuff& operator <<(int v) {
			char buf[64];
			sprintf(buf, "%d", v);
			addStr(buf);
			return *this;
		}
		StrBuff& operator <<(unsigned int v) {
			char buf[64];
			sprintf(buf, "%d", v);
			addStr(buf);
			return *this;
		}		
		StrBuff& operator <<(int64_t v) {
			char buf[64];
			sprintf(buf, "%lld", v);
			addStr(buf);
			return *this;
		}
		StrBuff& operator <<(const char* v) {
			addStr(v);
			return *this;
		}
		StrBuff& operator <<(strIter& pIter) {
			int len;
			while (true) {
				char* pStr = pIter.get(len);
				if (!pStr||len<=0)break;
				append(pStr, len);
			}
			return *this;
		}
		void addStr(const char* str) {
			append(str, strlen(str));
		}
	};

	//替换字符串中的',",这样可以不必产生新的内存需求。
	//这个是为JSON准备的
	std::string encodeStrForJSON(const char* pStr);

	class V8CpuProfileNode {
	public:
		//转成NodeInspector希望的格式
		static char* toJSON(const v8::CpuProfileNode* pnode,StrBuff& buf);
	};
	class V8CpuProfile {
	public:
		V8CpuProfile(DebuggerAgent* agent, V8Socket* client);
		bool cmdHandler(int cmdseq, const char* pCmd, JsonNode* pParams);
		//int ReportProgressValue(int done, int total);
	public:
		void startProfiling();
		void stopProfiling(bool isExit);
		char* toJSON(v8::CpuProfile* pData, StrBuff& buf);
	protected:
		DebuggerAgent* agent_;
		V8Socket* client_;
		int			curRequest_;
		int			nReqSeq;
	};
}
#endif
#endif