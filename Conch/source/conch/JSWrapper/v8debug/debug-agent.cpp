
#ifdef JS_V8
#include "debug-agent.h"
#include <util/Log.h>
#include "V8Socket.h"
#include <fileSystem/JCFileSystem.h>
#include <util/JCJson.h>
#include <util/JCCommonMethod.h>
#include "../LayaWrap/JSConchConfig.h"
#include <msgpack.hpp>
#include "../../JCScrpitRuntime.h"

using namespace laya;
#define DEBUGGERBINPROTVERSION 1

bool g_bSendLogToDbg = true;

void mygLayaLog(int level, const char* file, int line, const char* fmt, ...) {
    if (!JCScriptRuntime::s_JSRT)
        return;
    DebuggerAgent* pDbgAgent = JCScriptRuntime::s_JSRT->m_pDbgAgent;
	if (!g_bSendLogToDbg || !pDbgAgent) {
		va_list args;
		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);
		return;
	}
	char buf[1024];
	char* pBuf = NULL;
	va_list args;
	va_start(args, fmt);
	int len = vsprintf(buf, fmt, args);
	if (len < 0) {
		pBuf = new char[4096];
		len = vsprintf(pBuf, fmt, args);
		if (len < 0) {
			printf("log buffer is not enough!\n");
			//继续
		}
	}
	va_end(args);
	const char* pTypes[] = { "warning","error", "debug", "log","runtime" };
	int sz = sizeof(pTypes) / sizeof(const char*);
    pDbgAgent->sendToDbgConsole(pBuf ? pBuf : buf, file, line, 0, level<sz ? pTypes[level] : "unknown");
	if (pBuf) {
		delete[] pBuf;
	}
}

void mygLayaLogSimp(int level, const char* file, int line, const char* msg) {
    if (!JCScriptRuntime::s_JSRT)
        return;
    DebuggerAgent* pDbgAgent = JCScriptRuntime::s_JSRT->m_pDbgAgent;
    if (!g_bSendLogToDbg || !pDbgAgent) {
		printf("%s", msg);
		return;
	}
	const char* pTypes[] = { "warning","error", "debug", "log","runtime" };
	int sz = sizeof(pTypes) / sizeof(const char*);
	pDbgAgent->sendToDbgConsole((char*)msg, file, line, 0, level<sz ? pTypes[level] : "unknown");
}


void mygLayaLogBin(int level, const char* file, int line, void* pData, int len) {
}

#endif

#ifdef JS_V8
namespace laya {
	const char* const DebuggerAgent::kContentLength = "Content-Length";
	DebuggerAgent*		DebuggerAgent::s_Instance = NULL;
	int DebuggerAgent::g_nSeqNum = 0;
	DebuggerAgent::DebuggerAgent(const char* name, int port) :name_(name),
		port_(port),
		server_(new Win32Socket()), 
		terminate_(false),
		session_(NULL),
		terminate_now_(0),
		listening_(0) {
		g_nSeqNum = 0;
		s_Instance = this;
		m_pAgentThread = new std::thread(std::bind(&DebuggerAgent::Run, this));;
		v8debugEnabled_ = false;
		pJSThread_ = NULL;
		isolate_ = NULL;
	}

	DebuggerAgent::~DebuggerAgent() {
		delete server_;
	}

	void DebuggerAgentMessageHandler(const v8::Debug::Message& message) {
		if (DebuggerAgent::s_Instance)
			DebuggerAgent::s_Instance->DebuggerMessage(message);
	}
	void DebuggerAgent::setMessageHandler() {
		isolate_ = (v8::Isolate::GetCurrent());
		v8::Debug::SetMessageHandler(DebuggerAgentMessageHandler);
	}
	void DebuggerAgent::enableJSDebug() {
		v8debugEnabled_ = true;
		if( pJSThread_)
			pJSThread_->post(std::bind(&DebuggerAgent::setMessageHandler,this));
	}

