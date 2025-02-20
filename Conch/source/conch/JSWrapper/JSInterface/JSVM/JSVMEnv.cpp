﻿#ifdef JS_JSVM
#include "JSVMEnv.h"
#include "JSVMArrayBuffer.h"
#include "util/Log.h"
#include "JSVMProxyTLS.h"
#ifdef WIN32
    #include <windows.h>
    #include <process.h>
#endif
#include "../conch/JCScriptRuntime.h"
#include "../conch/JCConch.h"
#include "IsolateData.h"

namespace laya
{
    AutoHandleScope::AutoHandleScope() {
        OH_JSVM_OpenHandleScope(Javascript::getEnv(), &_handleScope);
    }
    
    AutoHandleScope::~AutoHandleScope() {
        OH_JSVM_CloseHandleScope(Javascript::getEnv(), _handleScope);
    }

    AutoEscHandleScope::AutoEscHandleScope() {
        OH_JSVM_OpenEscapableHandleScope(Javascript::getEnv(), &_escapableHandleScope);
    }
    
    AutoEscHandleScope::~AutoEscHandleScope() {
        OH_JSVM_CloseEscapableHandleScope(Javascript::getEnv(), _escapableHandleScope);
    }

	Javascript::Javascript() 
    {
		static bool initialized = false;
        if (initialized) {
            return;
        }
        //TODO 初始化添加--expose_gc 可暴露全局的gc方法
        OH_JSVM_Init(nullptr);
        initialized = true;
	}
	Javascript::~Javascript() = default;

	void Javascript::init(int p_iPort)
    {
		if (p_iPort > 0 && p_iPort <0xFFFF)
		{
			m_iListenPort = p_iPort;
		} else 
            m_iListenPort = 0;
	}
    void Javascript::uninit() 
    {
       
    }

    void Javascript::initJSEngine()
    {
		JSObjBaseJSVM::restarting = false;
        JSVM_Status status;
        JSVM_API_CALL(status, _env, OH_JSVM_CreateVM(nullptr, &_vm));
        JSVM_API_CALL(status, _env, OH_JSVM_OpenVMScope(_vm, &_vmScope));
        JSVM_API_CALL(status, _env, OH_JSVM_CreateEnv(_vm, 0, nullptr, &_env));
        JSVM_API_CALL(status, _env, OH_JSVM_OpenEnvScope(_env, &_envScope));
    
        m_IsolateData = new IsolateData(_env);
    }
    void Javascript::uninitJSEngine()
    {
        JSVM_Status status;
        JSVM_API_CALL(status, _env, OH_JSVM_CloseEnvScope(_env, _envScope));
        JSVM_API_CALL(status, _env, OH_JSVM_DestroyEnv(_env));
        JSVM_API_CALL(status, _env, OH_JSVM_CloseVMScope(_vm, _vmScope));
        JSVM_API_CALL(status, _env, OH_JSVM_DestroyVM(_vm));
        _envScope = nullptr;
        _env = nullptr;
        _vmScope = nullptr;
        _vm = nullptr;
    }

    JSVM_Env Javascript::getEnv() {
        return JCConch::s_pConch->m_pScrpitRuntime->m_ScriptThread.getEnv();
    }

    JSVM_Value Javascript::getJsGlobal() {
        JSVM_Value global;
        JSVM_Status status;
        auto env = getEnv();
        JSVM_API_CALL(status, env, OH_JSVM_GetGlobal(env, &global));
        return global;
    }

	void Javascript::run(const char* p_pszJS, std::function<void(void)> preRunFunc)
    {
	}
	void Javascript::run(const char* p_pszJS)
    {
	}
	void Javascript::run(voidfun func, void* pdata) 
    {
        func(pdata);
	}
    //------------------------------------------------------------------------------
    JSThread::JSThread()
    {
        setThreadName("JavaScript Main");
    }
    JSThread::~JSThread()
    {

    }
	void JSThread::_defRunLoop()
	{
#ifdef WIN32
		{
			DWORD thid = GetCurrentThreadId();
			SetNameInternal(thid, m_strName.c_str());
			//threadInfoLog("start thread:%s,%d", m_strName.c_str(), thid);
		}
#elif ANDROID
		{
			//threadInfoLog("start thread:%s,%ld", m_strName.c_str(), gettidv1());
		}
#endif
		//开始事件
		JCEventEmitter::evtPtr startEvt(new JCEventBase);
		startEvt->m_nID = JCWorkerThread::Event_threadStart;
		emit(startEvt);
        runObj task;
		while (!m_bStop)
		{
			if (!m_funcLoop)
			{
				//现在的waitdata返回false不再表示要退出。事件唤醒流程
				if (m_ThreadTasks.WaitData(&task))
					task();
			}
			else
			{
				//固定循环流程
				runQueue();
                if (!m_funcLoop()) {
                    break;
                }
			}
		}
		//退出事件
		JCEventEmitter::evtPtr stopEvt(new JCEventBase);
		stopEvt->m_nID = JCWorkerThread::Event_threadStop;
		emit(stopEvt);
	}
	void call_JSThread__defRunLoop(void* pdata) 
    {
        JSThread* pthis=(JSThread*)pdata;
		pthis->_defRunLoop();
	}
	void JSThread::_runLoop()
    {
        m_JS.initJSEngine();
		m_JS.run(call_JSThread__defRunLoop,this);
        m_JS.uninitJSEngine();
	}

    JSVM_Env JSThread::getEnv() {
        return m_JS._env;
    }
};
#endif
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------