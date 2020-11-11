/**
@file			JSConchConfig.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_18
*/

//包含头文件
#include "JSConchConfig.h"
#include "JSInput.h"
#include "resource/JCFileResManager.h"
#include "../../JCScrpitRuntime.h"
#include "../../../Conch/source/conch/JCConch.h"
#ifdef ANDROID
    #include "../../CToJavaBridge.h"
#elif WIN32
    #include <Windows.h>
#elif __APPLE__
    #include "../../CToObjectC.h"
#endif
#include "downloadMgr/JCDownloadMgr.h"
#include "util/Log.h"
#include "../../JCSystemConfig.h"
#include "../../JCConchRender.h"
#include "../../JCConch.h"
#include "../../WebSocket/WebSocket.h"
#include <resource/Audio/JCAudioWavPlayer.h>
#include "../../Audio/JCAudioManager.h"
#include "JSGraphics.h"
#include <3DRS/JCRenderer.h>
extern int g_nDebugLevel;
extern int g_nInnerWidth;
extern int g_nInnerHeight;
extern bool g_bGLCanvasSizeChanged;

#ifdef WIN32
extern HWND g_hWnd;
#endif

namespace laya
{

    ADDJSCLSINFO(JSConchConfig, JSObjNode);
    JSConchConfig* JSConchConfig::ms_pInstance = NULL;

    JSConchConfig* JSConchConfig::getInstance()
    {
        if (ms_pInstance == NULL)
        {
            ms_pInstance = new JSConchConfig();
        }
        return ms_pInstance;
    }

    JSConchConfig::JSConchConfig()
    {
        m_sGUID = "unknow";
        m_sDeviceModel = "unknow";
        m_sDeviceInfo = "{\"resolution\":\"unknow\",	\"guid\":\"unknow\",\"imei\":[\"unknow\"],\"imsi\":[\"unknow\"],\"os\":\"unknow\",\"osversion\":\"unknow\",\"phonemodel\":\"unknow\"	}";
        //AdjustAmountOfExternalAllocatedMemory( 128 ); 可能还没有js环境
        JCMemorySurvey::GetInstance()->newClass("conchConfig", 128, this);
    }

    JSConchConfig::~JSConchConfig()
    {
        ms_pInstance = NULL;
        JCMemorySurvey::GetInstance()->releaseClass("conchConfig", this);
    }

    const char* JSConchConfig::getLocalStoragePath()
    {
        if (JCConch::s_pConch)
            return JCConch::s_pConch->getLocalStoragePath();
        return "";
    }
    float JSConchConfig::getTotalMem()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getTotalMem", kRet, CToJavaBridge::JavaRet::RT_Float))
        {
            return kRet.floatRet;
        }
        return 0;
#elif WIN32
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);
        return (float)(statex.ullTotalPhys / 1024);
#elif __APPLE__
        return CToObjectCGetTotalMem();
#endif
        return 0;
    }
    int JSConchConfig::getUsedMem()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getUsedMem", kRet, CToJavaBridge::JavaRet::RT_Float))
        {
            return (int)(kRet.floatRet);
        }
        return 0;
#elif __APPLE__
        return CToObjectCGetUsedMem();
#elif WIN32
        return getAppUsedMem();
#endif
        return 0;
    }
    int JSConchConfig::getAvalidMem()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getAvalidMem", kRet, CToJavaBridge::JavaRet::RT_Float))
        {
            return (int)(kRet.floatRet);
        }
        return 0;
#elif WIN32
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        GlobalMemoryStatusEx(&statex);
        return (int)(statex.ullAvailPhys / 1024);
#elif __APPLE__
        return CToObjectCGetAvalidMem();
#endif
        return 0;
    }
    float JSConchConfig::getScreenInch()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod("layaair.game.utility.ProcessInfo", "getScreenInch", kRet, CToJavaBridge::JavaRet::RT_Float))
        {
            return kRet.floatRet;
        }
        return 0;
#elif WIN32
        return 0;
#elif __APPLE__
        return CToObjectCGetScreenInch();
#endif
        return 0;
    }
    void JSConchConfig::setTouchMoveRange(float p_fMM)
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setTouchMoveRange", p_fMM,kRet);
#elif WIN32