	void DebuggerAgent::onJSStart(JSThread* pJSThread) {
		pJSThread_ = pJSThread;
		if (v8debugEnabled_ && pJSThread_) {
			//pJSThread_->post(std::bind(&DebuggerAgent::setMessageHandler, this));
			//肯定是在js线程，立即执行。否则debugger来不及停止
			setMessageHandler();
		}
	}

	void DebuggerAgent::onJSExit() { 
		if (session_) session_->onjsexit();
		pJSThread_ = NULL;
		isolate_ = NULL;
	}

	void DebuggerAgent::onjsupdate() {
		if (session_) session_->onjsupdate();
	}

	// Debugger agent main thread.
	void DebuggerAgent::Run() {
		const int kOneSecondInMicros = 1000;// 1000000;
		// Allow this socket to reuse port even if still in TIME_WAIT.
		server_->SetReuseAddress(true);

		// First bind the socket to the requested port.
		bool bound = false;
		while (!bound && !terminate_) {
			bound = server_->Bind(port_);

			// If an error occurred wait a bit before retrying. The most common error
			// would be that the port is already in use so this avoids a busy loop and
			// make the agent take over the port when it becomes free.
			if (!bound) {
				LOGE("DebuggerAgent::Run Failed to open socket on port %d, "
					"waiting %d ms before retrying\n", port_, kOneSecondInMicros);// / 1000);
				terminate_now_.wait(kOneSecondInMicros);
			}
		}

		// Accept connections on the bound port.
		while (!terminate_) {
			bool ok = server_->Listen(2);
			listening_.signal();
			if (ok) {
				// Accept the new connection.
				V8Socket* client = server_->Accept();
				ok = client != NULL;
				if (ok) {
					// Create and start a new session.
					CreateSession(client);
				}
			}
		}
	}

	void DebuggerAgent::Shutdown() {
		terminate_ = true;

		// Signal termination and make the server exit either its listen call or its
		// binding loop. This makes sure that no new sessions can be established.
		terminate_now_.signal();
		server_->Shutdown();

		if(m_pAgentThread)
			m_pAgentThread->join();
		delete m_pAgentThread;
		// Close existing session if any.
		CloseSession();
	}

	void DebuggerAgent::WaitUntilListening() {
		listening_.wait();
	}

	static const char* kCreateSessionMessage =
		"Remote debugging session already active\r\n";

	void DebuggerAgent::CreateSession(V8Socket* client) {
        std::unique_lock<std::mutex> _lock(session_access_);
		//ScopedLock with(session_access_);

		// If another session is already established terminate this one.
		if (session_ != NULL) {
			client->Send(kCreateSessionMessage, strlen(kCreateSessionMessage));
			delete client;
			return;
		}

		// Create a new session and hook up the debug message handler.
		session_ = new DebuggerAgentSession(this, client);
		session_->m_pSessionThread = new std::thread(std::bind(&DebuggerAgentSession::Run, session_));
	}

	void DebuggerAgent::CloseSession() {
        std::unique_lock<std::mutex> _lock(session_access_);
		// Terminate the session.
		if (session_ != NULL) {
			session_->Shutdown();
			session_->m_pSessionThread->join();
			delete session_;
			session_ = NULL;
		}
	}

	//TODO 这个要发给线程发送
	void DebuggerAgent::DebuggerMessage(const v8::Debug::Message& message) {
		//int64_t st = tmGetCurms();
        std::unique_lock<std::mutex> _lock(session_access_);

		//如果没有客户端连接进来，break事件要保存
		if (session_ != NULL || message.GetEvent() == v8::DebugEvent::Break) {
			v8::String::Value val(message.GetJSON());	//這個返回的是UTF16的
			int unicodeLen = val.length();
			char* pUTF8Buff = new char[unicodeLen * 4];
			int uniNum=0, buffUsed=0;
			char* ret = UnicodeStrToUTF8Str((short*)*val, pUTF8Buff, unicodeLen * 4, uniNum, buffUsed);
			//返回的字符串包含0，需要去掉，因為協議不要這個。
			if (ret != 0) {
				//writeFileSync1("d:/temp/aaa_v8out.txt", ret, buffUsed-1, buffer::raw );
				if (session_)
					session_->DebuggerMessage((const char*)pUTF8Buff, buffUsed - 1);
				else if(message.IsEvent() && message.GetEvent() == v8::DebugEvent::Break)
					m_strBreakEvt = ret;
			}
			delete [] pUTF8Buff;
		}
		//int dt = (int)(tmGetCurms() - st);
		//LOGW("SEND debug info tm:%d", dt);
	}

