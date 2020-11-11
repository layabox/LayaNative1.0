/**
@file			JSWebGLContext.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#include "JSWebGLContext.h"
#include "../../JCScrpitRuntime.h"
#include "../../JCCmdDispatchManager.h"
#include "../../JCConchRender.h"
#include "../../JCConch.h"
#include "JSShaderActiveInfo.h"

namespace laya
{
    ADDJSCLSINFO(JSWebGLContext, JSObjNode);
    JSWebGLContext::JSWebGLContext()
    {
        AdjustAmountOfExternalAllocatedMemory(256000*4);
        JCCmdDispathManager* pCmdDispathManager = JCConch::s_pConchRender->m_pCmdDispathManager;
        m_nID = JCScriptRuntime::s_JSRT->getDispathGlobalID();
        JCScriptRuntime::s_JSRT->flushSharedCmdBuffer();
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pRenderCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_createWebGLContext);
        pRenderCmd->append(m_nID);
        JCMemorySurvey::GetInstance()->newClass("webGLContext", 256000 * 4, this);
        m_nProgramParameter = 0;
        m_pShaderActiveInfo = new WebGLActiveInfo();
        m_pShaderTemplate = NULL;
        m_pGpuProgram = NULL;
    }
    JSWebGLContext::~JSWebGLContext()
    {
        JCMemClass* pRenderCmd = JCScriptRuntime::s_JSRT->m_pGCCmd;
        pRenderCmd->append(PUBLIC_CMD_DISPATH);
        pRenderCmd->append(PFT_deleteWebGLContext);
        pRenderCmd->append(m_nID);
        JCMemorySurvey::GetInstance()->releaseClass("webGLContext", this);
        if (m_pShaderActiveInfo)
        {
            delete m_pShaderActiveInfo;
            m_pShaderActiveInfo = NULL;
        }
        if (m_pShaderTemplate)
        {
            delete m_pShaderTemplate;
            m_pShaderTemplate = NULL;
            m_pGpuProgram = NULL;
        }
    }
    int JSWebGLContext::getID()
    {
        return m_nID;
    }
    void JSWebGLContext::setSize(int w, int h)
    {

    }
    int JSWebGLContext::getProgramParameterEx(const char* vs, const char* ps, const char* define, int type)
    {
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSWebGLContext::_getProgramParameterEx,this,vs,ps,define,type));
        return m_nProgramParameter;
    }
    JsValue JSWebGLContext::getActiveAttribEx(const char* vs, const char* ps, const char* define, int nIndex)
    {
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSWebGLContext::_getActiveAttribEx, this, vs, ps, define, nIndex));
        JSShaderActiveInfo * pActiveInfo = new JSShaderActiveInfo();
        pActiveInfo->m_sName = m_pShaderActiveInfo->name;
        pActiveInfo->m_nType = m_pShaderActiveInfo->type;
        pActiveInfo->m_nSize = m_pShaderActiveInfo->size;
        return JSP_TO_JS(JSShaderActiveInfo, pActiveInfo);
    }
    JsValue JSWebGLContext::getActiveUniformEx(const char* vs, const char* ps, const char* define, int nIndex)
    {
        JCConch::s_pConchRender->setInterruptFunc(std::bind(&JSWebGLContext::_getActiveUniformEx, this, vs, ps, define, nIndex));
        JSShaderActiveInfo * pActiveInfo = new JSShaderActiveInfo();
        pActiveInfo->m_sName = m_pShaderActiveInfo->name;
        pActiveInfo->m_nType = m_pShaderActiveInfo->type;
        pActiveInfo->m_nSize = m_pShaderActiveInfo->size;
        return JSP_TO_JS(JSShaderActiveInfo, pActiveInfo);
    }
    void JSWebGLContext::_getProgramParameterEx(const std::string& vs, const std::string& ps, const std::string& define, int type)
    {
        _createShader(vs,ps,define);
        ::glGetProgramiv(m_pGpuProgram->getGpuProgram(), type, &m_nProgramParameter);
    }
    void JSWebGLContext::_getActiveAttribEx(const std::string& vs, const std::string& ps, const std::string& define, int nIndex)
    {
        _createShader(vs, ps, define);
        ::glGetActiveAttrib(m_pGpuProgram->getGpuProgram(), nIndex, m_pShaderActiveInfo->bufsize, &m_pShaderActiveInfo->length, &m_pShaderActiveInfo->size, &m_pShaderActiveInfo->type, m_pShaderActiveInfo->name);
    }
    void JSWebGLContext::_getActiveUniformEx(const std::string& vs, const std::string& ps, const std::string& define, int nIndex)
    {
        _createShader(vs, ps, define);
        ::glGetActiveUniform(m_pGpuProgram->getGpuProgram(), nIndex, m_pShaderActiveInfo->bufsize, &m_pShaderActiveInfo->length, &m_pShaderActiveInfo->size, &m_pShaderActiveInfo->type, m_pShaderActiveInfo->name);
    }
    void JSWebGLContext::_createShader(const std::string& vs, const std::string& ps, const std::string& define)
    {
        if (m_sShaderVS != vs || m_sShaderPS != ps || m_sShaderDefine != define)
        {
            if (m_pShaderTemplate)
            {
                delete m_pShaderTemplate;
                m_pShaderTemplate = NULL;
                m_pGpuProgram = NULL;
            }
            m_sShaderVS = vs;
            m_sShaderPS = ps;
            m_sShaderDefine = define;

            //重新new新的shader
            m_pShaderTemplate = new JCGpuProgramTemplate(vs.c_str(), ps.c_str());
            m_pGpuProgram = m_pShaderTemplate->getInst(define.c_str(), 0);
        }
    }
    void JSWebGLContext::exportJS()
    {
        JSP_CLASS("WebGLContext", JSWebGLContext);
        JSP_ADD_PROPERTY_RO(conchID, JSWebGLContext, getID);
        JSP_ADD_METHOD("setSize", JSWebGLContext::setSize);
        JSP_ADD_METHOD("getProgramParameterEx", JSWebGLContext::getProgramParameterEx);
        JSP_ADD_METHOD("getActiveAttribEx", JSWebGLContext::getActiveAttribEx);
        JSP_ADD_METHOD("getActiveUniformEx", JSWebGLContext::getActiveUniformEx);
        JSP_INSTALL_CLASS("WebGLContext", JSWebGLContext);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------