#endif
    }

    void JSConchConfig::setScreenOrientation(int p_nOrientation)
    {
        enum ori {
            landscape = 0,
            portrait = 1,
            user = 2,
            behind = 3,
            sensor = 4,
            nosensor = 5,
            sensor_landscape = 6,
            sensor_portrait = 7,
            reverse_landscape = 8,
            reverse_portrait = 9,
            full_sensor = 10,
            num
        };
        bool vbLandscapes[] = { true,false,false,false,false,false,true,false,true,false,false };
        if (p_nOrientation >= num) {
            return;
        }
        int w = g_nInnerWidth > g_nInnerHeight ? g_nInnerWidth : g_nInnerHeight;
        int h = g_nInnerWidth > g_nInnerHeight ? g_nInnerHeight : g_nInnerWidth;
        if (vbLandscapes[p_nOrientation]) {
            g_nInnerWidth = w;
            g_nInnerHeight = h;
        }
        else {
            g_nInnerWidth = h;
            g_nInnerHeight = w;
        }
        g_bGLCanvasSizeChanged = true;
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setScreenOrientation", p_nOrientation, kRet);
#elif WIN32
        MoveWindow(g_hWnd, 0, 0, g_nInnerWidth, g_nInnerHeight, true);
#elif __APPLE__
        CToObjectCSetScreenOrientation(p_nOrientation);
#endif

    }

    void JSConchConfig::setUrlIgnoreCase(bool b) {
        JCScriptRuntime::s_JSRT->m_pFileResMgr->m_bUrlToLowerCase = b;
    }

    bool JSConchConfig::getUrlIgnoreCase() {
        return JCScriptRuntime::s_JSRT->m_pFileResMgr->m_bUrlToLowerCase;
    }

    int JSConchConfig::getNetworkType()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getContextedType", kRet, CToJavaBridge::JavaRet::RT_Int))
        {
            return kRet.intRet;
        }
        return 1;
#elif WIN32
        return 1;
#elif __APPLE__
        return CToObjectCGetNetworkType();