	void DebuggerAgent::sendToDbgConsole(char* pMsg, const char* src, int line, int colum, const char* type) {
		if (session_)
			session_->sendToDbgConsole(pMsg, src, line, colum, type);
	}
	void DebuggerAgent::sendToDbg(char* pJSON) {
		if (session_)
			session_->sendToDbg(pJSON);
	}
	void DebuggerAgent::sendToDbg_command(int seq, const char* cmd, const char* arguments) {
		if (session_)
			session_->sendToDbg_command(seq, cmd, arguments);
	}

	void DebuggerAgent::sendToDbg_event(int seq, const char* event, const char* body) {
		if (session_)
			session_->sendToDbg_event(seq, event, body);
	}

	void DebuggerAgent::sendToDbg_resinfo(const char* resname, const char* event) {
		if (session_)
			session_->sendToDbg_resinfo(resname, event);
	}
	void DebuggerAgent::sendPerfInfo(int* pData, int nDataLen) {
		if (session_)
			session_->sendPerfInfo(pData, nDataLen);
	}

	void DebuggerAgent::OnSessionClosed(DebuggerAgentSession* session) {
		if (terminate_) {
			return;
		}

		// Terminate the session.
		std::unique_lock<std::mutex> _lock(session_access_);
		ASSERT(session == session_);
		if (session == session_) {
			session_->Shutdown();
			delete session_;
			session_ = NULL;
		}
	}

	DebuggerAgentSession::DebuggerAgentSession(DebuggerAgent* agent, V8Socket* client) :
		agent_(agent), client_(client) {
		m_pSessionThread = NULL;
		heap_profile_ = new V8HeapProfiler(agent, client);
		cpu_profile_ = new V8CpuProfile(agent,client);
		console = new V8Console(agent, client);
		gLayaLog = mygLayaLog;
		gLayaLogNoParam = mygLayaLogSimp;
	}

	DebuggerAgentSession::~DebuggerAgentSession() {
		if (!heap_profile_)
			delete heap_profile_;
		if (!cpu_profile_)
			delete cpu_profile_;
		if (!console)
			delete console;
	}

	void DebuggerAgentSession::onjsupdate() {
		if (heap_profile_) {
			heap_profile_->onjsupdate();
		}
	}

	void DebuggerAgentSession::onjsexit() {
		//停止profile
		if (cpu_profile_) {
			cpu_profile_->stopProfiling(true);
		}
		if (heap_profile_) {
			heap_profile_->stopTrackingHeapObjects(true);
		}
		//发送给调试器
		StrBuff strbuf(1024, 512);
		strbuf << "{\"seq\":" << 0 << ",\"type\":\"event\",\"event\":\"jsexit\", \"running\":false}";
		DebuggerMessage(strbuf.getBuffer(), strbuf.getDataSize());
	}

	void DebuggerAgentSession::sendToDbgConsole(char* pMsg, const char* src, int line, int colum, const char* type) {
		console->sendToDbgConsole(pMsg, src, line, colum, type);
	}

	void DebuggerAgentSession::sendToDbg(char* pJSON) {
		DebuggerMessage(pJSON, strlen(pJSON));
	}

	void DebuggerAgentSession::sendToDbg_command(int seq, const char* cmd, const char* arguments) {
		StrBuff strbuf(1024, 512);
		strbuf << "{\"seq\":" << seq << ",\"type\":\"command\",\"command\":\"" << (char*)cmd << "\",\"success\":true,\"arguments\":"
			<< (char*)arguments << ", \"running\":true}";
		DebuggerMessage(strbuf.getBuffer(), strbuf.getDataSize());
	}

