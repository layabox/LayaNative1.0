/**
@file			JSRuntime.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#include "JSRuntime.h"
#include "util/JCCrypto.h"
#include "downloadMgr/JCDownloadMgr.h"
#include "../../JCConch.h"
#include <downloadCache/JCFileSource.h>
#ifdef ANDROID
    #include "../../CToJavaBridge.h"
#elif __APPLE__
    #include "../../CToObjectC.h"
#endif
#include "../../JCSystemConfig.h"
#include "JSInput.h"
#include "JSConchConfig.h"
#include "../../../common/imageLib/JCImageRW.h"
//------------------------------------------------------------------------------
namespace laya
{
    ADDJSCLSINFO(JSRuntime, JSObjNode);
    JSRuntime::JSRuntime()
    {
        m_pScrpitRuntime = JCScriptRuntime::s_JSRT;
    
    }
    JSRuntime::~JSRuntime()
    {
        m_pScrpitRuntime = NULL;
    }
    void JSRuntime::setOnFrameFunction(JSValueAsParam p_pFunction)
    {
        m_pScrpitRuntime->m_pJSOnFrameFunction.set(onframeid, this, p_pFunction);
    }

    void JSRuntime::setOnDrawFunction(JSValueAsParam p_pFunction) {
        m_pScrpitRuntime->m_pJSOnDrawFunction.set(ondrawid, this, p_pFunction);
    }

    void JSRuntime::setOnResizeFunction(JSValueAsParam p_onresize)
    {
        m_pScrpitRuntime->m_pJSOnResizeFunction.set(onresizeid, this, p_onresize);
    }
    void JSRuntime::setCmdBuffer(JSValueAsParam pArrayBuffer) {
        m_pScrpitRuntime->m_pJSCmdABObjSharedWithJS.set(cmdABObjSharedWithJS, this, pArrayBuffer);
        m_pScrpitRuntime->setCmdBuffer(pArrayBuffer);
    }

    void JSRuntime::setBuffer(JSValueAsParam pArrayBuffer) {
        m_pScrpitRuntime->m_pJSCmdOtherBufferSharedWithJS.set(cmdOtherBufferSharedWithJS, this, pArrayBuffer);
        m_pScrpitRuntime->setBuffer(pArrayBuffer);
    }
    void JSRuntime::setHref(JSValueAsParam p_sHref)
    {
        const char* sBuffer = NULL;
        if (JsObjHandle::tryGetStr(p_sHref, (char**)&sBuffer)) {
            if (sBuffer && strlen(sBuffer) > 0)
            {
                if (!sBuffer)
                    return;
                std::string url = m_pScrpitRuntime->m_pUrl->resolve(sBuffer);
                g_kSystemConfig.m_strStartURL = url;
                g_kSystemConfig.m_strStartURL.at(0) = g_kSystemConfig.m_strStartURL.at(0);
                JCDownloadMgr* pdm = JCDownloadMgr::getInstance();
                if (pdm) {
                    pdm->resetFinalReplacePath();
                    pdm->resetDownloadTail();
                    pdm->resetDownloadReplaceExt();
                }
                m_pScrpitRuntime->m_pUrl->parse(url.c_str());
                std::string tempurl = m_pScrpitRuntime->m_pUrl->m_Host;
                JCEncrypt::getpassCode(tempurl);

                std::string ss = m_pScrpitRuntime->m_pUrl->m_Host;
                int n = ss.find(':');
                if (n>0)
                    ss.at(n) = '.';

                std::string cookiefile = JSConchConfig::getInstance()->getLocalStoragePath() + ss + "_curlcookie.txt";
                pdm->setCookieFile(cookiefile.c_str());
#ifdef ANDROID
                CToJavaBridge::JavaRet kRet;
                CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setHrefToJava", url.c_str(), kRet);
#endif
            }
        }
    }
    void  JSRuntime::disableConchResManager()
    {

    }
    void JSRuntime::disableConchAutoRestoreLostedDevice()
    {

    }
    void JSRuntime::setOnInvalidGLRes(JSValueAsParam p_pFunction)
    {
        m_pScrpitRuntime->m_pJSOnInvalidGLRes.set(oninvalidglid, this, p_pFunction);
    }
    void JSRuntime::setMouseEvtFunction(JSValueAsParam p_pFunction)
    {
        m_pScrpitRuntime->m_pJSMouseEvtFunction.set(onmouseevtid, this, p_pFunction);
    }
    void JSRuntime::setTouchEvtFunction(JSValueAsParam p_pFunction)
    {
        m_pScrpitRuntime->m_pJSTouchEvtFunction.set(ontouchevtid, this, p_pFunction);
    }
    void JSRuntime::setDeviceMotionEvtFunction(JSValueAsParam p_pFunction)
    {
        m_pScrpitRuntime->m_pJSDeviceMotionEvtFunction.set(ondevicemotionevtid, this, p_pFunction);
    }
    void JSRuntime::setKeyEvtFunction(JSValueAsParam p_pFunction)
    {
        m_pScrpitRuntime->m_pJSKeyEvtFunction.set(onkeyevtid, this, p_pFunction);
    }
    void JSRuntime::setNetworkEvtFunction(JSValueAsParam p_pFunction)
    {
        m_pScrpitRuntime->m_pJSNetworkEvtFunction.set(onnetworkevt, this, p_pFunction);
    }
	void JSRuntime::setOnBackPressedFunction(JSValueAsParam p_pFunction)
    {
		std::lock_guard<std::mutex> lock(m_pScrpitRuntime->m_OnBackPressedMutex);
		m_pScrpitRuntime->m_bJSOnBackPressedFunctionSet = true;
        m_pScrpitRuntime->m_pJSOnBackPressedFunction.set(onbackpressed, this, p_pFunction);
    }
    bool JSRuntime::initFreeTypeDefaultFontFromFile(const char* sDefaultTTFs)
    {
        return m_pScrpitRuntime->m_pFreeTypeRender->initDefaultFont(sDefaultTTFs);
    }
    void JSRuntime::captureScreen(JSValueAsParam p_pFunction)
    {
        m_pScrpitRuntime->m_pJSOnceOtherEvtFuction.set(onotherevtid, this, p_pFunction);
#ifdef ANDROID
		CToJavaBridge::JavaRet kRet;
		CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "captureScreen", kRet);
#elif __APPLE__
        CToObjectCCaptureScreen();
#endif
    }
    bool JSRuntime::initFreeTypeDefaultFontFromBuffer(JSValueAsParam pArrayBufferArgs)
    {
        JsArrayBufferData ab(false);
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, ab);
        if (bIsArrayBuffer)
        {
            return m_pScrpitRuntime->m_pFreeTypeRender->initDefaultFont(ab.data, ab.len);
        }
        return false;
    }
    bool JSRuntime::setFontFaceFromUrl(const char* sFontFamily, const char* sTTFFileName)
    {
        return m_pScrpitRuntime->m_pFreeTypeRender->setFontFaceFromUrl(sFontFamily, sTTFFileName);
    }
    bool JSRuntime::setFontFaceFromBuffer(const char* sFontFamily, JSValueAsParam pArrayBufferArgs )
    {
        JsArrayBufferData ab(false);
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, ab);
        if (bIsArrayBuffer)
        {
            return m_pScrpitRuntime->m_pFreeTypeRender->setFontFaceFromBuffer(sFontFamily, ab.data, ab.len);
        }
        return false;
    }
    bool JSRuntime::removeFont(const char* sFontFamily)
    {
        return m_pScrpitRuntime->m_pFreeTypeRender->removeFont(sFontFamily);
    }

    const char* JSRuntime::getCachePath() {
        return JCConch::s_pConch->m_sCachePath.c_str();
    }

    unsigned char* _readAssetAlloc(int sz, void* pUserData) {
        auto ret = new unsigned char[sz];
        *(unsigned char**)pUserData = ret;
        return ret;
    }

    JsValue JSRuntime::readFileFromAsset(const char* file, const char* encode) {
        if (!m_pScrpitRuntime->m_pAssetsRes) {
            return JSP_TO_JS_NULL;
        }
        int sz = 0;
        unsigned char* pBuff = NULL;
        if (m_pScrpitRuntime->m_pAssetsRes->loadFileContent(file, _readAssetAlloc, &pBuff, sz)) {
            if (strcmp(encode, "utf8") == 0) {
                std::string str;
                str.assign((char*)pBuff, sz);
                delete[] pBuff;
                return JSP_TO_JS_STR(str.c_str());
            }
            else {
                //TODO 写一个不用自己保留的AB
                JsValue ab = createJSAB((char*)pBuff, sz);
                //JSArrayBuffer* pab = JSArrayBuffer::create(sz);
                //memcpy(pab->getPtr(), pBuff, sz);
                delete[] pBuff;
                //return (pab->toLocal());;
                return ab;
            }
        }
        return JSP_TO_JS_NULL;
    }

    void JSRuntime::showLoadingView(bool p_bShow)
    {
        /*if (!p_bShow)
        {
            JSP_RUN_SCRIPT("window.loadingView&&window.loadingView.loading(100);");
        }*/
    }
    void JSRuntime::showAssistantTouch(bool p_bShow)
    {
#ifdef ANDROID
        std::vector<intptr_t> params;
        params.push_back((intptr_t)p_bShow);
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "showAssistantTouch", p_bShow, kRet);
#elif WIN32
        //TODO