#endif
        return 0;
    }

    void JSConchConfig::setDownloadTryNum(int p_nOptTry, int p_nConnTry) {
        JCDownloadMgr* pDmgr = JCDownloadMgr::getInstance();
        if (pDmgr) {
            pDmgr->setOpt_tryNumOnTimeout(p_nOptTry, p_nConnTry);
        }
    }

    void JSConchConfig::setDownloadPathReplace(const char* p_pszPath, const char* p_pszReplace) {
        JCDownloadMgr* pDmgr = JCDownloadMgr::getInstance();
        if (pDmgr) {
            pDmgr->setFinalReplacePath(p_pszPath, p_pszReplace);
        }
    }

    void JSConchConfig::setDownloadTail(int type, const char* p_strTail) {
        JCDownloadMgr* pDmgr = JCDownloadMgr::getInstance();
        if (pDmgr) {
            pDmgr->setDownloadTail(type, p_strTail);
        }
    }

    void JSConchConfig::setDownloadReplaceExt(const char* p_pszOrigin, const char* p_pszNew) {

    }

    void JSConchConfig::setDownloadIgnoreCRLR(bool b) {
       
    }

    void JSConchConfig::resetDownloadIgnoreCRLR() {
       
    }

    void JSConchConfig::addChkIgnoreChksumExt(const char* p_pszExt) {
       
    }

    void JSConchConfig::clearChkIgnoreChksumExt() {
       
    }

    void JSConchConfig::setDownloadUnmask(const char* p_pszExt, unsigned int p_nKey, int p_nLen) {
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        if (pdmgr) {
            pdmgr->setDownloadUnmask(p_pszExt, p_nKey, p_nLen);
        }
    }

    void JSConchConfig::resetDownloadUnmask() {
        JCDownloadMgr* pdmgr = JCDownloadMgr::getInstance();
        if (pdmgr) {
            pdmgr->resetDownloadUnmask();
        }
    }

    void JSConchConfig::setDownloadNoResponseTimeout(int p_nDuration) {
        JCDownloadMgr::s_nNoResponseTimeout = p_nDuration;
    }

    void JSConchConfig::resetDownloadNoResponseTimeout() {
        JCDownloadMgr::s_nNoResponseTimeout = 15000;
    }

    void JSConchConfig::setDownloadVersionString(const char* p_pszVersion) {
        if (p_pszVersion && strlen(p_pszVersion) > 0) {
            JCFileRes::s_strExtVersion = p_pszVersion;
            JCFileRes::s_strExtVersion += "=";
        }
        else
            JCFileRes::s_strExtVersion = "";
    }

    const char* JSConchConfig::getOS()
    {
#ifdef __APPLE__
        return "Conch-ios";
#elif ANDROID
        return "Conch-android";
#elif WIN32
        return "Conch-window";
#endif
    }
    const char* JSConchConfig::getBrowserInfo()
    {
#ifdef __APPLE__
        return "Conch-ios";
#elif ANDROID
        return "Conch-android";
#elif WIN32
        return "Conch-window";
#endif
    }
    const char* JSConchConfig::getGuid()
    {
#ifdef __APPLE__
        m_sGUID = CToObjectCGetGUID();
        return m_sGUID.c_str();
#elif ANDROID
        //TODO android是通过wifiMac获得的
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getWifiMac", kRet))
        {
            m_sGUID = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
        }
        LOGI("getGuid::get_Value=%s", m_sGUID.c_str());
        return m_sGUID.c_str();
#elif WIN32
        return "window-TODO";
#endif
    }
    const char* JSConchConfig::getRuntimeVersion()
    {
#ifdef __APPLE__
        return "ios-conch5-1.0.7";
#elif ANDROID
        return "android-conch5-1.0.7";
#elif WIN32
        return "window-conch5-1.0.7";
#endif
    }
    const char* JSConchConfig::getAppVersion()
    {
#ifdef __APPLE__
        m_sAppVersion = CToObjectCGetAppVersion();
        return m_sAppVersion.c_str();
#elif ANDROID
        //TODO
        return "1.0";
#elif WIN32
        //TODO
        return "1.0";
#endif
    }
    const char* JSConchConfig::getAppLocalVersion()
    {
#ifdef __APPLE__
        m_sAppLocalVersion = CToObjectCGetAppLocalVersion();
        return m_sAppLocalVersion.c_str();
#elif ANDROID
        return "1.0";
#elif WIN32
        return "1.0";
#endif
    }

    //TODO 建议去掉这个函数。目前可能只有老贾再用。经历了ios与android的返回不同后，他也不会再用了。
    const char* JSConchConfig::getDeviceModel()
    {
#ifdef __APPLE__
        m_sDeviceModel = CToObjectCGetDeviceModel().c_str();
        return m_sDeviceModel.c_str();
#elif ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "getAndroidModel", kRet))
        {
            m_sDeviceModel = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
        }
        LOGI("getDeviceModel::get_Value=%s", m_sDeviceModel.c_str());
        return m_sDeviceModel.c_str();
#elif WIN32
        return "window-Unknow";
#endif
    }

    bool JSConchConfig::getIsPlug() {
        return JCSystemConfig::s_bIsPlug != 0;
    }

    const char* JSConchConfig::getJsonparamExt()
    {
        return g_kSystemConfig.m_jsonparamExt.c_str();
    }
    int JSConchConfig::getGLCaps()
    {
        return JCRenderer::s_nGLCaps;
    }
    bool JSConchConfig::supportTextureCustomData()
    {
        return true;
    }
    const char* JSConchConfig::getDeviceInfo()
    {
#ifdef __APPLE__
        m_sDeviceInfo = CToObjectCGetDeviceInfo();
        return m_sDeviceInfo.c_str();
#elif ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "GetDeviceInfo", kRet))
        {
            m_sDeviceInfo = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
        }
        LOGI("getDeviceInfo::get_Value=%s", m_sDeviceInfo.c_str());
        return m_sDeviceInfo.c_str();
#elif WIN32
        return "{\"resolution\":\"1920*1080\",	\"guid\":\"xxxxxxxxx\",\"imei\":[\"imeixxx\"],\"imsi\":[\"imsixxx\"],\"os\":\"windows\",\"osversion\":\"windows7 64\",\"phonemodel\":\"Wintel\"	}";
#endif
    }
    float JSConchConfig::getSystemVersion()
    {
        //TODO
#ifdef __APPLE__
        return 7.0;
#elif ANDROID
        return 4.1;
#elif WIN32
        return 1;
#endif
    }
    float JSConchConfig::getCurrentDeviceSystemVersion()
    {
#ifdef __APPLE__
        return CToObjectCGetDeviceSystemVersion();
#else
        return 0.0f;
#endif
    }
    void JSConchConfig::setScreenScale(float fScaleW, float fScaleH, float ftx, float fty) {
        m_fScreenTx = ftx;
        m_fScreenTy = fty;
        m_fScreenScaleW = fScaleW;
        m_fScreenScaleH = fScaleH;
        //压流通知render线程
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_setMainCanvasScaleInfo);
        pRenderCmd->append(fScaleW);
        pRenderCmd->append(fScaleH);
        pRenderCmd->append(ftx);
        pRenderCmd->append(fty);

    }
    float JSConchConfig::getScreenScaleW() {
        return m_fScreenScaleW;
    }
    float JSConchConfig::getScreenScaleH() {
        return m_fScreenScaleH;
    }
    int JSConchConfig::getMaxTextureMemSize()
    {
        return g_kSystemConfig.m_nMaxTextureMemSize;
    }
    void JSConchConfig::setMaxTextureMemSize(int nSize)
    {
        if (JCConch::s_pConchRender->m_pTextureManager)
        {
            if (JCConch::s_pConchRender->m_pTextureManager->m_nCurSize <= 0)
            {
                g_kSystemConfig.m_nMaxTextureMemSize = nSize;
                JCConch::s_pConchRender->m_pTextureManager->setFreeSize(nSize);
            }
            else
            {
                LOGE("JSConchConfig::setMaxTextureMemSize Failed，必须在程序开始前设置");
            }
        }
    }
    int JSConchConfig::getPushAtlasLimitSize()
    {
        return JCImage::s_nPushAtlasLimitSize;
    }
    void JSConchConfig::setPushAtlasLimitSize(int nSize)
    {
        JCImage::s_nPushAtlasLimitSize = nSize;
    }
    int JSConchConfig::getAtlasNum()
    {
        return g_kSystemConfig.m_nAtlasNum;
    }
    void JSConchConfig::setAtlasNum(int nNum)
    {
        g_kSystemConfig.m_nAtlasNum = nNum;
        if (JCConch::s_pConchRender->m_pAtlasManager)
        {
            JCConch::s_pConchRender->m_pAtlasManager->m_nMaxAtlasNum = nNum;
        }
    }
    bool JSConchConfig::getLocalable()
    {
        return JCSystemConfig::s_bLocalizable;
    }
    void JSConchConfig::setLocalable(bool isLocalPackage)
    {
        JCSystemConfig::s_bLocalizable = isLocalPackage;
    }
    void JSConchConfig::setMouseFrame(double thredholdms)
    {
        g_kSystemConfig.m_nFrameType = FT_MOUSE;
        g_kSystemConfig.m_nFrameThreshold = thredholdms;
        g_kSystemConfig.m_nSleepTime = 32;
    }
    void JSConchConfig::setSlowFrame(bool p_bIsSlow)
    {
        g_kSystemConfig.m_nFrameType = p_bIsSlow ? FT_SLOW : FT_FAST;
        g_kSystemConfig.m_nSleepTime = 32;
    }
    void JSConchConfig::setLimitFPS(int nFpsNum)
    {
        if( nFpsNum >= 60 )
        {
            g_kSystemConfig.m_nFrameType = FT_FAST;
        }
        else
        {
            g_kSystemConfig.m_nFrameType = FT_SLOW;
            g_kSystemConfig.m_nSleepTime = 1000 / nFpsNum - 1;
        }
    }
    void JSConchConfig::setCurlProxy(const char* pProxy) {
        if (pProxy) {
            LOGI("setCurlProxy %s", pProxy);
            JCDownloadMgr::s_curlProxyString = pProxy;
            JCDownloadMgr::getInstance()->setProxyString(pProxy);
        }
    }

    void JSConchConfig::setWebsocketProxy(const char* pProxy) {
        if (pProxy) {
            WebSocket::s_strProxy = pProxy;
            LOGI("setWebsocketProxy:%s", pProxy);
        }
    }
    void JSConchConfig::setTouchMode(bool bMode)
    {
        JSInput::getInstance()->setTouchMode(bMode);
    }
    bool JSConchConfig::getTouchMode()
    {
        return JSInput::getInstance()->getTouchMode();
    }
    void JSConchConfig::setTransparentMode()
    {
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_setTransparentMode);
        float fAlpha = 0.5;
        pRenderCmd->append(fAlpha);
    }
    void JSConchConfig::setDebugLevel(int nLevel)
    {
        g_nDebugLevel = nLevel;
    }
    void JSConchConfig::setImageReleaseSpaceTime(int nSpaceTime)
    {
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_setImageReleaseSpaceTime);
        pRenderCmd->append(nSpaceTime);
    }
    void JSConchConfig::enableMemorySurvey(bool bEnable)
    {
        JCMemorySurvey::GetInstance()->setEnable(bEnable);
    }
    void JSConchConfig::printAllMemorySurvey(char* sPath)
    {
        string sWritePath = "";
        if (sPath == NULL || strlen(sPath) <= 0)
        {
            sWritePath = JCConch::s_pConch->m_sCachePath.c_str();
        }
        else
        {
            sWritePath = sPath;
        }
        int nTextureMem = getMaxTextureMemSize();
        int nAtlasMem = getAtlasNum() * g_kSystemConfig.m_nAtlasWidth * g_kSystemConfig.m_nAtlasHeight * 4;
        int nSoundMem = 0;
        JCAudioWavPlayer* pWavPlayer = JCAudioManager::GetInstance()->m_pWavPlayer;
        for (JCAudioWavPlayer::MapWaveInfoIter iter = pWavPlayer->m_vWaveInfos.begin(); iter != pWavPlayer->m_vWaveInfos.end(); iter++)
        {
            JCWaveInfo* pInfo = iter->second;
            nSoundMem += pInfo->m_nRealDataSize;
            nSoundMem += 80;
        }
        int nImageNum = JCConch::s_pConchRender->m_pImageManager->m_vOccupiedMemImages.size();
        JCMemorySurvey::GetInstance()->printAll(sWritePath.c_str(),nTextureMem,nAtlasMem,nSoundMem, nImageNum );
    }

    void JSConchConfig::showInternalPerfBar(int b, float scale) {
        if (scale == 0.0f)scale = 10.0f;

        if (b > 0) {
            g_kSystemConfig.m_bShowInternalPerBar = true;

            JCPerfHUD::addData(new perfBarData(JCPerfHUD::PHUD_BAR_JS_ONDRAW, 0x6600ff00, "jsbar", scale,0.0f));
            JCPerfHUD::addData(new perfBarData(JCPerfHUD::PHUD_BAR_RENDER, 0x66ff0000, "renderbar", scale,0.0f));
            JCPerfHUD::addData(new perfBarData(JCPerfHUD::PHUD_BAR_JSWAIT, 0x66003300, "jswait", scale,2.0f));
            JCPerfHUD::addData(new perfBarData(JCPerfHUD::PHUD_BAR_GLWAIT, 0x66330000, "glwait", scale,2.0f));
        }
        else {
            g_kSystemConfig.m_bShowInternalPerBar = false;
            JCPerfHUD::delData(JCPerfHUD::PHUD_BAR_JS_ONDRAW);
            JCPerfHUD::delData(JCPerfHUD::PHUD_BAR_RENDER);
            JCPerfHUD::delData(JCPerfHUD::PHUD_BAR_JSWAIT);
            JCPerfHUD::delData(JCPerfHUD::PHUD_BAR_GLWAIT);
        }
    }

    void JSConchConfig::useChoreographer(int b) {
#ifdef ANDROID
#else
        b = 0;
#endif
        g_kSystemConfig.m_bUseChoreographer = b > 0;
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "useChoreographer", b ,kRet);
#endif
        if (b) {
        }
        else {//启动一下
            if (JCScriptRuntime::s_JSRT) {
                JCScriptRuntime::s_JSRT->m_ScriptThread.post(std::bind(
                    &JCScriptRuntime::onUpdate
                    , JCScriptRuntime::s_JSRT));
            }
        }
    }
    void JSConchConfig::setGraphicsTextBaseLine(int nType)
    {
        JSGraphics::s_nTextBaseLineType = (TEXT_BASE_LINE_TYPE)nType;
    }
    void JSConchConfig::testSleep(int tm) {
        std::this_thread::sleep_for(std::chrono::milliseconds(tm));
    }

    void JSConchConfig::setResolution(int w, int h) {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setResolution", w,h, kRet);
#else
#endif
    }
    void JSConchConfig::setImageMisoperationWarningTime(int nSpaceTime)
    {
        JCImage::s_nMisoperationWarningTime = nSpaceTime;
    }
    void JSConchConfig::setDownloadConnTimeout(int tm) {
        JCDownloadMgr::s_nConnTimeout = tm;
    }

    void JSConchConfig::setDownloadOptTimeout(int tm) {
        JCDownloadMgr::s_nOptTimeout = tm;
    }
    bool JSConchConfig::getUseAndroidSystemFont() {
        return g_kSystemConfig.m_bUseAndroidSystemFont;
    }
    void JSConchConfig::setUseAndroidSystemFont(bool b) {
        g_kSystemConfig.m_bUseAndroidSystemFont = b;
    }

    void JSConchConfig::enableEncodeURI(bool b) {
        JCDownloadMgr::s_bEncodeURI = b;
    }
    void JSConchConfig::setSoundGarbageCollectionTime(int nTime)
    {
        JCAudioWavPlayer::s_nGarbageCollectionTime = nTime;
    }
    void JSConchConfig::exportJS()
    {
        JSP_GLOBAL_CLASS("conchConfig", JSConchConfig);
        JSP_ADD_PROPERTY_RO(supportTextureCustomData, JSConchConfig, supportTextureCustomData);
        JSP_ADD_PROPERTY_RO(glCaps, JSConchConfig, getGLCaps);
        JSP_ADD_PROPERTY_RO(paramExt, JSConchConfig, getJsonparamExt);
        JSP_ADD_PROPERTY(urlIgnoreCase, JSConchConfig, getUrlIgnoreCase, setUrlIgnoreCase);
        JSP_ADD_PROPERTY(maxTextureMemSize, JSConchConfig,getMaxTextureMemSize,setMaxTextureMemSize);
        JSP_ADD_PROPERTY(pushAtlasLimitSize, JSConchConfig, getPushAtlasLimitSize,setPushAtlasLimitSize);
        JSP_ADD_PROPERTY(atlasNum, JSConchConfig,getAtlasNum,setAtlasNum);
        JSP_ADD_PROPERTY(localizable, JSConchConfig, getLocalable, setLocalable);
        JSP_ADD_PROPERTY(useAndroidSystemFont, JSConchConfig, getUseAndroidSystemFont, setUseAndroidSystemFont);
        JSP_ADD_METHOD("getStoragePath", JSConchConfig::getLocalStoragePath);
        JSP_ADD_METHOD("getTotalMem", JSConchConfig::getTotalMem);
        JSP_ADD_METHOD("getUsedMem", JSConchConfig::getUsedMem);
        JSP_ADD_METHOD("getAvalidMem", JSConchConfig::getAvalidMem);
        JSP_ADD_METHOD("getScreenInch", JSConchConfig::getScreenInch);
        JSP_ADD_METHOD("setTouchMoveRange", JSConchConfig::setTouchMoveRange);
        JSP_ADD_METHOD("setScreenOrientation", JSConchConfig::setScreenOrientation);
        JSP_ADD_METHOD("setScreenScale", JSConchConfig::setScreenScale);
        JSP_ADD_METHOD("getScreenScaleW", JSConchConfig::getScreenScaleW);
        JSP_ADD_METHOD("getScreenScaleH", JSConchConfig::getScreenScaleH);
        JSP_ADD_METHOD("setUrlIgnoreCase", JSConchConfig::setUrlIgnoreCase);
        JSP_ADD_METHOD("getUrlIgnoreCase", JSConchConfig::getUrlIgnoreCase);
        JSP_ADD_METHOD("getNetworkType", JSConchConfig::getNetworkType);
        JSP_ADD_METHOD("getRuntimeVersion", JSConchConfig::getRuntimeVersion);
        JSP_ADD_METHOD("setDownloadTryNum", JSConchConfig::setDownloadTryNum);
        JSP_ADD_METHOD("setDownloadPathReplace", JSConchConfig::setDownloadPathReplace);
        JSP_ADD_METHOD("setDownloadTail", JSConchConfig::setDownloadTail);
        JSP_ADD_METHOD("setDownloadNoResponseTimeout", JSConchConfig::setDownloadNoResponseTimeout);
        JSP_ADD_METHOD("setDownloadReplaceExt", JSConchConfig::setDownloadReplaceExt);
        JSP_ADD_METHOD("setDownloadIgnoreCRLR", JSConchConfig::setDownloadIgnoreCRLR);
        JSP_ADD_METHOD("addChkIgnoreChksumExt", JSConchConfig::addChkIgnoreChksumExt);
        JSP_ADD_METHOD("clearChkIgnoreChksumExt", JSConchConfig::clearChkIgnoreChksumExt);
        JSP_ADD_METHOD("setDownloadUnmask", JSConchConfig::setDownloadUnmask);
        JSP_ADD_METHOD("resetDownloadUnmask", JSConchConfig::resetDownloadUnmask);
        JSP_ADD_METHOD("setDownloadVersionString", JSConchConfig::setDownloadVersionString);
        JSP_ADD_METHOD("getOS", JSConchConfig::getOS);
        JSP_ADD_METHOD("getAppVersion", JSConchConfig::getAppVersion);
        JSP_ADD_METHOD("getAppLocalVersion", JSConchConfig::getAppLocalVersion);
        JSP_ADD_METHOD("getBrowserInfo", JSConchConfig::getBrowserInfo);
        JSP_ADD_METHOD("getGuid", JSConchConfig::getGuid);
        JSP_ADD_METHOD("getDeviceModel", JSConchConfig::getDeviceModel);
        JSP_ADD_METHOD("getDeviceInfo", JSConchConfig::getDeviceInfo);
        JSP_ADD_METHOD("getSystemVersion", JSConchConfig::getSystemVersion);
        JSP_ADD_METHOD("getIsPlug", JSConchConfig::getIsPlug);
        JSP_ADD_METHOD("setLimitFPS", JSConchConfig::setLimitFPS);
        JSP_ADD_METHOD("setMouseFrame", JSConchConfig::setMouseFrame);
        JSP_ADD_METHOD("setSlowFrame", JSConchConfig::setSlowFrame);
        JSP_ADD_METHOD("setCurlProxy", JSConchConfig::setCurlProxy);
        JSP_ADD_METHOD("setWebsocketProxy", JSConchConfig::setWebsocketProxy);
        JSP_ADD_METHOD("setTouchMode", JSConchConfig::setTouchMode);
        JSP_ADD_METHOD("getTouchMode", JSConchConfig::getTouchMode);
        JSP_ADD_METHOD("setTransparentMode", JSConchConfig::setTransparentMode);
        JSP_ADD_METHOD("setDebugLevel", JSConchConfig::setDebugLevel);
        JSP_ADD_METHOD("setImageReleaseSpaceTime", JSConchConfig::setImageReleaseSpaceTime);
        JSP_ADD_METHOD("enableMemorySurvey", JSConchConfig::enableMemorySurvey);
        JSP_ADD_METHOD("printAllMemorySurvey", JSConchConfig::printAllMemorySurvey);
        JSP_ADD_METHOD("showInternalPerfBar", JSConchConfig::showInternalPerfBar);
        JSP_ADD_METHOD("useChoreographer", JSConchConfig::useChoreographer);
        JSP_ADD_METHOD("setGraphicsTextBaseLine", JSConchConfig::setGraphicsTextBaseLine);
#ifdef ANDROID
        //目前只有android实现了这个函数，所以ios等就不往外导了，这样判断方便。
        JSP_ADD_METHOD("setResolution", JSConchConfig::setResolution);
#endif
        //TEST
        JSP_ADD_METHOD("test_sleep", JSConchConfig::testSleep);
        JSP_ADD_METHOD("setImageMisoperationWarningTime", JSConchConfig::setImageMisoperationWarningTime);
        JSP_ADD_METHOD("setDownloadConnTimeout", JSConchConfig::setDownloadConnTimeout);
        JSP_ADD_METHOD("setDownloadOptTimeout", JSConchConfig::setDownloadOptTimeout);
        JSP_ADD_METHOD("enableEncodeURI", JSConchConfig::enableEncodeURI);
        JSP_ADD_METHOD("setSoundGarbageCollectionTime", JSConchConfig::setSoundGarbageCollectionTime);
        JSP_INSTALL_GLOBAL_CLASS("conchConfig", JSConchConfig, this);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
