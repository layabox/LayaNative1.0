#include "JCShaderManager.h"
#include <3DRS/JCGpuProgram.h>

namespace laya
{

    JCShaderManager::ShaderContent::~ShaderContent()
    {
        if (pProg) {
            delete pProg;
            pProg = nullptr;
        }
    }

    JCShaderManager::JCShaderManager()
    {

    }

    JCShaderManager::~JCShaderManager()
    {
        clear();
    }

    void JCShaderManager::clear()
    {
        MapShader::iterator it = m_shaderMap.begin();
        MapShader::iterator ed = m_shaderMap.end();
        while (it != ed) {
            delete (*it).second;
            it++;
        }
        m_shaderMap.clear();
    }

    void JCShaderManager::createOrUpdateShader(int id,char* ps ,char *vs)
    {
        ShaderContent* pShader = getShaderById(id);
        if (pShader == NULL)
        {
            pShader = new ShaderContent();
            m_shaderMap[id] = pShader;
        }
        pShader->ps = ps;
        pShader->vs = vs;
    }

    JCShaderManager::ShaderContent* JCShaderManager::getShaderById(int id)
    {
        MapShader::iterator it=m_shaderMap.find(id);
        if (it == m_shaderMap.end()) {
            return NULL;
        }
        return (*it).second;
    }

    void JCShaderManager::invalidGLRes() {
        for (auto sh : m_shaderMap) {
            ShaderContent* pSh = sh.second;
            if (pSh->pProg) {
                pSh->pProg->freeGLResource();
            }
        }
    }
}
