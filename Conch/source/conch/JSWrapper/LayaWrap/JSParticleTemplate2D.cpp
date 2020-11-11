/**
@file			JSParticleTemplate2D.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_10_31
*/


//------------------------------------------------------------------------------
//包含头文件
#include "JSParticleTemplate2D.h"
#include "../../JCCmdDispatchManager.h"
#include "../../JCConch.h"
#include "../../JCScrpitRuntime.h"

namespace laya 
{
	ADDJSCLSINFO(JSParticleTemplate2D, JSObjNode);
    //------------------------------------------------------------------------------
    JSParticleTemplate2D::JSParticleTemplate2D()
    {
	    AdjustAmountOfExternalAllocatedMemory( 20480 );
	    JCMemorySurvey::GetInstance()->newClass( "particleTemplate2D", 20480,this );
		JCCmdDispathManager* pCmdDispathManager = JCConch::s_pConchRender->m_pCmdDispathManager;
		m_nID = JCScriptRuntime::s_JSRT->getDispathGlobalID();
		JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
		JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
		pRenderCmd->append(PUBLIC_CMD_DISPATH);
		pRenderCmd->append(PFT_createParticalTemplate2D);
		pRenderCmd->append(m_nID);
    }
    //------------------------------------------------------------------------------
    JSParticleTemplate2D::~JSParticleTemplate2D()
    {
	    JCMemorySurvey::GetInstance()->releaseClass( "particleTemplate2D",this );
		JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pGCCmd;
		pRenderCmd->append(PUBLIC_CMD_DISPATH);
		pRenderCmd->append(PFT_deleteParticalTemplate2D);
		pRenderCmd->append(m_nID);
    }

    int JSParticleTemplate2D::getID()
    {
        return m_nID;
    }

    void JSParticleTemplate2D::exportJS()
    {
        JSP_CLASS("_conchParticleTemplate2D", JSParticleTemplate2D);
        JSP_ADD_PROPERTY_RO(conchID, JSParticleTemplate2D, getID);
        JSP_INSTALL_CLASS("_conchParticleTemplate2D", JSParticleTemplate2D);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------