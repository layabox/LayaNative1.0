
#ifndef _V8_HEAP_PROFILE_H__
#define _V8_HEAP_PROFILE_H__
#ifdef JS_V8
#include <v8.h>
#include <v8-profiler.h>

namespace laya {
	class DebuggerAgent;
	class V8Socket;
	class JsonNode;
	class V8HeapProfiler:public v8::OutputStream {
	public:
		V8HeapProfiler(DebuggerAgent* agent, V8Socket* client);
		void takeHeapSnapshot(bool needProg);
		void findObjectById(v8::SnapshotObjectId id);
		void startTrackingHeapObjects();
		void stopTrackingHeapObjects(bool isExit);
	public:
		bool cmdHandler(int cmdseq, const char* pCmd, JsonNode* pParams);
		int ReportProgressValue(int done, int total);
	public:
		void EndOfStream();
		int GetChunkSize() { return 51200; }
		v8::OutputStream::WriteResult WriteAsciiChunk(char* data, int size);
		v8::OutputStream::WriteResult WriteHeapStatsChunk(v8::HeapStatsUpdate* data, int count);
		void onjsupdate();
	protected:
		static char	sTmpBuff[512];
		DebuggerAgent* agent_;
		V8Socket* client_;
		int			curRequest_;
		v8::SnapshotObjectId lastSeenObj;
		bool	trackHeap;
	};
}
#endif
#endif