	void DebuggerAgentSession::sendToDbg_event(int seq, const char* event, const char* body) {
		StrBuff strbuf(1024, 512);
		strbuf<<"{\"seq\":"<<seq<<",\"type\":\"event\",\"event\":\""<<(char*)event<<"\",\"success\":true,\"body\":"<<(char*)body<<",\"running\":true}";
		DebuggerMessage(strbuf.getBuffer(), strbuf.getDataSize());
	}

	void DebuggerAgentSession::sendToDbg_resinfo(const char* resname, const char* event) {

	}

	void DebuggerAgentSession::sendPerfInfo(int* pData, int nDataLen) {
		msgpack::type::tuple<
			int,		//0 seqid
			int,		//1 seqtype
			bool,		//2 predef event true则用数字表示event，否则用字符串。
			int,		//3 eventname	int或者字符串
			int,		//4 delaymax
			int,		//5 delaymin
			int,		//6 imgfilemax
			int,		//7 imgfilemin
			int,		//8 texmax
			int,		//9 texmin
			int,		//10 ejanimmax
			int			//11 ejanimmin
		> msgpack(
			DebuggerAgent::g_nSeqNum++,
			DebuggerAgent::Proto_MsgType::Event,
			true,
			DebuggerAgent::Proto_Event::Perf_BaseInfo,
			pData[0], pData[1], pData[2], pData[3],
			pData[4], pData[5], pData[6], pData[7]
			);
		std::stringstream buffer;
		msgpack::pack(buffer, msgpack);
		buffer.seekg(0);
		std::string str(buffer.str());
		agent_->session_->SendDbgMessage(str.data(), str.size(), DebuggerAgentSession::BinMsgHead::BIN);
	}

	void DebuggerAgentSession::Run() {
		// Send the hello message.
		bool ok = DebuggerAgent::SendConnectMessage(client_, agent_->name_.c_str());
		if (!ok) return;
		//发送存储的的break信息
		int breakStrLen = agent_->m_strBreakEvt.length();
		if (breakStrLen) {
			DebuggerMessage(agent_->m_strBreakEvt.c_str(), breakStrLen);
			agent_->m_strBreakEvt = "";
		}
		while (true) {
			// Read data from the debugger front end.
			char* message = DebuggerAgent::ReceiveMessage(client_);
			bool toV8 = true;
			const char* msg = (const char*)message;
			bool is_closing_session = (msg == NULL);

			int msglen = 0;
			if (msg == NULL) {
				// If we lost the connection, then simulate a disconnect msg:
				msg = "{\"seq\":1,\"type\":\"request\",\"command\":\"disconnect\"}";
				msglen = strlen(msg);
			}
			else {
				//writeFileSync1("d:/temp/v8in.txt", message, strlen(message), buffer::raw);
				//先检查一下command，分配给不同的对象处理。
				msglen = strlen(msg);
				char* ptmpMsg = new char[msglen + 1];
				ptmpMsg[msglen] = 0;
				memcpy(ptmpMsg, msg, msglen);
				JCJson json;
				if (json.paserJson((char*)ptmpMsg)) {
					JsonObject* pRoot = (JsonObject*)json.getRoot();
					JsonValue* pCmd = (JsonValue*)pRoot->getNode("command");
					JsonValue* pSeq = (JsonValue*)pRoot->getNode("seq");
					int nSeq = pSeq ? atoi(pSeq->m_sValue) : 0;

					if (pCmd) {
						static const char* HEAPPROFILER = "HeapProfiler.";
						static const char* PROFILER = "Profiler.";
						static const char* CONSOLE = "Console.";
						switch ((intptr_t)*pCmd->m_sValue) {
						case 'H':
							if (memcmp(pCmd->m_sValue, HEAPPROFILER,strlen(HEAPPROFILER)) == 0) {
								toV8 = false;
								bool bRet = 
									heap_profile_->cmdHandler(nSeq,pCmd->m_sValue + strlen(HEAPPROFILER), pRoot->getNode("arguments"));
							}
							break;
						case 'C':
							if (memcmp(pCmd->m_sValue, CONSOLE, strlen(CONSOLE)) == 0) {
								toV8 = false;
								bool bRet =
									console->cmdHandler(nSeq, pCmd->m_sValue + strlen(HEAPPROFILER), pRoot->getNode("arguments"));
							}
							break;
						case 'd':
							if (strcmp(pCmd->m_sValue + 1, "isconnect") == 0) {
								//先看是不是断开连接的消息
								is_closing_session = true;
							}
							break;
						case 'P':
							if (memcmp(pCmd->m_sValue, PROFILER, strlen(PROFILER)) == 0) {
								toV8 = false;
								//start,stop
								bool bRet = cpu_profile_->cmdHandler(nSeq, pCmd->m_sValue + strlen(PROFILER), pRoot->getNode("arguments"));
							}
						}
					}
				}
				delete [] ptmpMsg;
			}

			if (toV8) {
				// Convert UTF-8 to UTF-16.
				uint16_t* pUTF16 = new uint16_t[msglen * 4];
				int uniLen = UTF8StrToUnicodeStr((unsigned char*)msg, pUTF16, msglen);
				// Send the request received to the debugger.
				v8::Isolate* piso = agent_->isolate();
				if(piso)
					v8::Debug::SendCommand(piso, (const uint16_t*)pUTF16, uniLen, NULL);
				delete[] pUTF16;
			}
			delete[] message;

			if (is_closing_session) {
				// Session is closed.
				agent_->OnSessionClosed(this);
				return;
			}
		}
	}

