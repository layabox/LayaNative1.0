#include "JSCEnv.h"
#include "../../../CToObjectC.h"
#ifdef __APPLE__
#include <pthread.h>
#endif
#include "EJConvertTypedArray.h"
#include "JSCArrayBuffer.h"
/*
//TODO
这个类还有问题，还没有经过严格测试
程序进行reload还肯定存在问题，包括postMainThread等方法
 */

namespace laya
{
    extern bool isSupportTypedArrayAPI();
    pthread_key_t __TlsData::s_tls_curThread;
    
	static JSValueRef gcCallback( JSContextRef ctx, JSObjectRef functionObject, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception )
    {
        JSGlobalContextRef pCtx = JSContextGetGlobalContext(ctx);
        JSGarbageCollect( pCtx );
        //JSSynchronousEdenCollectForDebugging(pCtx);
        //JSSynchronousGarbageCollectForDebugging(pCtx);
        return JSValueMakeUndefined(ctx);
    }
    void AlertOnJsException(bool b){
        
    }

	JSThread::JSThread()
	{
        setThreadName("JavaScript Main");
	}
	JSThread::~JSThread()
	{

	}
	void JSThread::initialize( int p_nPort )
	{
		
	}
	void JSThread::uninitialize()
	{

	}
    void JSThread::stop()
    {
        if(m_bStop)return;
        m_bStop = true;
        m_ThreadTasks.Stop();
        m_bdbgThreadStarted=false;
    }
    void JSThread::post( runObj func )
    {
        if (m_bStop)
            return;
        /*
        if( !m_funcLoop){
            m_ThreadTasks.SendData(func);
        }else{
            m_QueueLock.lock();
            m_FuncQueue.push_back(func);
            m_QueueLock.unlock();
        }
        */
        CToObjectCPostFunc(func);
    }
    void JSThread::_defRunLoop()
    {
        //设置tls
        pthread_setspecific(s_tls_curThread,(void*)this);

        //设置名字
#ifdef WIN32
        {
            DWORD thid = GetCurrentThreadId();
            SetNameInternal(thid, m_strName.c_str());
            threadInfoLog("start thread:%s,%d", m_strName.c_str(), thid);
        }
#elif ANDROID
        {
            threadInfoLog("start thread:%s,%ld", m_strName.c_str(), gettidv1());
        }
#endif
        //开始事件
        JCEventEmitter::evtPtr startEvt(new JCEventBase);
        startEvt->m_nID=JCWorkerThread::Event_threadStart;
        emit(startEvt);
        
        CToObjectCRunJSLoop();
        /*
        runObj task;
        while (!m_bStop) {
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
        }
        */
        
        //退出事件
        JCEventEmitter::evtPtr stopEvt(new JCEventBase);
        stopEvt->m_nID=JCWorkerThread::Event_threadStop;
        emit(stopEvt);
    }
	void JSThread::_runLoop()
	{
		JSGlobalContextRef jsGlobalContext = JSGlobalContextCreateInGroup( NULL, NULL );
        JSValueRef jsUndefined = JSValueMakeUndefined(jsGlobalContext);
        JSValueProtect(jsGlobalContext, jsUndefined);
        __TlsData::GetInstance()->SetCurContext( jsGlobalContext );
        
        if (!isSupportTypedArrayAPI())
        {
            JSContextPrepareTypedArrayAPI(jsGlobalContext);
        }
        
        // expose gc()
        JSStringRef pjsName = JSStringCreateWithUTF8CString( "gc" );
        JSObjectRef pFunc = JSObjectMakeFunctionWithCallback(jsGlobalContext, pjsName, gcCallback);
        JSValueRef exception = 0;
        JSObjectSetProperty(jsGlobalContext, JSContextGetGlobalObject(jsGlobalContext), pjsName, pFunc, kJSPropertyAttributeDontDelete, &exception);
        if( 0 != exception )
        {
            __JSRun::OutputException( exception );
        }
        JSStringRelease(pjsName);

		//runLoop
		_defRunLoop();

		//release
        __TlsData::GetInstance()->SetCurContext(0);
        JSValueUnprotect(jsGlobalContext, jsUndefined);
        JSGlobalContextRelease(jsGlobalContext);
	}
};
