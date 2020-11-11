/**
@file			JSGlobalExportCFun.cpp
@brief			
@author			wyw
@version		1.0
@date			2013_11_12
*/

#include "JSGlobalExportCFun.h"
#include "../JSInterface/JSInterface.h"
#include <util/Log.h>
#include <util/JCCommonMethod.h>
#include <fileSystem/JCFileSystem.h>
#include <buffer/JCBuffer.h>
#include <util/JCMemorySurvey.h>
#include <util/JCLayaUrl.h>
#include <util/JCCrypto.h>
#include <util/Log.h>
#include "JSConsole.h"
#include "XMLHttpRequest.h"
#include "JSConchConfig.h"
#include "JSXmlNode.h"
#include "JSXmlAttr.h"
#include "JSDOMParser.h"
#include "JSAudio.h"
#include "JSAppCache.h"
#include "JSWebSocket.h"
#include "JSFileSystem.h"
#include "JSZip.h"
#include "JSNotify.h"
#ifdef ANDROID
    #include "JSAndroidEditBox.h"
	#include "../../CToJavaBridge.h"
#elif WIN32
	#include <Windows.h>
    #include "JSWindowEditBox.h"
#elif __APPLE__
    #include "JSIOSEditBox.h"
    #include "CToObjectC.h"
#endif
#include "JSRuntime.h"
#include "../../JCConch.h"
#include <downloadCache/JCFileSource.h>
#include "JSWebGLContext.h"
#include "JSImage.h"
#include "JSTextMemoryCanvas.h"
#include "JSMeasureTextInfo.h"
#include "JSLayaContext.h"
#include "JSNode2D.h"
#include "JSGraphics.h"
#include "JSHistory.h"
#include "JSParticleTemplate2D.h"
#include "JSShaderActiveInfo.h"
#include <functional>
#include <downloadMgr/JCDownloadMgr.h>
#include <Performance/JCPerfHUD.h>
#include "JSArrayBufferRef.h"

extern int g_nInnerWidth ;
extern int g_nInnerHeight ;
extern bool g_bGLCanvasSizeChanged;
#ifdef WIN32
	int g_bEnableTouch = false;
#elif ANDROID
	int g_bEnableTouch = true;
#elif __APPLE__
	int g_bEnableTouch = true;
#endif
 std::string g_sExePath = "";

/** @brief 这个函数是为了实现comman库中的alert函数
 * 不定长的函数
*/
void alert(const char* fmt, ...)
{
    char buf[1024];
    char* pBuf = NULL;
    va_list args;
    va_start(args, fmt);
    int len = vsprintf(buf, fmt, args);
    if (len < 0) {
        pBuf = new char[4096];
        len = vsprintf(pBuf, fmt, args);
    }
    va_end(args);
    laya::LayaAlert(pBuf ? pBuf : buf);
    if (pBuf) 
    {
        delete[] pBuf;
    }
}
namespace laya 
{
    //下载大文件，zip用的
    struct JSFuncWrapper :public JsObjBase
    {
        enum { onprogid, oncompid };
        static JsObjClassInfo JSCLSINFO;
        JsObjHandle funcOnProg;
        JsObjHandle funcOnComp;
        bool stop;
        JSFuncWrapper(JSValueAsParam onprog, JSValueAsParam onComp)
        {
            createJSObj();
            funcOnProg.set(onprogid, this, onprog);
            funcOnComp.set(oncompid, this, onComp);
            stop = false;
        }
    };
    ADDJSCLSINFO(JSFuncWrapper, JSObjNode);


    void downloadBig_onProg_js(JSFuncWrapper* pWrapper, unsigned int total, unsigned int now, float speed){
        if (pWrapper->funcOnProg.Empty())return;
        pWrapper->funcOnProg.Call(total, now, speed);
        pWrapper->stop = __TransferToCpp<bool>::ToCpp(pWrapper->funcOnProg.m_pReturn);
    }

    int downloadBig_onProg(unsigned int total, unsigned int now, float speed, JSFuncWrapper* pWrapper){
        if (pWrapper && pWrapper->stop)
            return 1;			//TODO 删除的事情
        JCScriptRuntime::s_JSRT->m_pPoster->postToJS(std::bind(downloadBig_onProg_js, pWrapper, total, now, speed));
        return 0;
    }

    void downloadBig_onComp_js(int curlret, int httpret, JSFuncWrapper* pWrapper){
        if (!pWrapper->IsMyJsEnv()){
            delete pWrapper;
            return;
        }

        if (!pWrapper->funcOnComp.Empty()) {
            pWrapper->funcOnComp.Call(curlret,httpret);
        }
        delete pWrapper;
    }

