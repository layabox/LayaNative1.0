#ifdef JS_V8

#include "JSEnv.h"
#include <v8-profiler.h>
#include "JSArrayBuffer.h"
#ifdef JS_V8_DEBUGGER
#include "../../v8debug/debug-agent.h"
#endif
#include "util/Log.h"
#include "JSCProxyTLS.h"
#ifdef WIN32
#include <windows.h>
#include <process.h>
#endif
#include "IsolateData.h"
//#define V8PROFILE

namespace laya{
	
	v8::Persistent<v8::Context> Javascript::m_DebugMessageContext;

	const char* ToCString(const v8::String::Utf8Value& value){
		return *value ? *value : "<string conversion failed>";
	}
   Javascript::Javascript()
   {
		m_pIsolate = NULL;
		m_pPlatform = NULL;
        m_iListenPort = 0;


		m_pPlatform = v8::platform::NewDefaultPlatform().release();
		v8::V8::InitializePlatform(m_pPlatform);
		v8::V8::Initialize();
		static char* flags[] =
		{
#if defined(OHOS) || defined(__APPLE__)
            " --jitless",
#endif
			"--expose_gc",
			"--no-flush-bytecode",
			"--no-lazy",
		};
		for (auto f : flags)
		{
			v8::V8::SetFlagsFromString(f, strlen(f));
		}
   }
	Javascript::~Javascript() {
     	v8::V8::Dispose();
		v8::V8::ShutdownPlatform();
		delete m_pPlatform;
	}

	void Javascript::init(int p_iPort) {
		if( p_iPort > 0 && p_iPort <0xFFFF ){
			m_iListenPort = p_iPort;
		}else
			m_iListenPort = 0;

	}

    void Javascript::uninit() {//不能在v8线程
    }
	void Javascript::initJSEngine()
    {
		v8::Isolate::CreateParams create_params;
		create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
		m_pIsolate = v8::Isolate::New(create_params);
		m_pIsolate->Enter();
		v8::HandleScope handle_scope(m_pIsolate);
		v8::Local<v8::Context> context = v8::Context::New(m_pIsolate);
		m_context.Reset(m_pIsolate, context);
		m_IsolateData = new IsolateData(m_pIsolate, NULL);
		m_IsolateData->m_data = (void*)this;
		//m_pIsolate->SetPromiseRejectCallback(PromiseRejectHandlerInMainThread);
		context->Enter();
    }
    void Javascript::uninitJSEngine()
    {
		{
			v8::HandleScope handle_scope(m_pIsolate);
			v8::Local<v8::Context> context = m_context.Get(m_pIsolate);
			context->Exit();
			m_context.Reset();
			delete m_IsolateData;
			m_pIsolate->Exit();
			
		}
		m_pIsolate->Dispose();

    }
	void Javascript::run(const char* p_pszJS, std::function<void(void)> preRunFunc) {
	}

	void Javascript::run(const char* p_pszJS) {
	}
	
	void Javascript::run(voidfun func, void* pdata) {
		v8::TryCatch try_catch(m_pIsolate);
		//_defRunLoop();
		func(pdata);
		if(try_catch.HasCaught()){
			v8::String::Utf8Value exceptioninfo(m_pIsolate, try_catch.Exception());
			printf("Exception info [%s]\n", *exceptioninfo);
		}
	}

	JSThread::JSThread(){
		setThreadName("JavaScript Main");
	}

	JSThread::~JSThread(){
		//uninitialize();
	}

	void JSThread::_defRunLoop() {
		//设置tls
		//long* old = s_tls_curThread.release();
		//s_tls_curThread.reset(new long((long)this));
		//设置名字
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
		while (!m_bStop) {
			v8::HandleScope handle_scope(v8::Isolate::GetCurrent());
			v8::TryCatch trycatch(v8::Isolate::GetCurrent());
			if (!m_funcLoop) {
				//现在的waitdata返回false不再表示要退出。
				//事件唤醒流程
				//if( !m_ThreadTasks.WaitData(&task) )
				//	break;
				if (m_ThreadTasks.WaitData(&task))
					task();
			}
			else {
				//固定循环流程
				runQueue();
				if (!m_funcLoop()) {
					break;	//
				}
			}
			if (trycatch.HasCaught()) {
				v8::Isolate* piso = v8::Isolate::GetCurrent();
				if( piso )
                    __JSRun::ReportException(piso, &trycatch);
			}
		}

		//退出事件
		JCEventEmitter::evtPtr stopEvt(new JCEventBase);
		stopEvt->m_nID = JCWorkerThread::Event_threadStop;
		emit(stopEvt);
	}

	void call_JSThread__defRunLoop(void* pdata) {
		JSThread* pthis=(JSThread*)pdata;
		pthis->_defRunLoop();
	}

	void JSThread::_runLoop(){
		m_JS.initJSEngine();
		m_JS.run(call_JSThread__defRunLoop,this);
        m_JS.uninitJSEngine();
	}
};
#endif