
#ifndef _V8_DEBUG_CONSOLE_H__
#define _V8_DEBUG_CONSOLE_H__
#ifdef JS_V8
#include <v8.h>
#include <v8-profiler.h>

namespace laya {
	class DebuggerAgent;
	class V8Socket;
	class JsonNode;
	class V8Console {
	public:
		V8Console(DebuggerAgent* agent, V8Socket* client);
		bool cmdHandler(int cmdseq, const char* pCmd, JsonNode* pParams);
		//int ReportProgressValue(int done, int total);
		void sendToDbgConsole(char* pMsg, const char* src, int line, int colum, const char* type);
	protected:
		DebuggerAgent* agent_;
		V8Socket* client_;
		int			curRequest_;
		int			nReqSeq;
		int			nCurLogID;
	};
}
#endif
#endif