#ifndef V8_DEBUG_AGENT_H_
#define V8_DEBUG_AGENT_H_
#ifdef JS_V8
#include <v8.h>
#include <v8-debug.h>
#include <misc/boostSemaphore.h>
#include "V8HeapProfiler.h"
#include "V8CpuProfile.h"
#include "V8Console.h"
#include <thread>
#include <mutex>
#define ASSERT(condition)      ((void) 0)

namespace laya {
	class V8Socket;
	class DebuggerAgentSession;
	class JSThread;
	// Debugger agent which starts a socket listener on the debugger port and
	// handles connection from a remote debugger.
	class DebuggerAgent {
	public:
		enum Proto_MsgType {
			Request=0,
			Response=1,
			Event=2
		};
		enum Proto_Event {
			Console_messageAdded = 100,
			Perf_BaseInfo = 200,
		};

		DebuggerAgent(const char* name, int port);//如果只是测试Socket和消息收发部分，直接New DebuggerAgent(const char* name, int port)
		//如果是完整调试则要从ChormeDebugAPI开始调用
		~DebuggerAgent();

		void Shutdown();
		void WaitUntilListening();
		v8::Isolate* isolate() { return isolate_; }
		void enableJSDebug();
		/**
		* 启动js线程了，创建一个新的jsid，以后js相关的消息，都使用这个jsid。
		* 提供一个函数，希望js线程在循环中调用他。
		*/
		typedef void(*JSDebugger_OnJSUpdate)();
		typedef void(*JSDebugger_DebuggerMessage)(const v8::Debug::Message& message);
		void onJSStart(JSThread* pJSThread);
		/**
		* js线程结束了，当前的jsid就失效了，以后接收到的此id的消息都忽略。
		*/
		void onJSExit();
		void setMessageHandler();
		void DebuggerMessage(const v8::Debug::Message& message);
		void Run();
		void onjsupdate();
		//给调试器发送log
		void sendToDbgConsole(char* pMsg, const char* src, int line, int colum, const char* type);
		//发送一个对象给调试器。seq不需要设置。
		void sendToDbg(char* pJSON);
		void sendToDbg_command(int seq, const char* cmd, const char* arguments);
		void sendToDbg_event(int seq, const char* event, const char* body);
		void sendToDbg_resinfo(const char* resname, const char* event);
		void sendPerfInfo(int* pData, int nDataLen);
	private:
		void CreateSession(V8Socket* socket);

		void CloseSession();
		void OnSessionClosed(DebuggerAgentSession* session);

		v8::Isolate* isolate_;
		std::string name_;
		///*v8::base::SmartArrayPointer<const char>*/const char* name_;  // Name of the embedding application.
		int port_;  // Port to use for the agent.
		V8Socket* server_;  // Server socket for listen/accept.
		bool terminate_;  // Termination flag.
		std::mutex session_access_;  // Mutex guarging access to session_.
		semaphore terminate_now_;  // Semaphore to signal termination.
		semaphore listening_;
		friend class DebuggerAgentSession;
		bool	    v8debugEnabled_;	//已经设置v8调试了
		JSThread*	pJSThread_;
	public:
		DebuggerAgentSession*       session_;  // Current active session if any.
		std::thread*		        m_pAgentThread;
		static DebuggerAgent*		s_Instance;
		std::string				    m_strBreakEvt;	//保存的break事件，即使没有session也要保存这个，一旦有session链接上了，就发给他。
		static int g_nSeqNum;
		//DISALLOW_COPY_AND_ASSIGN(DebuggerAgent);
	public:
		static const char* const kContentLength;

		static char* ReceiveMessage(const V8Socket* conn);
		static bool SendConnectMessage(const V8Socket* conn, const char* embedding_host);
		//static bool SendMessage(const V8Socket* conn, const uint16_t* message, int msgLen);
		//這個發送的是utf8的
		static bool SendDbgMessage(const V8Socket* conn, const char* message, int msgLen);
		static bool SendDbgMessage(const V8Socket* conn, const v8::Handle<v8::String> message);
		static int ReceiveAll(const V8Socket* conn, char* data, int len);
	};


	// Debugger agent session. The session receives requests from the remote
	// debugger and sends debugger events/responses to the remote debugger.
	class DebuggerAgentSession {//: public v8::base::Thread {
	public:
		struct BinMsgHead {
			enum DataType {
				JSON,
				BIN
			};
			unsigned int flag:32;//0xfedc0123
			unsigned int version : 8;
			unsigned int session : 8;		//no use
			unsigned int type : 1;			//0 json, 1. bin
			unsigned int padd : 15;
			unsigned int payloadlen : 32;
		};
	public:
		DebuggerAgentSession(DebuggerAgent* agent, V8Socket* client);
		~DebuggerAgentSession();
		///void DebuggerMessage(v8::internal::Vector<uint16_t> message);
		void Shutdown();

		void DebuggerMessage(const char* message, int msgLen);
		void Run();
		void onjsupdate();
		void onjsexit();
		void sendToDbgConsole(char* pMsg, const char* src, int line, int colum, const char* type);
		void sendToDbg(char* pJSON);
		void sendToDbg_command(int seq, const char* cmd, const char* arguments);
		void sendToDbg_event(int seq, const char* event, const char* body);
		void sendToDbg_resinfo(const char* resname, const char* event);
		void sendPerfInfo(int* pData, int nDataLen);
		bool SendDbgMessage(const char* data, int datalen, BinMsgHead::DataType type);
	private:
		DebuggerAgent* agent_;
		V8Socket* client_;
		V8HeapProfiler*		heap_profile_;
		V8CpuProfile*		cpu_profile_;
		V8Console*			console;
	public:
		std::thread*		m_pSessionThread;
		//DISALLOW_COPY_AND_ASSIGN(DebuggerAgentSession);
	};
}
#endif
#endif  // V8_DEBUG_AGENT_H_
