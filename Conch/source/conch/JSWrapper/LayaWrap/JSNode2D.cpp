/**
@file			JSNode2D.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_13
*/

#include "JSNode2D.h"
#include "../../JCScrpitRuntime.h"
#include "../../JCCmdDispatchManager.h"
#include "../../JCConchRender.h"
#include "../../JCConch.h"

namespace laya
{
    ADDJSCLSINFO(JSNode2D, JSObjNode);
    JSNode2D::JSNode2D()
    {
        //大概估算内部变量 54个int
        AdjustAmountOfExternalAllocatedMemory( 1024 );
        JCCmdDispathManager* pCmdDispathManager = JCConch::s_pConchRender->m_pCmdDispathManager;
        m_nID = JCScriptRuntime::s_JSRT->getDispathGlobalID();
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_createNode);
        pRenderCmd->append(m_nID);
        JCMemorySurvey::GetInstance()->newClass("sprite2D", 1024, this);
    }
    JSNode2D::~JSNode2D()
    {
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pGCCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_deleteNode);
        pRenderCmd->append(m_nID);
        JCMemorySurvey::GetInstance()->releaseClass("sprite2D", this);
    }
    void JSNode2D::setRootNode()
    {
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_setRootNode);
        pRenderCmd->append(m_nID);
    }
    void JSNode2D::setFPSNode()
    {
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_setFPSNode);
        pRenderCmd->append(m_nID);
    }

    void JSNode2D::setLoadingViewNode()
    {
		JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_setLoadingViewNode);
        pRenderCmd->append(m_nID);
    }
    int JSNode2D::getID()
    {
        return m_nID;
    }
    void JSNode2D::exportJS()
    {
        JSP_CLASS("conchNode2D", JSNode2D);
        JSP_ADD_PROPERTY_RO(conchID, JSNode2D, getID);
        JSP_ADD_METHOD("setRootNode", JSNode2D::setRootNode);
        JSP_ADD_METHOD("setFPSNode", JSNode2D::setFPSNode);
        JSP_ADD_METHOD("setLoadingViewNode", JSNode2D::setLoadingViewNode);
        JSP_INSTALL_CLASS("conchNode2D", JSNode2D);
    }
}

//-----------------------------END FILE--------------------------------