/**
@file			JSArrayBufferRef.cpp
@brief			
@author			James
@version		1.0
@date			2017_11_29
*/

#include "JSArrayBufferRef.h"
#include "../../JCScrpitRuntime.h"
#include "buffer/JCBuffer.h"
#include "../../JCCmdDispatchManager.h"

namespace laya
{
    ADDJSCLSINFO(JSArrayBufferRef, JSObjNode);
    JSArrayBufferRef::JSArrayBufferRef()
    {
        m_nID = 0;
    }
    JSArrayBufferRef::~JSArrayBufferRef()
    {
        //从JS线程删除这个ID
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pGCCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_deleteVetexBuffer);
        pRenderCmd->append(m_nID);
    }
    int JSArrayBufferRef::getID()
    {
        return m_nID;
    }
    int JSArrayBufferRef::setID(int nID)
    {
        m_nID = nID;
        return m_nID;
    }
    void JSArrayBufferRef::exportJS()
    {
        JSP_CLASS("ArrayBufferRef", JSArrayBufferRef);
        JSP_ADD_PROPERTY(id, JSArrayBufferRef, getID, setID);
        JSP_INSTALL_CLASS("ArrayBufferRef", JSArrayBufferRef);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