    void downloadBig_onComp(JCBuffer& buff, const std::string& localip,
        const std::string& svip, int curlret, int httpret,
        const std::string& httpresheader, JSFuncWrapper* pWrapper)
    {
        JCScriptRuntime::s_JSRT->m_pPoster->postToJS(std::bind(downloadBig_onComp_js, curlret,httpret, pWrapper));
    }

    long _downloadBigFile(const char* p_pszUrl, const char* p_pszLocal, JSValueAsParam p_ProgCb,
        JSValueAsParam p_CompleteCb, int p_nTryNum, int p_nOptTimeout)
    {
        /*
        if (!canWrite(pCurProcess->getFSPermission(p_pszLocal))) {
        JSP_THROW("downloadBigFile to this localfile is forbidden!");
        LOGE("本用户不允许在%s目录下写文件", p_pszLocal);
        return 0;
        }*/

        JCDownloadMgr* dmgr = JCDownloadMgr::getInstance();
        JSFuncWrapper* pJSObj = new JSFuncWrapper(p_ProgCb, p_CompleteCb);
        dmgr->downloadBigFile(p_pszUrl, p_pszLocal,
            std::bind(downloadBig_onProg, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, pJSObj),
            std::bind(downloadBig_onComp, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, pJSObj), 
            p_nTryNum, p_nOptTimeout);;
        return (long)pJSObj;
    }


    void downloadHeader_onComp_js(char* pBuff, int curlret, int httpret, JSFuncWrapper* pWrapper) {
        if (!pWrapper->IsMyJsEnv()) {
            delete pWrapper;
            return;
        }

        if (!pWrapper->funcOnComp.Empty()) {
            if (pBuff) {
                pWrapper->funcOnComp.Call(curlret, httpret, pBuff);
                delete [] pBuff;
            }
            else {
                pWrapper->funcOnComp.Call(curlret, httpret);
            }
        }
        delete pWrapper;
    }

    void downloadHeader_onComp(JCBuffer& buff, const std::string& localip,
        const std::string& svip, int curlret, int httpret,
        const std::string& httpresheader, JSFuncWrapper* pWrapper)
    {
        char* pBuff = nullptr;
        if (buff.m_pPtr && buff.m_nLen) {
            //这个肯定是字符串
            pBuff = new char[buff.m_nLen+1];
            memcpy(pBuff, buff.m_pPtr, buff.m_nLen);
            pBuff[buff.m_nLen] = 0;
        }

        JCScriptRuntime::s_JSRT->m_pPoster->postToJS(std::bind(downloadHeader_onComp_js, pBuff, curlret, httpret, pWrapper));
    }

    long _downloadGetHeader(const char* p_pszUrl, JSValueAsParam p_CompleteCb, int p_nTryNum, int p_nOptTimeout){
        JCDownloadMgr* dmgr = JCDownloadMgr::getInstance();
        JSFuncWrapper* pJSObj = new JSFuncWrapper(p_CompleteCb, p_CompleteCb);//第一个没有用
        dmgr->getHeader(p_pszUrl,
            std::bind(downloadHeader_onComp, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, pJSObj),
            p_nTryNum, p_nOptTimeout);
        return (long)pJSObj;
    }

