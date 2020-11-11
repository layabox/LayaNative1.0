/**
@file			JSGraphics.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_14
*/

#include "JSGraphics.h"
#include "../../JCScrpitRuntime.h"
#include "../../JCCmdDispatchManager.h"
#include "../../JCConchRender.h"
#include "../../JCConch.h"

namespace laya
{
//------------------------------------------------------------------------------
    TEXT_BASE_LINE_TYPE JSGraphics::s_nTextBaseLineType = TBL_TYPE_TOP;
    ADDJSCLSINFO(JSGraphics, JSObjNode);
    JSGraphics::JSGraphics()
    {
        //大概估算内部变量 7个int + memClass 256 * 5
        AdjustAmountOfExternalAllocatedMemory( 1280 );
        JCCmdDispathManager* pCmdDispathManager = JCConch::s_pConchRender->m_pCmdDispathManager;
        m_nID = JCScriptRuntime::s_JSRT->getDispathGlobalID();
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_createGraphics);
        pRenderCmd->append(m_nID);
        pRenderCmd->append(s_nTextBaseLineType);
        JCMemorySurvey::GetInstance()->newClass("graphics", 1280, this);
    }
    JSGraphics::~JSGraphics()
    {
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pGCCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_deleteGraphics);
        pRenderCmd->append(m_nID);
        JCMemorySurvey::GetInstance()->releaseClass("graphics", this);
    }
    int JSGraphics::getID()
    {
        return m_nID;
    }
    void JSGraphics::exportJS()
    {
        JSP_CLASS("_conchGraphics", JSGraphics);
        JSP_ADD_PROPERTY_RO(conchID, JSGraphics, getID);
        JSP_INSTALL_CLASS("_conchGraphics", JSGraphics);
    }
//------------------------------------------------------------------------------
}

//-----------------------------END FILE--------------------------------