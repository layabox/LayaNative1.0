/**
@file			JCGpuProgram.cpp
@brief			
@author			guo
@version		1.0
@date			2016_5_31
*/

#include "JCGpuProgram.h"
#include "../util/Log.h"
#include <stdio.h>
#include "JCNamedData.h"

namespace laya{

    JCGpuProgramTemplate::JCGpuProgramTemplate(const char* p_pszVS, const char* p_pszPS) {
        setSrc(p_pszVS, p_pszPS);
    }
    JCGpuProgramTemplate::~JCGpuProgramTemplate() {
        for (auto& v : m_ShaderCache) {
            delete v.second;
        }
        for (std::map<std::string, UniformInfo*>::iterator iter = m_vUniformSemantics.begin(); iter != m_vUniformSemantics.end(); iter++)
        {
            UniformInfo* pInfo = iter->second;
            if (pInfo)
            {
                delete pInfo;
                pInfo = NULL;
            }
        }
        m_vUniformSemantics.clear();
    }
    void JCGpuProgramTemplate::setSrc(const char* p_pszVS, const char* p_pszPS) {
        if (!p_pszPS || !p_pszVS)
            return;
        m_strVS = p_pszVS;
        m_strPS = p_pszPS;
    }
    bool JCGpuProgram::checkCompile(int p_nShader, char* desc) {
        GLint compiled = 0;
        glGetShaderiv(p_nShader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(p_nShader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = new char[infoLen];
                if (buf) {
                    glGetShaderInfoLog(p_nShader, infoLen, NULL, buf);
                    LOGE("JCGpuProgram::checkCompile Could not compile shader %s:\n%s\n", desc ? desc : "", buf);
                    delete[] buf;
                }
                glDeleteShader(p_nShader);
            }
            return false;
        }
        return true;
    }
    JCGpuProgram* JCGpuProgramTemplate::getInst(const char* p_Macro, int p_nKey) {
        //先查找
        auto it = m_ShaderCache.find(p_nKey);
        if (it != m_ShaderCache.end()) {
            return (*it).second;
        }
        JCGpuProgram* pRet = new JCGpuProgram();
        pRet->m_strMacro = p_Macro ? p_Macro : "";
        m_ShaderCache[p_nKey] = pRet;
        pRet->m_pTemp = this;
        return pRet;
    }
    void JCGpuProgramTemplate::freeGLResource() {
        for (auto& v : m_ShaderCache) {
            (v.second)->freeGLResource();
        }
    }
    const char* JCGpuProgram::shader_SamplerPre = "g_Tex";            //例如 g_Tex0表示第一个纹理
    const char* JCGpuProgram::shader_CubeSamplerPre = "g_CubeTex";    //例如 g_Tex0表示第一个纹理
    JCGpuProgram::JCGpuProgram(){
        m_nVS = m_nPS = m_nProgram = 0;
    }
    JCGpuProgram::~JCGpuProgram(){
        freeGLResource();
        if (m_pUniformDesc)
            delete m_pUniformDesc;
    }
    void JCGpuProgram::useProgram(){
        if( m_nProgram <= 0 ){
            compile();
            if(m_nProgram<=0)
                return;
        }
        glUseProgram( m_nProgram );
    }
    void JCGpuProgram::freeGLResource(){
        if( m_nVS>0 ){
            glDeleteShader(m_nVS);
            m_nVS = 0;
        }
        if( m_nPS ){
            glDeleteShader( m_nPS);
            m_nPS = 0;
        }
        glDeleteProgram( m_nProgram );
        m_nProgram = 0;
    }
    bool JCGpuProgramTemplate::splitKeyValue(const char* pSems, std::map<std::string, std::string>& out) {
        char strVarName[64];
        const char* pCur = pSems;
        char* pDes = strVarName;
        std::string* pSemNameInMap = nullptr;
        while (true) {
            if (*pCur != ',' && *pCur != 0) {
                *pDes++ = *pCur;
            }
            else {
                *pDes = 0;
                if (pSemNameInMap == nullptr)
                    pSemNameInMap = &(out[strVarName]);
                else {
                    *pSemNameInMap = strVarName;
                    pSemNameInMap = nullptr;
                }
                pDes = strVarName;
                if (*pCur == 0)
                    break;
            }
            pCur++;
        }
        return true;
    }
    void JCGpuProgramTemplate::addAttrSemantics(int nSemanticsEnum,const char* sName)
    {
        m_vAttrSemantics[sName] = nSemanticsEnum;
    }
    void JCGpuProgramTemplate::addUniformSemantics(UNIFORM_PERIOD nType, int nSemanticsEnum, const char* sName)
    {
        std::map<std::string, UniformInfo*>::iterator iter = m_vUniformSemantics.find(sName);
        if (iter != m_vUniformSemantics.end())
        {
            UniformInfo* pInfo = iter->second;
            pInfo->m_nType = nType;
            pInfo->m_nSemanticsEnum = nSemanticsEnum;
        }
        else
        {
            m_vUniformSemantics[sName] = new UniformInfo(nType, nSemanticsEnum);
        }
    }
    int JCGpuProgramTemplate::getAttrSemanticsName(const char* sName)
    {
        std::map<std::string, int>::iterator iter = m_vAttrSemantics.find(sName);
        if (iter != m_vAttrSemantics.end())
        {
            return iter->second;
        }
        return -1;
    }
    JCGpuProgramTemplate::UniformInfo* JCGpuProgramTemplate::getUniformInfo(const char* pVarName)
    {
        std::map<std::string, UniformInfo*>::iterator iter = m_vUniformSemantics.find(pVarName);
        if (iter != m_vUniformSemantics.end())
        {
            return iter->second;
        }
        return NULL;
    }
    bool JCGpuProgram::compile() {
        int nGLVS, nGLPS;
        nGLVS = glCreateShader(GL_VERTEX_SHADER);
        const char* strShaders[2] = { m_strMacro.c_str(), m_pTemp->m_strVS.c_str() };
        glShaderSource(nGLVS, 2, strShaders, NULL);
        glCompileShader(nGLVS);
        if (!checkCompile(nGLVS, "VS"))
            return nullptr;

        nGLPS = glCreateShader(GL_FRAGMENT_SHADER);
        strShaders[1] = m_pTemp->m_strPS.c_str();
        glShaderSource(nGLPS, 2, strShaders, NULL);
        glCompileShader(nGLPS);
        if (!checkCompile(nGLPS, "PS"))
            return nullptr;

        int nProgram = glCreateProgram();
        glAttachShader(nProgram, nGLVS);
        glAttachShader(nProgram, nGLPS);
        glLinkProgram(nProgram);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(nProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(nProgram, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = new char[bufLength];
                if (buf) {
                    glGetProgramInfoLog(nProgram, bufLength, NULL, buf);
                    LOGE("JCGpuProgram::compile Could not link program:\n%s\n", buf);
                    delete[] buf;
                }
            }
            glDeleteProgram(nProgram);
            nProgram = 0;
        }
        m_nVS = nGLVS;
        m_nPS = nGLPS;
        m_nProgram = nProgram;
        return true;
    }
    GLuint  JCGpuProgram::getGpuProgram() {
        if (m_nProgram > 0)return m_nProgram;
        compile();
        return m_nProgram;
    }
}