#elif __APPLE__
        CToObjectCShowAssistantTouch(p_bShow);
#endif
    }
    void JSRuntime::flushRenderCommands()
    {
        m_pScrpitRuntime->flushSharedCmdBuffer();
    }
    void JSRuntime::appendBuffer(JSValueAsParam pArrayBufferArgs, int nDataSize, int nOffset)
    {
        JsArrayBufferData ab(!isSupportTypedArrayAPI());
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, ab);
        if (bIsArrayBuffer)
        {
            if (nDataSize+nOffset <= ab.len)
            {
                JCMemClass* pWebGLCmd = m_pScrpitRuntime->m_pRenderCmd;
                pWebGLCmd->append(ab.data + nOffset, nDataSize);
            }
            else
            {
                LOGE("JSRuntime::appendBuffer error dataSize > ArrayBuffer size");
            }
        }
        else
        {
            LOGE("JSRuntime::appendBuffer buffer type error");
        }
    }

    void JSRuntime::setScreenWakeLock(bool p_bWakeLock)
    {
#ifdef __APPLE__
        CToObjectCSetScreenWakeLock(p_bWakeLock);
#elif ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setScreenWakeLock", p_bWakeLock, kRet);
#elif WIN32

#endif
    }

    void JSRuntime::setSensorAble(bool p_bSensorAble)
    {
#ifdef __APPLE__
        CToObjectCSetSensorAble(p_bSensorAble);
#elif ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setSensorAble", p_bSensorAble, kRet);
#elif WIN32

#endif
    }

    JsValue JSRuntime::strTobufer(const char* s)
    {
        return createJSABAligned((char*)s, (strlen(s)+1));
    }

    const char* JSRuntime::getPresetUrl() {
        return g_kSystemConfig.m_strStartURL.c_str();
    }
    void JSRuntime::printCorpseImages()
    {
        JCImageManager* pImageManger = JCConch::s_pConchRender->m_pImageManager;
        if (pImageManger == NULL) return;
        std::string sFilePath = JCConch::s_pConch->m_strLocalStoragePath;
        sFilePath += "/imagesLog.txt";
        pImageManger->printCorpseImages(sFilePath.c_str());
    }

    std::string JSRuntime::callMethod(int objid,bool isSyn,const char*clsName, const char* methodName, const char* paramStr)
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet kRet;
        if (CToJavaBridge::GetInstance()->callMethod(objid, isSyn, clsName, methodName, paramStr, kRet))
        {
            std::string value = CToJavaBridge::GetInstance()->getJavaString(kRet.pJNI, kRet.strRet);
             LOGI("JSRuntime::callMethod %s %s %s", value.c_str(), clsName , methodName);
            return value;
        }
        return "";
