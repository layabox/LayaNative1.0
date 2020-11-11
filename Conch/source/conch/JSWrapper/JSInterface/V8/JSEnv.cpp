#ifdef JS_V8

#include "JSEnv.h"
#include <v8-profiler.h>
#include "JSArrayBuffer.h"
#include "../../v8debug/debug-agent.h"
#include "util/Log.h"
#include "JSCProxyTLS.h"
#ifdef WIN32
#include <windows.h>
#include <process.h>
#endif
//#define V8PROFILE

namespace laya{
	
	v8::Persistent<v8::Context> Javascript::m_DebugMessageContext;

	const char* ToCString(const v8::String::Utf8Value& value){
		return *value ? *value : "<string conversion failed>";
	}
   
	Javascript::Javascript() {
		m_pIsolate = NULL;
		m_pPlatform = NULL;
		//m_JsDbgAgentThread = NULL;
		//m_v8dbgAgent = NULL;
	}

	void Javascript::init(int p_iPort) {
		v8::V8::InitializeICU();
		m_pPlatform = v8::platform::CreateDefaultPlatform();
		v8::V8::InitializePlatform(m_pPlatform);
		v8::V8::Initialize();
		//v8::V8::InitializeExternalStartupData("E:\\conch4\\trunk\\Redist\\conch.exe");
		if( p_iPort > 0 && p_iPort <0xFFFF ){
			m_iListenPort = p_iPort;
		}else
			m_iListenPort = 0;
	}

    void Javascript::uninit() {//不能在v8线程
        if (0 != m_pIsolate) {
            m_pIsolate->Dispose();
            m_pIsolate = 0;
            ArrayBufferAllocator* pABAlloc = ArrayBufferAllocator::getInstance();
            //pABAlloc->FreeAllAlive();
            delete pABAlloc;
        }
    }

	void Javascript::run(const char* p_pszJS, std::function<void(void)> preRunFunc) {
	}

	void Javascript::run(const char* p_pszJS) {
	}
	
	void Javascript::run(voidfun func, void* pdata) {
		v8::Isolate::CreateParams create_params;
        ArrayBufferAllocator* pABAlloc = ArrayBufferAllocator::getInstance();
		create_params.array_buffer_allocator = pABAlloc;
		m_pIsolate = v8::Isolate::New(create_params);
		v8::V8::SetFlagsFromString("--expose_gc",11);
		v8::V8::SetFlagsFromString("--allow-natives-syntax", 22);	//导出内部的%函数
		//v8::V8::SetFlagsFromString("--trace-gc-verbose",18);

#ifdef V8PROFILE
		v8::V8::SetFlagsFromString("--prof",6);
		char* pSingleLogFile = "--no-logfile_per_isolate";
		v8::V8::SetFlagsFromString(pSingleLogFile,strlen(pSingleLogFile));
#ifdef ANDROID
		char* profFile = "--logfile /sdcard/lr2perf/v8.log";
#elif WIN32
		char* profFile = "--logfile d:/temp/v8.log";
#endif
		v8::V8::SetFlagsFromString( profFile, strlen(profFile));
#endif
		v8::Locker* pLocker = NULL;
		if(m_iListenPort>0)
			pLocker = new v8::Locker(m_pIsolate);//这个是debug需要的。调用这个后，isolate就所在线程会用锁来同步。

		{//这个{}是为了能让 Scope起作用
			v8::Isolate::Scope _isolate(m_pIsolate);
			v8::HandleScope _scope(m_pIsolate);

			v8::Handle<v8::ObjectTemplate> pGlobalTemplate = v8::ObjectTemplate::New();

			v8::Local<v8::Context> context = v8::Context::New(m_pIsolate,
																NULL,
																pGlobalTemplate,
																v8::Handle<v8::Value>());
			context->Enter();

			//v8::CpuProfiler* pCpuProf =  m_pIsolate->GetCpuProfiler();
			//pCpuProf->StartCpuProfiling(v8::String::New(""));
			//pCpuProf->StopCpuProfiling(v8::String::New(""));
			if(m_iListenPort>0 ){
				{
                    std::unique_lock<std::recursive_mutex> __guard(m_Lock);
					if( m_DebugMessageContext.IsEmpty() ){
						m_DebugMessageContext.Reset(m_pIsolate, v8::Local<v8::Context>::New(m_pIsolate, context) );
					}
				}
				//m_v8dbgAgent = new DebuggerAgent("layabox", m_iListenPort);
				//v8::Debug::SetMessageHandler(DebuggerAgentMessageHandler);
			}
			v8::TryCatch try_catch;
			//_defRunLoop();
			func(pdata);
			if(try_catch.HasCaught()){
				v8::String::Utf8Value exceptioninfo(try_catch.Exception());
				printf("Exception info [%s]\n", *exceptioninfo);
			}

			//当js线程要结束的时候，关闭调试，否则调试器客户端在线程结束后再发送调试事件，会导致非法。
			if(m_iListenPort>0){
				//v8::Debug::DisableAgent();
			}
			if(!m_DebugMessageContext.IsEmpty()){
				m_DebugMessageContext.Reset();
			}

			//v8::Local<v8::Context>::New(m_pIsolate, context)->Exit();
            context->Exit();
        }
        //{
        //    v8::Isolate::Scope _isolate(m_pIsolate);
        //    v8::HandleScope _scope(m_pIsolate);

        //    v8::Handle<v8::ObjectTemplate> pGlobalTemplate = v8::ObjectTemplate::New();
        //    auto context1 = v8::Context::New(m_pIsolate);
        //    auto context2 = v8::Context::New(m_pIsolate,nullptr,pGlobalTemplate);
        //}
        //while (!m_pIsolate->IdleNotificationDeadline(m_pPlatform->MonotonicallyIncreasingTime() + 1.0)) {
        //    int a = 0;
        //}
        //m_pIsolate->LowMemoryNotification();
        //m_pIsolate->ContextDisposedNotification();

		if(pLocker)
			delete pLocker;
		//m_pIsolate->Dispose();
		//v8::V8::Dispose();
		v8::V8::ShutdownPlatform();
		delete m_pPlatform;
		m_pPlatform = NULL;
		//调试相关
		//if (m_v8dbgAgent) {
		//	m_v8dbgAgent->Shutdown();
		//	delete m_v8dbgAgent;
		//	m_v8dbgAgent = NULL;
		//}
        //pABAlloc->FreeAllAlive();
        //delete pABAlloc;
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
			v8::TryCatch trycatch;
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
		m_JS.run(call_JSThread__defRunLoop,this);
	}
};
#endif