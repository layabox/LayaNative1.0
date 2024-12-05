#ifndef __JSVMEnv_H__
#define __JSVMEnv_H__

#include <misc/JCWorkerThread.h>
#include "JSVMArrayBuffer.h"
#include <util/Log.h>
#include "ark_runtime/jsvm.h"
#include "libplatform/libplatform.h"
#include "mutex"

using HandleScope = JSVM_HandleScope;
using EscapableHandleScope = JSVM_EscapableHandleScope;

namespace laya
{
    class AutoHandleScope {
        public:
            AutoHandleScope();
            ~AutoHandleScope();
    
        private:
        HandleScope _handleScope;
    };
    class AutoEscHandleScope {
        public:
            AutoEscHandleScope();
            ~AutoEscHandleScope();
    
        private:
        EscapableHandleScope _escapableHandleScope;
    };

	void JSPrint( const char* p_sBuffer );
	void JSAlert( const char* p_sBuffer );
	void evalJS( const char* p_sSource );
	class IsolateData;
	class Javascript
    {
	public:
		typedef void(*voidfun)(void* data);

		Javascript();

		~Javascript();

		void init(int p_iPort);

		//在init之后，并且在js线程的情况下，可以添加js导出
		//执行一段脚本。并立即退出。
		void run(const char* p_pszJS );

		//preRunFunc 是在执行脚本之前先执行的函数，例如可以做导出对象
		void run(const char* p_pszJS, std::function<void(void)> preRunFunc );

		//执行一个函数。并立即退出
		void run(voidfun func,void* pdata);

        //不能在v8线程，在js退出后，可以进行js对象的清理
        void uninit();

        void initJSEngine();

        void uninitJSEngine();
    
        static JSVM_Env getEnv();
        static JSVM_Value getJsGlobal();

	public:
		//debug
		typedef std::recursive_mutex _MutexType;
		_MutexType m_Lock;
		int                                 m_iListenPort;
		IsolateData*						m_IsolateData;
        JSVM_Env      _env      = nullptr;
        JSVM_VM       _vm       = nullptr;
        JSVM_VMScope  _vmScope  = nullptr;
        JSVM_EnvScope _envScope = nullptr;
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
        JSVM_Env getEnv();
	protected:
		virtual void _runLoop();
		Javascript	m_JS;
	};
};
//------------------------------------------------------------------------------


#endif //__JSEnv_H__

//-----------------------------END FILE--------------------------------