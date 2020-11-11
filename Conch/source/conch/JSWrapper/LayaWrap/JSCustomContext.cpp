/**
@file			JSCustomContext.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_17
*/

#include "JSCustomContext.h"
#include "JSMeasureTextInfo.h"
#include "../../JCScrpitRuntime.h"
#include "../../JCCmdDispatchManager.h"
#include "../../JCConchRender.h"
#include "../../JCConch.h"
#include <Html5Render/JCHtml5Context.h>

//------------------------------------------------------------------------------
namespace laya 
{
    ADDJSCLSINFO(JSCustomContext, JSObjNode);
    JSCustomContext::JSCustomContext()
    {
        JCCmdDispathManager* pCmdDispathManager = JCConch::s_pConchRender->m_pCmdDispathManager;
        m_nID = pCmdDispathManager->getCmdDispathID();
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_createHtml5Context);
        pRenderCmd->append(m_nID);

        //设置type为 CANVAS_TYPE_MEMORY
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_setContextType);
        pRenderCmd->append(CANVAS_TYPE_MEMORY);
        pRenderCmd->append(m_nID);
    }
    JSCustomContext::~JSCustomContext()
    {
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_deleteHtml5Context);
        pRenderCmd->append(m_nID);
    }
    void JSCustomContext::setSize(int w,int h)
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
    JsValue JSCustomContext::measureText(const char* sFont, const char* sText)
    {
        JSMeasureTextInfo * pMeasureTextInfo = new JSMeasureTextInfo();
        if (sText != NULL)
        {
            JCFontInfo * pFontInfo = JCScriptRuntime::s_JSRT->m_pFontManager->getFontInfoFromText(sFont);
            pMeasureTextInfo->measureText(pFontInfo, sText);
        }
        return JSP_TO_JS(JSMeasureTextInfo, pMeasureTextInfo);
    }
    int JSCustomContext::getID()
    {
        return m_nID;
    }
    void JSCustomContext::exportJS()
    {
        JSP_CLASS("customContext", JSCustomContext);
        JSP_ADD_PROPERTY_RO(conchID, JSCustomContext, getID);
        JSP_ADD_METHOD("setSize", JSCustomContext::setSize );
        JSP_ADD_METHOD("measureText", JSCustomContext::measureText);
        JSP_INSTALL_CLASS("customContext", JSCustomContext);
    }

}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