	void DebuggerAgentSession::DebuggerMessage(const char* message, int msgLen) {
		//printf("v8 send:%s\n", message);
		DebuggerAgent::SendDbgMessage(client_,message,msgLen);
	}

	void DebuggerAgentSession::Shutdown() {
		// Shutdown the socket to end the blocking receive.
		client_->Shutdown();
	}

	bool DebuggerAgentSession::SendDbgMessage(const char* data, int datalen, BinMsgHead::DataType type) {
		if (!client_)return false;
		BinMsgHead head;
		head.padd = 0;
		head.flag = 0xfedc0123;
		head.version = DEBUGGERBINPROTVERSION;
		head.type = type;
		head.payloadlen = datalen;
		client_->Send((char*)&head, sizeof(head));
		client_->Send(data, datalen);
		return true;
	}

	/*
		如果失败，则返回NULL，
			失败了则无法继续了么？
		否则返回的字符串是new出来的。
	*/
	char* DebuggerAgent::ReceiveMessage(const V8Socket* conn) {
		int received;

		// Read header.
		int content_length = 0;
		while (true) {
			const int kHeaderBufferSize = 80;
			char header_buffer[kHeaderBufferSize];
			int header_buffer_position = 0;
			char c = '\0';  // One character receive buffer.
			char prev_c = '\0';  // Previous character.

			// Read until CRLF.
			while (!(c == '\n' && prev_c == '\r')) {
				prev_c = c;
				received = conn->Receive(&c, 1);
				if (received == 0) {
					LOGE("Error %d\n", V8Socket::LastError());
					return NULL;
				}

				// Add character to header buffer.
				if (header_buffer_position < kHeaderBufferSize) {
					header_buffer[header_buffer_position++] = c;
				}
			}

			// Check for end of header (empty header line).
			if (header_buffer_position == 2) {  // Receive buffer contains CRLF.
				break;
			}

			// Terminate header.
			ASSERT(header_buffer_position > 1);  // At least CRLF is received.
			ASSERT(header_buffer_position <= kHeaderBufferSize);
			header_buffer[header_buffer_position - 2] = '\0';

			// Split header.
			char* key = header_buffer;
			char* value = NULL;
			for (int i = 0; header_buffer[i] != '\0'; i++) {
				if (header_buffer[i] == ':') {
					header_buffer[i] = '\0';
					value = header_buffer + i + 1;
					while (*value == ' ') {
						value++;
					}
					break;
				}
			}

			// Check that key is Content-Length.
			if (strcmp(key, kContentLength) == 0) {
				// Get the content length value if present and within a sensible range.
				if (value == NULL || strlen(value) > 7) {
					return NULL;
				}
				for (int i = 0; value[i] != '\0'; i++) {
					// Bail out if illegal data.
					if (value[i] < '0' || value[i] > '9') {
						LOGE("content length 后面跟的不是数字。");
						return NULL;
					}
					content_length = 10 * content_length + (value[i] - '0');
				}
			}
			else {
				// For now just print all other headers than Content-Length.
				LOGE("%s: %s\n", key, value != NULL ? value : "(no value)");
			}
		}

		// Return now if no body.
		if (content_length == 0) {
			return NULL;
		}

		// Read body.
		char* buffer = new char[content_length + 1];
		received = ReceiveAll(conn, buffer, content_length);
		if (received < content_length) {
			LOGE("Error %d\n", V8Socket::LastError());
			delete[] buffer;
			return NULL;
		}
		buffer[content_length] = '\0';

		return buffer;
	}


