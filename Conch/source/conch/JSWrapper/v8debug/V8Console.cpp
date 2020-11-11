#ifdef JS_V8

#include "V8Console.h"
#include "debug-agent.h"
#include <util/JCJson.h>
#include "V8CpuProfile.h"
#include <util/JCCommonMethod.h>
#include <msgpack.hpp>
#include <iostream>
#include <sstream>

namespace laya {

	V8Console::V8Console(DebuggerAgent* agent, V8Socket* client) {
		agent_ = agent;
		client_ = client;
		nCurLogID = 0;
	}

	void V8Console::sendToDbgConsole(char* pMsg, const char* src, int line, int colum, const char* type) {
        if(pMsg==NULL || src==NULL){
            return;
        }
		msgpack::type::tuple<
			int,		//0 seq
			int,		//1 seqtype
			bool,		//2 predef event true则用数字表示event，否则用字符串。
			int,		//3 eventname	int或者字符串
			int,		//4 logid
			std::string,//5 msg 
			std::string,//6 src
			int,		//7 line
			int,		//8 colum
			int64_t,	//9 time
			std::string	//10 type
		> msgpack(
			DebuggerAgent::g_nSeqNum++,
			DebuggerAgent::Proto_MsgType::Event,
			true,
			DebuggerAgent::Proto_Event::Console_messageAdded,
			nCurLogID++,
			pMsg,
			src,
			line, 
			colum,
			tmGetCurms(),
			type);
		std::stringstream buffer;
		msgpack::pack(buffer, msgpack);
		buffer.seekg(0);
		std::string str(buffer.str());
		agent_->session_->SendDbgMessage(str.data(), str.size(), DebuggerAgentSession::BinMsgHead::BIN);
		// send the buffer ...
		/*
		buffer.seekg(0);

		std::string str(buffer.str());
		msgpack::unpacked result;
		msgpack::unpack(result, str.data(), str.size());
		msgpack::object deserialized = result.get();
		std::cout << deserialized << std::endl;
		//msgpack::type::tuple<int, bool, std::string> dst;
		//deserialized.convert(dst);
		*/

		//int a = 0;
		//std::string strMsg = encodeStrForJSON(pMsg);
		//int msglen = strMsg.length();
		//StrBuff strbuf(1024, 512);
		//strbuf << "{\"seq\":" << DebuggerAgent::g_nSeqNum++
		//	<< ", \"type\":\"event\",\"success\":true,\"running\":true,\"event\":\"Console.messageAdded\",\"body\":{\"message\":"
		//	<< "{\"id\":" << nCurLogID++ << ",\"source\":\"console-api\",\"level\":\"" << (char*)type <<"\",\"type\":\"" << (char*)type << "\","
		//	<< "\"line\":" << line << ",\"column\":" << colum << ",\"url\":\"" << (char*)src
		//	<< "\",\"repeatCount\":0,\"timestamp\":" << tmGetCurms()
		//	<< ",\"parameters\":[{\"handle\":0,\"type\":\"string\",\"value\":\"";
		//strbuf.append(strMsg.c_str(), msglen);
		//strbuf << "\""
		//	<< ",\"length\":" << msglen << ",\"text\":\"";
		//strbuf.append(strMsg.c_str(), msglen);
		//strbuf << "\"}]}}}";
		//agent_->DebuggerMessage()
		//agent_->session_->DebuggerMessage(strbuf.getBuf(), strbuf.size());
	}

	bool V8Console::cmdHandler(int cmdseq, const char* pCmd, JsonNode* pParams) {
		static const char* cmdAddMsg = "messageAdded";
		static const char* cmdCleared = "messagesCleared"; 
		static const char* cmdMsgRptUpdated = "messageRepeatCountUpdated"; 
		static const char* cmdClrMsg = "clearMessages";
		static const char* cmdLookupID = "_lookupConsoleId";
		if (!pCmd)
			return false;
		nReqSeq = cmdseq;
		switch (pCmd[0]){
		case 'm':
			if (strcmp(pCmd, cmdCleared) == 0) {
				int a = 0;
			}
			else if (strcmp(pCmd, cmdMsgRptUpdated) == 0) {
				int a = 0;
			}
			else{

			}
			break;
		case 'c':
			if (strcmp(pCmd, cmdClrMsg) == 0) {
				int a = 0;
			}
			break;
		case '_':
			if (strcmp(pCmd, cmdLookupID) == 0) {
				int a = 0;
			}
			break;
		default:
			break;
		}
		return true;
	}
}
#endif