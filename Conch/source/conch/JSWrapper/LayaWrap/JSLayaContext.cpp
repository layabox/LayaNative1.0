/**
@file			JSLayaContext.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_25
*/

//包含头文件
#include "JSLayaContext.h"
#include "JSMeasureTextInfo.h"
#include "../../JCScrpitRuntime.h"
#include "../../JCCmdDispatchManager.h"
#include "../../JCConchRender.h"
#include "../../JCConch.h"

//------------------------------------------------------------------------------
namespace laya 
{
    std::map<int, JSLayaContext*> JSLayaContext::m_sMapContexts;
    ADDJSCLSINFO(JSLayaContext, JSObjNode);
    JSLayaContext::JSLayaContext()
    {
        //预估内部变量 101 * 8 + 400 + 1024 * 4 + 1024
        AdjustAmountOfExternalAllocatedMemory(4096+2048);
        JCCmdDispathManager* pCmdDispathManager = JCConch::s_pConchRender->m_pCmdDispathManager;
        m_nID = JCScriptRuntime::s_JSRT->getDispathGlobalID();
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_createHtml5Context);
        pRenderCmd->append(m_nID);
        JCMemorySurvey::GetInstance()->newClass("context2D", 4096 + 2048, this);
        m_sMapContexts.insert(std::make_pair(m_nID, this));
        m_pGetPixelsCallBackFunction.Reset();
        m_pToBase64CallBackFunction.Reset();
    }
    JSLayaContext::~JSLayaContext() 
    {
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pGCCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_deleteHtml5Context);
        pRenderCmd->append(m_nID);
        JCMemorySurvey::GetInstance()->releaseClass("context2D", this);
        std::map<int, JSLayaContext*>::iterator it = m_sMapContexts.find(m_nID);
        if (it != m_sMapContexts.end())
        {
            m_sMapContexts.erase(it);
        }
    }
    void JSLayaContext::setSize(int w,int h)
    {
        m_nWidth = w;
        m_nHeight = h;
        //压流通知render线程
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_setHtml5CanvasSize);
        pRenderCmd->append(m_nID);
        pRenderCmd->append(m_nWidth);
        pRenderCmd->append(m_nHeight);
    }


    JsValue JSLayaContext::measureText(const char* sFont, const char* sText)
    {
        JSMeasureTextInfo * pMeasureTextInfo = new JSMeasureTextInfo();
        if (sText != NULL)
        {
            JCFontInfo * pFontInfo = JCScriptRuntime::s_JSRT->m_pFontManager->getFontInfoFromText(sFont);
            pMeasureTextInfo->measureText(pFontInfo, sText);
        }
        return JSP_TO_JS(JSMeasureTextInfo, pMeasureTextInfo);
    }
    void JSLayaContext::setGetPixelsCallback(JSValueAsParam p_pCallback)
    {
        m_pGetPixelsCallBackFunction.set(getpixelsid, this, p_pCallback);
    }
    void JSLayaContext::onGetPixelsCallbackCallJS(unsigned char *pPixels, int nLength, int w, int h)
    {
        if (m_pGetPixelsCallBackFunction.isValid()) {
            JsValue ab = createJSAB((char*)pPixels, nLength);
            m_pGetPixelsCallBackFunction.Call(ab, w, h);
        }
    }
    void JSLayaContext::setToBase64Callback(JSValueAsParam p_pCallback)
    {
        m_pToBase64CallBackFunction.set(tobase64id, this, p_pCallback);
    }
    void JSLayaContext::onToBase64CallbackCallJS(const char* pBase64)
    {
        if (m_pToBase64CallBackFunction.isValid()) {
            m_pToBase64CallBackFunction.Call(pBase64);
            delete[] pBase64;
        }
    }
    int JSLayaContext::getID()
    {
        return m_nID;
    }
    void JSLayaContext::exportJS()
    {
        JSP_CLASS("context", JSLayaContext);
        JSP_ADD_PROPERTY_RO(conchID, JSLayaContext, getID);
        JSP_ADD_METHOD("setSize", JSLayaContext::setSize );
        JSP_ADD_METHOD("measureText", JSLayaContext::measureText);
        JSP_ADD_METHOD("setGetPixelsCallback", JSLayaContext::setGetPixelsCallback);
        JSP_ADD_METHOD("setToBase64Callback", JSLayaContext::setToBase64Callback);
        JSP_INSTALL_CLASS("context", JSLayaContext);
    }

}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
