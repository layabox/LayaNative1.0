
#ifndef V8_DEBUG_AGENT_H_
#define V8_DEBUG_AGENT_H_
#ifdef JS_V8_DEBUGGER
#include <v8.h>
#include <v8-inspector.h>
//#include <misc/boostSemaphore.h>
#include <thread>
#include <mutex>
#include "buffer/JCBuffer.h"

#define ASSERT(condition)      ((void) 0)

namespace laya {
	class JSMThread;
    class JSThreadInterface;
    class strIter {
    public:
        //���ؿվͱ�ʾû���ˡ�
        virtual char* get(int& len) = 0;
    };
    class StrBuff :public JCMemClass {
    public:
        StrBuff(int sz, int adsz) :JCMemClass(sz, adsz) {}
        StrBuff& operator <<(int v) {
            char buf[64];
            sprintf(buf, "%d", v);
            addStr(buf);
            return *this;
        }
        StrBuff& operator <<(double v) {
            char buf[64];
            sprintf(buf, "%f", v);
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
                if (!pStr || len <= 0)break;
                append(pStr, len);
            }
            return *this;
        }
        void addStr(const char* str) {
            append(str, strlen(str));
        }
    };

    //�滻�ַ����е�',",�������Բ��ز����µ��ڴ�����
    //�����ΪJSON׼����
    std::string encodeStrForJSON(const char* pStr);
    class InspectorFrontend;
    class per_session_data__v8dbg;
	class DebuggerAgent {
	public:
		DebuggerAgent(const char* name, int port);//���ֻ�ǲ���Socket����Ϣ�շ����֣�ֱ��New DebuggerAgent(const char* name, int port)
		//���������������Ҫ��ChormeDebugAPI��ʼ����
		~DebuggerAgent();

		void Shutdown();
		v8::Isolate* isolate() { return isolate_; }
		/**
		* ����js�߳��ˣ�����һ���µ�jsid���Ժ�js��ص���Ϣ����ʹ�����jsid��
		* �ṩһ��������ϣ��js�߳���ѭ���е�������
		*/
		void onJSStart(JSThreadInterface* pJSThread,bool bDebugWait);
		/**
		* js�߳̽����ˣ���ǰ��jsid��ʧЧ�ˣ��Ժ���յ��Ĵ�id����Ϣ�����ԡ�
		*/
		void onJSExit();
		//������������log
		void sendToDbgConsole(char* pMsg, const char* src, int line, int colum, const char* type);

        void onAcceptNewFrontend(per_session_data__v8dbg* pData);
        void onFrontEndClose();
        void onDbgMsg(char* pMsg, int len);
        void sendMsgToFrontend(char* pMsg, int len);
        void onMsgToV8End(int id);    //js�߳�ִ����ϵĻص�������js�߳�
	private:

		v8::Isolate* isolate_;
		std::string name_;
		int port_; 
		bool terminate_;  
		std::mutex session_access_;  // Mutex guarging access to session_.
		//semaphore terminate_now_;  // Semaphore to signal termination.
		JSThreadInterface*	pJSThread_;
        per_session_data__v8dbg*    pWsSessionData=nullptr;
        bool        bHasFrontend = false;//�ȵ���������������js
        bool        bFirst = true;
        int         nFrontEndMsgID = 0; //�Լ��涨����Ϣid�������ǽ�������Ϣ��json����������һЩ��������Ҫ���ж��Ƿ�����Debugger.enable
        int         nEnableDebuggerMsgID = -1;  //�ȴ������Ϣ��������������Ҫ��¼���ĸ�
        static int  sMsgID;
	public:
        std::unique_ptr<v8_inspector::V8Inspector> _new_inspector;
        std::unique_ptr<v8_inspector::V8InspectorSession> _dbg_session_; //new debugger
        v8_inspector::V8InspectorClient*    m_pInspectorClient=nullptr;
        InspectorFrontend*                  m_pInspectorChannel = nullptr;
	};
}
#endif
#endif  // V8_DEBUG_AGENT_H_