#elif WIN32
        //TODO
#elif __APPLE__
        CToObjectCCallMethod( objid, isSyn, clsName, methodName,paramStr);
        return "";
#endif
        return "";
    }
    int JSRuntime::getCountVertext()
    {
        return m_pScrpitRuntime->m_nCountVertex;
    }
    int JSRuntime::getCountGroup()
    {
        return m_pScrpitRuntime->m_nCountGroup;
    }
    int JSRuntime::getCountNode()
    {
        return m_pScrpitRuntime->m_nCountNode;
    }

    bool JSRuntime::saveAsPng(JSValueAsParam pArrayBufferArgs, int w, int h, const char* p_pszFile)
    {
        JsArrayBufferData ab(!isSupportTypedArrayAPI());
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, ab);
        if (bIsArrayBuffer)
        {
            return laya::saveAsPng(ab.data, w, h, p_pszFile);
        }
        return false;
    }
    bool JSRuntime::saveAsJpeg(JSValueAsParam pArrayBufferArgs, int w, int h, const char* p_pszFile)
    {
        JsArrayBufferData ab(!isSupportTypedArrayAPI());
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, ab);
        if (bIsArrayBuffer)
        {
            ImageBaseInfo info;
            info.m_nBpp = 32;
            info.m_nWidth = w;
            info.m_nHeight = h;
            return laya::saveAsJpeg(ab.data, info, p_pszFile);
        }
        return false;
    }
    JsValue JSRuntime::convertBitmapToPng(JSValueAsParam pArrayBufferArgs, int w, int h)
    {
        JsArrayBufferData ab(!isSupportTypedArrayAPI());
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, ab);
        if (bIsArrayBuffer)
        {
            std::pair<unsigned char*, unsigned long> ret = laya::convertBitmapToPng((const char*)ab.data, w, h, 8);
            if (ret.first != nullptr)
                return  createJSAB((char*)ret.first, ret.second);
        }
        return JSP_TO_JS_NULL;
    }
    JsValue JSRuntime::convertBitmapToJpeg(JSValueAsParam pArrayBufferArgs, int w, int h)
    {
        JsArrayBufferData ab(!isSupportTypedArrayAPI());
        bool bIsArrayBuffer = extractJSAB(pArrayBufferArgs, ab);
        if (bIsArrayBuffer)
        {
            std::pair<unsigned char*, unsigned long> ret = laya::convertBitmapToJpeg((const char*)ab.data, w, h, 32);
            if (ret.first != nullptr)
                return  createJSAB((char*)ret.first, ret.second);
        }
        return JSP_TO_JS_NULL;
    }
    void JSRuntime::setExternalLinkEx(const char* sUrl,int x, int y, int w, int h, bool bCloseWebview)
    {
#ifdef ANDROID
        //TODO
        CToJavaBridge::JavaRet ret;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setExternalLink", sUrl,x,y,w,h, bCloseWebview?1:0,ret);
#elif __APPLE__
       CToObjectCSetExternalLink( sUrl,x,y,w,h, bCloseWebview);
#elif WIN32

#endif
    }
    void JSRuntime::setExternalLink(const char* sUrl)
    {
#ifdef ANDROID
        //TODO
        CToJavaBridge::JavaRet ret;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "setExternalLink", sUrl, 0, 0, 0, 0,1,ret);