	bool DebuggerAgent::SendConnectMessage(const V8Socket* conn, const char* embedding_host) {
		static const int kBufferSize = 80;
		char buffer[kBufferSize];  // Sending buffer.
		bool ok;
		int len;

		// Send the header.
		len = snprintf(buffer, kBufferSize,	"Type: connect\r\n");
		ok = conn->Send(buffer, len)>=len;
		if (!ok) return false;

		len = snprintf(buffer, kBufferSize,	"V8-Version: %s\r\n", v8::V8::GetVersion());
		ok = conn->Send(buffer, len)>=len;
		if (!ok) return false;

		len = snprintf(buffer, kBufferSize,	"Protocol-Version: 1\r\n");
		ok = conn->Send(buffer, len)>=len;
		if (!ok) return false;

		if (embedding_host != NULL) {
			len = snprintf(buffer, kBufferSize,	"Embedding-Host: %s\r\n", embedding_host);
			ok = conn->Send(buffer, len)>=len;
			if (!ok) return false;
		}

		len = snprintf(buffer, kBufferSize, "layabox version: %s\r\n", JSConchConfig::getInstance()->getRuntimeVersion());
		ok = conn->Send(buffer, len) >= len;
		if (!ok) return false;

		len = snprintf(buffer, kBufferSize,	"%s: 0\r\n", kContentLength);
		ok = conn->Send(buffer, len)>=len;
		if (!ok) return false;

		// Terminate header with empty line.
		len = snprintf(buffer, kBufferSize, "\r\n");
		ok = conn->Send(buffer, len)>=len;
		if (!ok) return false;

		// No body for connect message.
		return true;
	}

	bool DebuggerAgent::SendDbgMessage(const V8Socket* conn, const char* message, int msgLen) {
		static const int kBufferSize = 80;
		char buffer[kBufferSize];  // Sending buffer both for header and body.

		// Send the header.
		int len;
		len = snprintf(buffer, kBufferSize, "Content-Length: %d\r\n", msgLen);
		conn->Send(buffer, len);

		// Terminate header with empty line.
		len = snprintf(buffer, kBufferSize, "\r\n");
		conn->Send(buffer, len);

		// Send message body as UTF-8.
		conn->Send(message, msgLen);

		return true;
	}

	bool DebuggerAgent::SendDbgMessage(const V8Socket* conn,	const v8::Handle<v8::String> request) {
		// Convert the request to UTF-8 encoding.
		v8::String::Utf8Value utf8_request(request);
		return SendDbgMessage(conn, *utf8_request, utf8_request.length());
	}

	// Receive the full buffer before returning unless an error occours.
	int DebuggerAgent::ReceiveAll(const V8Socket* conn, char* data, int len) {
		int total_received = 0;
		while (total_received < len) {
			int received = conn->Receive(data + total_received, len - total_received);
			if (received == 0) {
				return total_received;
			}
			total_received += received;
		}
		return total_received;
	}
}

#endif