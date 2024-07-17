/**
@file			JCScriptRuntime.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#include "JCScriptRuntime.h"
#include <algorithm>
#include <util/Log.h>
#include "JSWrapper/JSInterface/JSInterface.h"
#include "JSWrapper/LayaWrap/JSFileReader.h"
#include "JSWrapper/LayaWrap/JSGlobalExportCFun.h"
#include "JSWrapper/LayaWrap/JSInput.h"
#include <downloadCache/JCFileSource.h>
#include <resource/JCFileResManager.h>
#include "Audio/JCAudioManager.h"
#include "JCSystemConfig.h"
#include "JCConch.h"
#include <Performance/JCPerfHUD.h>
#include <WebGLRender/JCWebGLRender.h>
#include "JCCmdDispatchManager.h"
#include <downloadMgr/JCDownloadMgr.h>
#include <inttypes.h>
#include <util/JCCommonMethod.h>
#ifdef JS_V8_DEBUGGER
    #include "JSWrapper/v8debug/debug-agent.h"
#endif
#ifdef JS_JSC
#include "JSWrapper/JSInterface/JSC/EJConvertTypedArray.h"
#endif
extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;

#ifdef ANDROID
#include <android/configuration.h>
#include <dlfcn.h>
// Declaration for native chreographer API.
struct AChoreographer;
typedef void(*AChoreographer_frameCallback)(long frameTimeNanos, void* data);
typedef AChoreographer* (*func_AChoreographer_getInstance)();
typedef void(*func_AChoreographer_postFrameCallback)(
    AChoreographer* choreographer, AChoreographer_frameCallback callback,
    void* data);

// Function pointers for native Choreographer API.
func_AChoreographer_getInstance AChoreographer_getInstance_;
func_AChoreographer_postFrameCallback AChoreographer_postFrameCallback_;

void choreographer_callback(long frameTimeNanos, void* data);
double lastVSYNC = 0.0;
void StartChoreographer() {
    auto choreographer = AChoreographer_getInstance_();
    AChoreographer_postFrameCallback_(choreographer, choreographer_callback, nullptr);
}

void choreographer_callback(long frameTimeNanos, void* data) {
    //LOGE("---TM:"PRIu64",d=%f", frameTimeNanos,(frameTimeNanos- lastVSYNC)/(1e6f));
    double vsynctm = ((unsigned long)frameTimeNanos) / 1e6;
    auto ctm = laya::tmGetCurms();
    //LOGE("---TM:%f,d=%f,cur=%f,d=%f", (float)vsynctm, (float)(vsynctm - lastVSYNC) , ctm,(ctm-vsynctm));
    laya::JCPerfHUD::m_tmVSYNC = vsynctm;
    if (laya::JCScriptRuntime::s_JSRT) {
        laya::JCScriptRuntime::s_JSRT->onVSyncEvent(vsynctm);
    }
    lastVSYNC = vsynctm;
    StartChoreographer();
}

void initChoreographer() {
    return;//不用了，全部用java更方便一些。
    //>=24
    void* lib = dlopen("libandroid.so", RTLD_NOW | RTLD_LOCAL);
    if (lib != nullptr) {
        LOGE("Run with Choreographer Native API.");
        //api_mode_ = kAPINativeChoreographer;

        // Retrieve function pointers from shared object.
        AChoreographer_getInstance_ = reinterpret_cast<func_AChoreographer_getInstance>( dlsym(lib, "AChoreographer_getInstance"));
        AChoreographer_postFrameCallback_ = reinterpret_cast<func_AChoreographer_postFrameCallback>(dlsym(lib, "AChoreographer_postFrameCallback"));
        //assert(AChoreographer_getInstance_);
        //assert(AChoreographer_postFrameCallback_);

        //开始
        if(AChoreographer_getInstance_)
            StartChoreographer();
    }
}
#endif

namespace laya {
    JCScriptRuntime* JCScriptRuntime::s_JSRT = NULL;
    JCScriptRuntime::JCScriptRuntime() 
    {
#ifdef ANDROID
        initChoreographer();
#endif
        m_nDispathGlobalID = PUBLIC_CMD_DISPATH;
        m_pFreeTypeRender = NULL;
        s_JSRT = this;
        m_pPoster = NULL;
        m_bHasJSThread = false;
        m_nLastUpdateTm = 0;
        m_pFileResMgr = NULL;
        m_pAssetsRes = NULL;
        m_pRenderCmd = new JCMemClass(MEM_RENDER_COMMAND_SIZE, MEM_RENDER_COMMAND_SIZE);
        m_pRenderCmd->setAlign(true);
        m_pGCCmd = new JCMemClass(1024, 1024);
        m_pGCCmd->setAlign(true);
        m_bIsExit = false;
        m_pFontManager = new JCFontManager();
        m_pMeasureTextManager = new JCMeasureTextManager();
        m_pUrl = new JCUrl();
        m_pMarket = NULL;
        m_nThreadState = 0;
        m_nCountGroup = 0;
        m_nCountVertex = 0;
        m_nCountNode = 0;
#ifdef ANDROID
        m_pDbgAgent = nullptr;
        m_pCurEditBox=NULL;
#elif __APPLE__
        m_pCurEditBox=NULL;
#elif OHOS
        m_pCurEditBox=NULL;
#endif
        JCAudioManager::GetInstance();
        //事件现在有问题，只能添加不能删除。所以不要调用多次。
        m_ScriptThread.on(JCWorkerThread::Event_threadStart, std::bind(&JCScriptRuntime::onThInit, this, std::placeholders::_1));
        m_ScriptThread.on(JCWorkerThread::Event_threadStop, std::bind(&JCScriptRuntime::onThExit, this, std::placeholders::_1));
        
        /*if (!isSupportTypedArrayAPI())
        {
            m_nCmdBufferSharedWithJSLen = 20 * 1024;
            m_pCmdBufferSharedWithJS = new char[m_nCmdBufferSharedWithJSLen];
            
            m_nOtherBufferSharedWidthJSLen = 40;
            m_pOtherBufferSharedWidthJS = new char[m_nOtherBufferSharedWidthJSLen];
        }*/
    }
    JCScriptRuntime::~JCScriptRuntime() {
        //在这时候js可能还在执行。需要先停止才能释放其运行环境。
        m_ScriptThread.stop();
        m_pFreeTypeRender = NULL;
        s_JSRT = NULL;
        m_pFileResMgr = NULL;
        m_pAssetsRes = NULL;
        if (m_pRenderCmd)
        {
            delete m_pRenderCmd;
            m_pRenderCmd = NULL;
        }
        if (m_pGCCmd)
        {
            delete m_pGCCmd;
            m_pGCCmd = NULL;
        }
        if (m_pUrl)
        {
            delete m_pUrl;
            m_pUrl = NULL;
        }
        if (m_pFontManager)
        {
            delete m_pFontManager;
            m_pFontManager = NULL;
        }
        if( m_pMeasureTextManager ){
            delete m_pMeasureTextManager;
            m_pMeasureTextManager = nullptr;
        }
        /*if (!isSupportTypedArrayAPI())
        {
            delete [] m_pCmdBufferSharedWithJS;
        }*/
    }

    void  JCScriptRuntime::init(JCFileResManager* pFileMgr, JCFileSource* pAssetRes, IConchThreadCmdMgr* pThreadCmdSender,JCFreeTypeFontRender* pFreeTypeRender ) 
    {
        m_pFreeTypeRender = pFreeTypeRender;
        m_pMeasureTextManager->setFreeTypeFontRender(m_pFreeTypeRender);
        m_pFileResMgr = pFileMgr;
        m_pAssetsRes = pAssetRes;
        m_pPoster = pThreadCmdSender;
    }

    void JCScriptRuntime::start(const char* pStartJS) {
        LOGI("Start js %s", pStartJS);
        if (pStartJS)
            m_strStartJS = pStartJS;
        m_ScriptThread.initialize(JCConch::s_nDebugPort);
#ifdef __APPLE__
        m_ScriptThread.setLoopFunc(std::bind(&JCScriptRuntime::onUpdate, this));
#endif
        m_nThreadState = 1;
        m_ScriptThread.start();
    }
    void JCScriptRuntime::stop() {
        LOGI("Stop js start...");
        while (m_nThreadState==1) {
            LOGI("stop: wait for thread to start...");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#ifdef __APPLE__
        if( JCConch::s_pConchRender)
        {
            while (JCConch::s_pConchRender->m_kRenderSem.getDataNum() == 1)
            {
                JCConch::s_pConchRender->renderFrame(0, false);
            }
        }
#endif
        m_ScriptThread.stop();
        m_ScriptThread.uninitialize();
        LOGI("Stop js end.");
    }

    void JCScriptRuntime::reload() {
#ifdef __APPLE__
        if(JCConch::s_pConchRender)
        {
            JCConch::s_pConchRender->m_bStopRender = true;
        }
#endif
         stop();
        //为了避免上个js的下载的影响，去掉下载任务
        //这个必须在stop之后做，且保证stop确实会停止线程，包括还没有启动的线程。
        //如果线程还没有启动，stop会等待线程先启动。这样就可能会执行脚本，所以，清理必须放在stop之后。
        JCDownloadMgr* pNetLoader = JCDownloadMgr::getInstance();
        pNetLoader->stopCurTask();
        pNetLoader->clearAllAsyncTask();
        pNetLoader->resetDownloadTail();	//防止第二次进入的时候，下载错误的dcc等
        pNetLoader->resetFinalReplacePath();
        pNetLoader->resetDownloadReplaceExt();
        //文件资源不能跨js环境使用，所以必须clear
        // 例如一个资源正在下载，则可能的问题：1.可能会上个线程取消了，不会再回调， 2. 自己希望回调的是上个js环境，也无法传给新的js环境。
        // 所以需要clear。
        m_pFileResMgr->clear();
        start(m_strStartJS.c_str());
    }
    void JCScriptRuntime::onThInit(JCEventEmitter::evtPtr evt) 
    {
        LOGI("js thread started.");
        m_nThreadState = 2;
        JCPerfHUD::resetFrame();
#ifdef JS_V8
        JSObjNode::s_pListJSObj = new JCSimpList();
        #ifdef JS_V8_DEBUGGER
        if (m_pDbgAgent) 
        {
            m_pDbgAgent->onJSStart(&m_ScriptThread);
        }
        #endif
#endif
        JCConch::s_pConchRender->m_pImageManager->resetJSThread();
        m_nDispathGlobalID = PUBLIC_CMD_DISPATH;
        //给渲染线程发开始指令
        m_pRenderCmd->clearData();
        m_pRenderCmd->append(PUBLIC_CMD_DISPATH);
        m_pRenderCmd->append(PFT_startJSThread);
        m_pGCCmd->clearData();

        auto curtm = tmGetCurms();
        m_nLastUpdateTm = curtm;

        //DEBUG
        /*
        m_pEJAnimResMgr->chkThread(true);
        m_pEJAnimResMgr->InitWorkThread();
        m_pAtlasMgr->chkThread(true);
        m_pAtlasMgr->InitWorkThread();
        m_ThumbMgr.chkThread(true);
        m_ThumbMgr.InitWorkThread();
        */
        m_pMarket = new JSMarket();
        JsFile::RegisterToJS();
        JsFileReader::RegisterToJS();
        JSGlobalExportC();
        //设置js的一些环境。必须在所有导出之后，执行其他脚本之前。
#ifndef WIN32
        //JSP_RUN_SCRIPT((const char*)"function getExePath(){return null;}");
#endif
        {
            char* sJSRuntime = NULL;
            int nSize = 0;
            if (m_pAssetsRes->loadFileContent("scripts/runtimeInit.js", sJSRuntime, nSize))
            {
                JSP_RUN_SCRIPT(sJSRuntime);
                delete[] sJSRuntime;
            }
        }
        char* sJCBuffer = NULL;
        int nJSSize = 0;
        if (m_pAssetsRes->loadFileContent(m_strStartJS.c_str(), sJCBuffer, nJSSize))
        {
            std::string kBuf = "(function(window){\n'use strict'\n";
            kBuf += sJCBuffer;
            kBuf += "\n})(window);\n//@ sourceURL=apploader.js";
            #ifdef JS_V8
                v8::Isolate* isolate = v8::Isolate::GetCurrent();
                v8::HandleScope handle_scope(isolate);
                v8::TryCatch try_catch(isolate);
                JSP_RUN_SCRIPT(kBuf.c_str());
                if (try_catch.HasCaught())
                {
                    __JSRun::ReportException(isolate, &try_catch);
                }
            #else
                JSP_RUN_SCRIPT(kBuf.c_str());
            #endif	
            delete[] sJCBuffer;
            sJCBuffer = NULL;
        }
#ifdef __APPLE__
        if (JCConch::s_pConchRender)
        {
            JCConch::s_pConchRender->m_bStopRender = false;
        }
#else
        m_ScriptThread.post(
            std::bind(&JCScriptRuntime::onUpdate, this)
            );
#endif
    }

    bool JCScriptRuntime::onUpdate() 
    {
        m_nUpdateCount++;
        bool bRunOnDraw = false;
        auto nTime = tmGetCurms();
#ifdef ANDROID1
        if (m_bRunDraw) {
            m_bRunDraw = false;
            bRunOnDraw = true;
            m_dbLastUsedVsync = m_dbCurVsync;
            onUpdateDraw(m_dbCurVsync);
        }
#else
        if (!g_kSystemConfig.m_bUseChoreographer) {
            //if (nTime - m_dLastDrawTm > (1000.0 / 60.0)) {
                JCPerfHUD::m_tmVSYNC = nTime;
                //m_dLastDrawTm = nTime;
                //onUpdateDraw(nTime);
            //}
        }
#endif

        onUpdateDraw(JCPerfHUD::m_tmVSYNC);
        m_nLastUpdateTm = nTime;
        
        PERF_INITVAR(nBenginTime);
        JSInput* pInput = JSInput::getInstance();
        if ( pInput->m_bTouchMode )
        {
            pInput->swapCurrentTouchEvent();
            if( pInput->m_vInputEventsJS.size() > 0 )
            {
                pInput->m_nTouchFrame = 120;
                for (int i = 0, nSize = (int)pInput->m_vInputEventsJS.size(); i < nSize; i++ )
                {
                    TouchEventInfo* touchEvent = &pInput->m_vInputEventsJS[i];
                    m_pJSTouchEvtFunction.Call(touchEvent->nType, touchEvent->nID,"type",touchEvent->x, touchEvent->y);
                }
            }
            if( pInput->m_nTouchFrame > 0 )
            {
                pInput->m_nTouchFrame--;
            }
        }
        if (g_bGLCanvasSizeChanged)
        {
            m_pJSOnResizeFunction.Call(g_nInnerWidth, g_nInnerHeight);
            //m_pRootCanvas->size( g_nInnerWidth,g_nInnerHeight );
            g_bGLCanvasSizeChanged = false;
        }
        int nUpdateNum = m_nUpdateCount % 3;
        switch (nUpdateNum)
        {
        case 0:
            JCAudioManager::GetInstance()->update();
            break;
        case 1:
            JCAudioManager::GetInstance()->m_pWavPlayer->autoGarbageCollection();
            break;
        case 2:
            //TODO 如果有需要清理的或者update可以放到这
            break;
        }
        JS_TRY;
        m_pJSOnFrameFunction.Call();
        JS_CATCH;
        if (!bRunOnDraw) {
            //std::this_thread::sleep_for(std::chrono::milliseconds(4));
        }
        return true;
    }

    void JCScriptRuntime::onUpdateDraw(double vsyncTime) 
    {
        m_bHasPostVsync = false;
        if (!m_pJSOnDrawFunction.Empty()) 
        {
            PERF_INITVAR(nBenginTime);
            JS_TRY;
            m_pJSOnDrawFunction.Call(vsyncTime);
            JS_CATCH;
            auto dt = (float)(tmGetCurms() - nBenginTime);
            PERF_UPDATE_DATA(JCPerfHUD::PHUD_JS_DELAY, dt);
            runConchCmd();
        }
    }
    void JCScriptRuntime::runConchCmd()
    {
        flushSharedCmdBuffer();
        if (JCConch::s_pConchRender != NULL)
        {
            if (m_pGCCmd->getDataSize() > 0)
            {
                m_pRenderCmd->append(m_pGCCmd->getBuffer(), m_pGCCmd->getDataSize());
                m_pGCCmd->clearData();
            }
            JCConch::s_pConchRender->setRenderData(m_pRenderCmd, m_nCountGroup, m_nCountVertex, m_nCountNode);
        }
    }
    void JCScriptRuntime::onVSyncEvent(double vsyncTime) {
        JCPerfHUD::m_tmVSYNC = vsyncTime;
        m_dbCurVsync = vsyncTime;
        //m_bRunDraw = true;
        //这个事件的速度始终固定，但是js可能会非常卡，导致大量积累，所以要保护 一下。
        if (!m_bHasPostVsync) {
            m_bHasPostVsync = true;
            m_ScriptThread.post(std::bind(&JCScriptRuntime::onUpdate, this));
        }
    }

    void JCScriptRuntime::onThExit(JCEventEmitter::evtPtr evt) {
        LOGI("js thread exiting...");
        m_nThreadState = 0;
        m_pJSOnFrameFunction.Reset();
        m_pJSOnResizeFunction.Reset();
        m_pJSMouseEvtFunction.Reset();
        m_pJSKeyEvtFunction.Reset();
        m_pJSTouchEvtFunction.Reset();
        m_pJSDeviceMotionEvtFunction.Reset();
        m_pJSNetworkEvtFunction.Reset();
		m_pJSOnBackPressedFunction.Reset();
        m_pJSOnceOtherEvtFuction.Reset();
        m_pJSOnInvalidGLRes.Reset();
        m_pJSOnDrawFunction.Reset();
        m_pJSCmdABObjSharedWithJS.Reset();
        m_pJSCmdOtherBufferSharedWithJS.Reset();
#ifdef ANDROID
        m_pCurEditBox = NULL;
#endif
        JSClassMgr::GetInstance()->resetAllRegClass();
#ifdef JS_V8
        JCSimpList* pNodeLists = JSObjNode::s_pListJSObj;
        if (pNodeLists != NULL)
        {
            JCListNode* pCur = pNodeLists->begin();
            JCListNode* pEnd = pNodeLists->end();
            while (pCur != pEnd)
            {
                JSObjNode* pJsCur = (JSObjNode*)pCur;
                pCur = pNodeLists->delNode(pCur);
                delete pJsCur;
            }
            delete JSObjNode::s_pListJSObj;
            JSObjNode::s_pListJSObj = nullptr;
        }
        #ifdef JS_V8_DEBUGGER
        if (m_pDbgAgent) {
            m_pDbgAgent->onJSExit();
        }
        #endif
#elif JS_JSC
        JSP_RESET_GLOBAL_FUNCTION;
#endif
        m_pMeasureTextManager->clearAll();
        JCAudioManager::ClearAllWork();
        JCAudioManager::GetInstance()->stopMp3();
        JCAudioManager::GetInstance()->pauseMp3();
    }

    void JCScriptRuntime::dispatchInputEvent(inputEvent e)
    {
        JSInput::getInstance()->activeCall(e);
    }

    void JCScriptRuntime::dispatchInputEvent(DeviceOrientationEvent e)
    {
        JSInput::getInstance()->activeCall(e);
    }

    void JCScriptRuntime::dispatchInputEvent(DeviceMotionEvent e)
    {
        JSInput::getInstance()->activeCall(e);
    }
    void JCScriptRuntime::setCmdBuffer(JSValueAsParam pArrayBuffer) {
        char* pABPtr = NULL;
        int nABLen = 0;
        bool bisab = extractJSAB(pArrayBuffer, pABPtr, nABLen);

        if (bisab)
        {
             m_pCmdBufferSharedWithJS = pABPtr;
             m_nCmdBufferSharedWithJSLen = nABLen;
        }
        else {
            LOGE("JCScriptRuntime::setCmdBuffer param is not an ArrayBuffer!");
        }
    }
    void JCScriptRuntime::setBuffer(JSValueAsParam pArrayBuffer) {
        char* pABPtr = NULL;
        int nABLen = 0;
        bool bisab = extractJSAB(pArrayBuffer, pABPtr, nABLen);

        if (bisab)
        {
            m_pOtherBufferSharedWidthJS = pABPtr;
        }
        else 
        {
            LOGE("JCScriptRuntime::setBuffer param is not an ArrayBuffer!");
        }
    }
    void JCScriptRuntime::flushSharedCmdBuffer() {
/*#ifdef JS_JSC
        if (!isSupportTypedArrayAPI())
            setCmdBuffer(m_pJSCmdABObjSharedWithJS.m_pValue);
#endif*/
        char* pBuff = m_pCmdBufferSharedWithJS;
        if (!pBuff)
            return;
        int len = *(int*)pBuff - 4;
        if (len > 0){
            m_pRenderCmd->append(pBuff + 4, len);
            /*if (!isSupportTypedArrayAPI())
            {
                JSP_RUN_SCRIPT("conch.bf.initPos(4);");
            }
            else*/
            {
                ((int*)pBuff)[0] = 4;
            }
        }
    }
    void JCScriptRuntime::onInvalidGLRes()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::onInvalidGLResCallJSFunction, this);
        m_ScriptThread.post(pFunction);
    }
    void JCScriptRuntime::onInvalidGLResCallJSFunction()
    {
        if (m_bIsExit || m_pJSOnInvalidGLRes.Empty())
            return;
        m_pJSOnInvalidGLRes.Call();
    }
    void JCScriptRuntime::onNetworkChanged(int nType)
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::onNetworkChangedCallJSFunction, this, nType);
        m_ScriptThread.post(pFunction);
    }
    void JCScriptRuntime::onNetworkChangedCallJSFunction(int nType)
    {
        m_pJSNetworkEvtFunction.Call(nType);
    }
    void JCScriptRuntime::jsGC()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsGCCallJSFunction, this);
        m_ScriptThread.post(pFunction);
    }
    void JCScriptRuntime::jsGCCallJSFunction()
    {
        JSP_RUN_SCRIPT("gc()");
    }
    void JCScriptRuntime::callJC(std::string sFunctionName, std::string sJsonParam, std::string sCallbackFunction)
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::callJSFuncton, this, sFunctionName, sJsonParam, sCallbackFunction );
        m_ScriptThread.post(pFunction);
    }
    void JCScriptRuntime::callJSString( std::string sBuffer )
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::callJSStringFunction, this,sBuffer);
        m_ScriptThread.post(pFunction);
    }
    void JCScriptRuntime::callJSStringFunction( std::string sBuffer )
    {
        JSP_RUN_SCRIPT(sBuffer.c_str());
    }
    void JCScriptRuntime::callJSFuncton(std::string sFunctionName, std::string sJsonParam, std::string sCallbackFunction)
    {
        std::string sBuffer = sFunctionName;
        sBuffer += "(\"";
        sBuffer += sJsonParam;
        sBuffer += "\",\"";
        sBuffer += sCallbackFunction;
        sBuffer += "\");";
        LOGI("JCScriptRuntime::callJSFuncton buffer=%s",sBuffer.c_str() );
        JSP_RUN_SCRIPT( sBuffer.c_str() );
    }
    void JCScriptRuntime::restoreAudio()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsRestoreAudioFunction, this);
        m_ScriptThread.post(pFunction);
    }
    void JCScriptRuntime::jsRestoreAudioFunction()
    {
        if(JCAudioManager::GetInstance()->getMp3Mute() == false && JCAudioManager::GetInstance()->getMp3Stopped() == false)
        {
            JCAudioManager::GetInstance()->resumeMp3();
        }
    }
    void JCScriptRuntime::jsReloadUrl()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsReloadUrlJSFunction, this);
        m_ScriptThread.post(pFunction);
    }
    void JCScriptRuntime::jsReloadUrlJSFunction()
    {
        JSP_RUN_SCRIPT("reloadJS(true)");
    }
    void JCScriptRuntime::jsUrlback()
    {
        std::function<void(void)> pFunction = std::bind(&JCScriptRuntime::jsUrlbackJSFunction, this);
        m_ScriptThread.post(pFunction);
    }
    void JCScriptRuntime::jsUrlbackJSFunction()
    {
        JSP_RUN_SCRIPT("history.back()");
    }
    int JCScriptRuntime::getDispathGlobalID()
    {
        return ++m_nDispathGlobalID;
    }
	bool JCScriptRuntime::onBackPressed()
	{
		std::lock_guard<std::mutex> lock(m_OnBackPressedMutex);
		if (!m_bJSOnBackPressedFunctionSet){
			return false;
		}
        if (JCScriptRuntime::s_JSRT->m_pPoster)
			JCScriptRuntime::s_JSRT->m_pPoster->postToJS([this](){
					 this->m_pJSOnBackPressedFunction.Call();
			});
		return true;
	}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