#elif __APPLE__
        CToObjectCSetExternalLink(sUrl, 0, 0, 0, 0, true);
#elif WIN32

#endif
    }
    void JSRuntime::closeExternalLink()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet ret;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "closeExternalLink", ret);
#elif __APPLE__
        CToObjectCCloseExternalLink();
#elif WIN32

#endif
    }
    void JSRuntime::callWebviewJS(const char* sFunctionName, const char* sJsonParam, const char* sCallbackFunction)
    {
        LOGI("JSRuntime::callWebviewJS functionName=%s,sJsonParam=%s,sCallbackFunction=%s", sFunctionName, sJsonParam, sCallbackFunction );
#ifdef ANDROID
        CToJavaBridge::JavaRet ret;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "callWebViewJS", sFunctionName, sJsonParam, sCallbackFunction, ret);
#elif __APPLE__
        CToObjectCCallWebviewJS(sFunctionName, sJsonParam, sCallbackFunction);
#elif WIN32

#endif
    }
    void JSRuntime::hideWebview()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet ret;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "hideExternalLink", ret);
#elif __APPLE__
        CToObjectCHideWebView();
#elif WIN32
        
#endif
    }
    void JSRuntime::showWebView()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet ret;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "showExternalLink", ret);
#elif __APPLE__
        CToObjectCShowWebView();
#elif WIN32
        
#endif
    }
	void JSRuntime::regShaderDefine(const char* name, long long value)
	{
		//JCShaderDefine::registShaderDefine(name, value);
		if (name != NULL)
		{
			JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
			JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
			pRenderCmd->append(PUBLIC_CMD_DISPATH);
			pRenderCmd->append(PFT_registShaderDefine);
			int len = strlen(name);
			pRenderCmd->append(len);
			pRenderCmd->append(name,len);
			pRenderCmd->append(&value, sizeof(long long));
		}
	}
    void JSRuntime::exit()
    {
#ifdef ANDROID
        CToJavaBridge::JavaRet ret;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "exit", ret);
#elif __APPLE__
#elif WIN32

