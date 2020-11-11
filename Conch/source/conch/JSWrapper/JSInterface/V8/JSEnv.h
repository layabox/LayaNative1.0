/**
@file			JSEnv.h
@brief			
@author			guo
@version		1.0
@date			2016_5_12
*/

#ifndef __JS_ENVIRMENT_H__
#define __JS_ENVIRMENT_H__

#include <v8.h>
#include <libplatform/libplatform.h>
#include <thread>
#include <mutex>
#include <v8-debug.h>
#include <misc/JCWorkerThread.h>
#include <util/ListNode.h>

namespace laya{
	//class DebuggerAgent;

	void JSPrint( const char* p_sBuffer );
	void JSAlert( const char* p_sBuffer );
	void JSAlertExit( const char* p_sBuffer );
	void evalJS( const char* p_sSource );

	const char* ToCString(const v8::String::Utf8Value& value);

	//对v8的封装。没有考虑线程的事情
	class Javascript{
	public:
		typedef void(*voidfun)(void* data);
		Javascript();

		void init(int p_iPort);
		//在init之后，并且在js线程的情况下，可以添加js导出
		//执行一段脚本。并立即退出。
		void run(const char* p_pszJS );
		//preRunFunc 是在执行脚本之前先执行的函数，例如可以做导出对象
		void run(const char* p_pszJS, std::function<void(void)> preRunFunc );
		//执行一个函数。并立即退出
		void run(voidfun func,void* pdata);
        void uninit();//不能在v8线程
		//在js退出后，可以进行js对象的清理
	public:
		v8::Isolate*	m_pIsolate;
		v8::Platform*	m_pPlatform;
		//debug
		typedef std::recursive_mutex _MutexType;
		_MutexType m_Lock;
		static v8::Persistent<v8::Context> m_DebugMessageContext;
		int m_iListenPort;
		//v8调试
		//std::thread*			m_JsDbgAgentThread;
		//DebuggerAgent*			m_v8dbgAgent;
	};

	//把Javascript对象放到一个单独的线程中执行。
	class JSThread:public JCWorkerThread{
	public:
		JSThread();
		~JSThread();

		void initialize(int p_iPort){ m_JS.init(p_iPort);};

		void uninitialize(){	//不能在v8线程	
			m_JS.uninit();
		}
		//因为要trycatch，所以只好再抄一份了。：( 
		void _defRunLoop();
		//DebuggerAgent* getDebugAgent() { return m_JS.m_v8dbgAgent; }
	protected:
		virtual void _runLoop();
		Javascript	m_JS;
	};
};

#endif //__JSEnv_H__

//-----------------------------END FILE--------------------------------