    void setTouchEvtFunc(JSValueAsParam pObj) 
    {
    }
    void setKeyEvtFunc(JSValueAsParam pObj) 
    {
    }
    void setJoystickEvtFunc(JSValueAsParam pObj) 
    {
    }
    void evalJS(const char* p_sSource)
    {
        JSP_RUN_SCRIPT(p_sSource);
    }
    void JSPrint(const char* p_sBuffer)
    {
        int nLen = strlen(p_sBuffer) + 3;
        unsigned short* ucStr = new unsigned short[nLen];
        int nlen = UTF8StrToUnicodeStr((unsigned char*)p_sBuffer, ucStr, nLen);
        LOGI("%ws\n", (wchar_t*)ucStr);
        delete[] ucStr;
        ucStr = NULL;
    }
    void LayaAlert(const char* p_sBuffer)
    {
#ifdef WIN32
        int nLen = strlen(p_sBuffer) + 3;
        unsigned short* ucStr = new unsigned short[nLen];
        int nlen = UTF8StrToUnicodeStr((unsigned char*)p_sBuffer, ucStr, nLen);
        delete[] ucStr;
        ucStr = NULL;
        std::wstring wsBuffer = (wchar_t*)utf8_unicode(p_sBuffer).c_str();
        MessageBoxW(NULL, wsBuffer.c_str(), L"alert", MB_OK);
#elif ANDROID
        CToJavaBridge::JavaRet kRet;
        CToJavaBridge::GetInstance()->callMethod(CToJavaBridge::JavaClass.c_str(), "alert", p_sBuffer, kRet);
#elif __APPLE__
        CToObjectCAlert(p_sBuffer);
#endif
    }
    void JSAlert(const char* p_sBuffer)
    {
        LayaAlert(p_sBuffer);
#ifndef WIN32
        LOGI("alert=%s", p_sBuffer);
#endif
    }
    int getInnerWidth()
    {
        return g_nInnerWidth;
    }
    int getInnerHeight()
    {
        return g_nInnerHeight;
    }
	JsValue getExePath()
    {
#ifdef WIN32
        TCHAR szPath[MAX_PATH];
        ::GetModuleFileName(NULL, szPath, MAX_PATH);
        ::GetFullPathName(szPath, MAX_PATH, szPath, NULL);
        g_sExePath = szPath;
        return JSP_TO_JS(const char*, g_sExePath.c_str());
#elif __APPLE__
        return JSP_TO_JS_NULL;
#else
        return JSP_TO_JS_NULL;
#endif
    }
	void PerfAddData(int nID, int nColor, float fScale, float fAlert )
    {
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_PerfAddData);
        pRenderCmd->append(nID);
        pRenderCmd->append(nColor);
        pRenderCmd->append(fScale);
        pRenderCmd->append(fAlert);
	}
    void PerfUpdateDt(int nID, float nSpace)
    {
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_PerfUpdateDt);
        pRenderCmd->append(nID);
        pRenderCmd->append(nSpace);
    }
	void PerfShow(int b)
    {
		JCConch::s_pConchRender->m_bShowPerf = b!=0;
	}
	void writeStrFileSync(const char* p_pszFile, const char* p_pString )
    {
        JCBuffer buf((char*)p_pString, strlen(p_pString), false, false);
        writeFileSync(p_pszFile, buf, JCBuffer::utf8);
	}
    void reloadCurJSThread() 
    {
#ifdef __APPLE__
        CToObjectCRunStopJSLoop();
#endif
        if (JCConch::s_pConch)
        {
            JCConch::s_pConch->postCmdToMainThread(JCConch::CMD_ReloadProcess, 0, 0);
        }
    }
	void simAndroidDeviceLosted()
    {
        //TODO以后有节点了，节点刷新要做一下
        JCConch::s_pConchRender->onGLDeviceLosted();
        if (JCConch::s_pConch)
        {
            JCConch::s_pConch->onClearMemory();
        }
	}
    std::string readTextAsset(const char* p_pszFile)
    {
        char* sBuffer = NULL;
        int nSize = 0;
        if (JCConch::s_pAssetsFiles->loadFileContent(p_pszFile, sBuffer, nSize))
        {
            std::string rsBuffer = sBuffer;
            delete[] sBuffer;
            return rsBuffer;
        }
        return "";
    }
    bool gbAlertException = true;
    void showAlertOnJsException(bool show)
    {
        gbAlertException = show;
    }

    std::string calcMD5(unsigned char* buf, int len){
        JCMD5 imd5;
        imd5.GenerateMD5(buf, len);
        return imd5.ToString();
    }

    std::string calcMD5_JSAB(JSValueAsParam pjs){
        JsArrayBufferData ab(!isSupportTypedArrayAPI());
        if (!extractJSAB(pjs, ab))
            return "";
        std::string ret = calcMD5((unsigned char*)ab.data, ab.len);
        return ret;
    }

	void JSGlobalExportC()	
    {
		JSP_GLOBAL_START1();
        JSRuntime* pJSRuntime = new JSRuntime();
        pJSRuntime->exportJS();
        JSHistory* pJsHistory = new JSHistory();
        pJsHistory->exportJS();
        JSConsole::exportJS();
        JSImage::exportJS();
        JSWebGLContext::exportJS();
        XMLHttpRequest::exportJS();
        JSConchConfig::getInstance()->exportJS();
        JSXmlDocument::exportJS();
        JSXmlNode::exportJS();
        JSXmlAttr::exportJS();
        JSDOMParser::exportJS();
        JSAudio::exportJS();
        JsAppCache::exportJS();
        JSWebSocket::exportJS();
        JSMeasureTextInfo::exportJS();
        JSShaderActiveInfo::exportJS();
        JSMarket::exportJS();
        JSZip::exportJS();
        JSNotify::exportJS();
        JSLayaContext::exportJS();

#ifdef WIN32
        JSWindowEditBox::exportJS();
#endif // WIN32

#ifdef ANDROID
        JSAndroidEditBox::exportJS();
#endif
#ifdef __APPLE__
        JSIOSEditBox::exportJS();
#endif
        JSNode2D::exportJS();
        JSGraphics::exportJS();
        JSParticleTemplate2D::exportJS();
        JSArrayBufferRef::exportJS();

        //JSTextCanvas
        JSTextMemoryCanvas* pTextMemoryCanvas = new JSTextMemoryCanvas();
        pTextMemoryCanvas->m_pTextMemoryCanvas = JCConch::s_pConchRender->m_pTextMemoryCanvas;
        pTextMemoryCanvas->m_pTextMemoryCanvas->setFreeTypeFontRender(JCScriptRuntime::s_JSRT->m_pFreeTypeRender);
        pTextMemoryCanvas->m_pTextMemoryCanvas->setFontManager(JCScriptRuntime::s_JSRT->m_pFontManager);
        pTextMemoryCanvas->exportJS();

        //以下是全局函数
        //------------------------------------------------------------------------------
        JSP_ADD_GLOBAL_FUNCTION(setTouchEvtFunction, setTouchEvtFunc, JSValueAsParam);
        JSP_ADD_GLOBAL_FUNCTION(setKeyEvtFunction, setKeyEvtFunc, JSValueAsParam);
        JSP_ADD_GLOBAL_FUNCTION(setJoystickEvtFunction, setJoystickEvtFunc, JSValueAsParam);
        JSP_ADD_GLOBAL_FUNCTION(tmGetCurms, tmGetCurms);
        JSP_ADD_GLOBAL_FUNCTION(reloadJS, reloadCurJSThread);
        JSP_ADD_GLOBAL_FUNCTION(simAndroidDeviceLosted, simAndroidDeviceLosted);
        JSP_ADD_GLOBAL_FUNCTION(getExePath, getExePath);
        JSP_ADD_GLOBAL_FUNCTION(getInnerHeight, getInnerHeight);
        JSP_ADD_GLOBAL_FUNCTION(getInnerWidth, getInnerWidth);
        JSP_ADD_GLOBAL_PROPERTY(enableTouch, g_bEnableTouch);
        JSP_ADD_GLOBAL_FUNCTION(alert, JSAlert, const char*);
        JSP_ADD_GLOBAL_FUNCTION(print, JSPrint, const char*);
        JSP_ADD_GLOBAL_FUNCTION(evalJS, evalJS, const char*);
        JSP_ADD_GLOBAL_FUNCTION(PerfShow, PerfShow, int);
        JSP_ADD_GLOBAL_FUNCTION(PerfAddData, PerfAddData, int,int,float,float);
        JSP_ADD_GLOBAL_FUNCTION(PerfUpdateDt, PerfUpdateDt, int,float);
        JSP_ADD_GLOBAL_FUNCTION(readFileSync, readFileSync1, const char*, const char*);
        JSP_ADD_GLOBAL_FUNCTION(writeStrFileSync, writeStrFileSync, const char*, const char*);
        JSP_ADD_GLOBAL_FUNCTION(readTextAsset, readTextAsset, const char*);
        JSP_ADD_GLOBAL_FUNCTION(fs_exists, JSFileSystem::exists, const char*);
        JSP_ADD_GLOBAL_FUNCTION(fs_mkdir, JSFileSystem::mkdir, const char*);
        JSP_ADD_GLOBAL_FUNCTION(fs_rm, JSFileSystem::rm, const char*);
        JSP_ADD_GLOBAL_FUNCTION(fs_rmDir, JSFileSystem::rmDir);
        JSP_ADD_GLOBAL_FUNCTION(fs_rmDirSync, JSFileSystem::rmDirSync);
        JSP_ADD_GLOBAL_FUNCTION(fs_readdirSync, JSFileSystem::readdirSync);
        JSP_ADD_GLOBAL_FUNCTION(fs_lstatSync, JSFileSystem::lstatSync);
        JSP_ADD_GLOBAL_FUNCTION(fs_writeFileSync, JSFileSystem::JSWriteFileSync);
        JSP_ADD_GLOBAL_FUNCTION(decodeTemp, UrlDecode);//以后实现各和JS一样的这个就可以删了
        JSP_ADD_GLOBAL_FUNCTION(showAlertOnJsException, showAlertOnJsException,bool);
        //这个返回的是ArrayBuffer接口
        JSP_ADD_GLOBAL_FUNCTION(fs_readFileSync, JSFileSystem::readBinFileSync, const char*);
        JSP_ADD_GLOBAL_FUNCTION(downloadBigFile, _downloadBigFile);
        JSP_ADD_GLOBAL_FUNCTION(downloadGetHeader, _downloadGetHeader);
        JSP_ADD_GLOBAL_FUNCTION(calcmd5, calcMD5_JSAB);
	}
}
//------------------------------------------------------------------------------

//-----------------------------END FILE--------------------------------