#endif
    }
    void JSRuntime::exportJS()
    {
        JSP_GLOBAL_CLASS("conch", JSRuntime);
        JSP_ADD_METHOD("setOnFrame", JSRuntime::setOnFrameFunction);
        JSP_ADD_METHOD("setOnDraw", JSRuntime::setOnDrawFunction);
        JSP_ADD_METHOD("setOnResize", JSRuntime::setOnResizeFunction);
        JSP_ADD_METHOD("setHref", JSRuntime::setHref);
        JSP_ADD_METHOD("disableConchResManager", JSRuntime::disableConchResManager);
        JSP_ADD_METHOD("disableConchAutoRestoreLostedDevice", JSRuntime::disableConchAutoRestoreLostedDevice);
        JSP_ADD_METHOD("setOnInvalidGLRes", JSRuntime::setOnInvalidGLRes);
        JSP_ADD_METHOD("setMouseEvtFunction", JSRuntime::setMouseEvtFunction);
        JSP_ADD_METHOD("setKeyEvtFunction", JSRuntime::setKeyEvtFunction);
        JSP_ADD_METHOD("setTouchEvtFunction", JSRuntime::setTouchEvtFunction);
        JSP_ADD_METHOD("setDeviceMotionEvtFunction", JSRuntime::setDeviceMotionEvtFunction);
        JSP_ADD_METHOD("setNetworkEvtFunction", JSRuntime::setNetworkEvtFunction);
		JSP_ADD_METHOD("setOnBackPressedFunction", JSRuntime::setOnBackPressedFunction);
        JSP_ADD_METHOD("setCmdBuffer", JSRuntime::setCmdBuffer);
        JSP_ADD_METHOD("setBuffer", JSRuntime::setBuffer);
        JSP_ADD_METHOD("showLoadingView", JSRuntime::showLoadingView);
        JSP_ADD_METHOD("flushRenderCommands", JSRuntime::flushRenderCommands);
        JSP_ADD_METHOD("appendBuffer", JSRuntime::appendBuffer);
        JSP_ADD_PROPERTY_RO(presetUrl, JSRuntime,getPresetUrl);
        JSP_ADD_METHOD("showAssistantTouch", JSRuntime::showAssistantTouch);
        JSP_ADD_METHOD("setScreenWakeLock", JSRuntime::setScreenWakeLock);
        JSP_ADD_METHOD("setSensorAble", JSRuntime::setSensorAble);
        //字体相关的
        JSP_ADD_METHOD("initFreeTypeDefaultFontFromFile",JSRuntime::initFreeTypeDefaultFontFromFile);
        JSP_ADD_METHOD("initFreeTypeDefaultFontFromBuffer", JSRuntime::initFreeTypeDefaultFontFromBuffer);
        JSP_ADD_METHOD("setFontFaceFromUrl", JSRuntime::setFontFaceFromUrl);
        JSP_ADD_METHOD("setFontFaceFromBuffer", JSRuntime::setFontFaceFromBuffer);
        JSP_ADD_METHOD("removeFont", JSRuntime::removeFont);
        JSP_ADD_METHOD("readFileFromAsset", JSRuntime::readFileFromAsset);
        JSP_ADD_METHOD("getCachePath", JSRuntime::getCachePath);
        JSP_ADD_METHOD("strTobufer", JSRuntime::strTobufer);
        JSP_ADD_METHOD("callMethod", JSRuntime::callMethod);
        JSP_ADD_METHOD("printCorpseImages", JSRuntime::printCorpseImages);
        JSP_ADD_METHOD("getCountVertext", JSRuntime::getCountVertext);
        JSP_ADD_METHOD("getCountGroup", JSRuntime::getCountGroup);
        JSP_ADD_METHOD("getCountNode", JSRuntime::getCountNode);
        JSP_ADD_METHOD("setExternalLink", JSRuntime::setExternalLink);
        JSP_ADD_METHOD("setExternalLinkEx", JSRuntime::setExternalLinkEx);
        JSP_ADD_METHOD("closeExternalLink", JSRuntime::closeExternalLink);
        JSP_ADD_METHOD("hideWebview", JSRuntime::hideWebview);
        JSP_ADD_METHOD("showWebview", JSRuntime::showWebView);
        JSP_ADD_METHOD("captureScreen", JSRuntime::captureScreen);
        JSP_ADD_METHOD("saveAsPng", JSRuntime::saveAsPng);
        JSP_ADD_METHOD("saveAsJpeg", JSRuntime::saveAsJpeg);
        JSP_ADD_METHOD("convertBitmapToPng", JSRuntime::convertBitmapToPng);
        JSP_ADD_METHOD("convertBitmapToJpeg", JSRuntime::convertBitmapToJpeg);
        JSP_ADD_METHOD("callWebviewJS", JSRuntime::callWebviewJS);
		JSP_ADD_METHOD("regShaderDefine", JSRuntime::regShaderDefine);
        JSP_ADD_METHOD("exit", JSRuntime::exit);
        JSP_INSTALL_GLOBAL_CLASS("conch", JSRuntime, this );
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
