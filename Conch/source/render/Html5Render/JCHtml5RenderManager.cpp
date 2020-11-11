/**
@file			JCHtml5RenderManager.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_1
*/

#include "JCHtml5RenderManager.h"
#include "JCHtml5Render.h"

namespace laya
{
    JCHtml5RenderManager::JCHtml5RenderManager()
    {
        
    }
    JCHtml5RenderManager::~JCHtml5RenderManager()
    {
        clearAll();
    }
    void JCHtml5RenderManager::clearAll()
    {
        m_vHtml5Renders.clear();
    }
    void JCHtml5RenderManager::pushHtml5Render(JCHtml5Render* pHtml5Render)
    {
        m_vHtml5Renders[pHtml5Render->m_nID] = pHtml5Render;
    }
    bool JCHtml5RenderManager::removeHtml5Render( int nID )
    {
        MapHtml5Render::iterator iter = m_vHtml5Renders.find(nID);
        if (iter != m_vHtml5Renders.end())
        {
            iter = m_vHtml5Renders.erase(iter);
            return true;
        }
        return false;
    }
    JCHtml5Render* JCHtml5RenderManager::getHtml5Render(int nID)
    {
        MapHtml5Render::iterator iter = m_vHtml5Renders.find(nID);
        if (iter != m_vHtml5Renders.end())
        {
            return iter->second;
        }
        return NULL;
    }
    bool JCHtml5RenderManager::invalidGLRes()
    {
        for (MapHtml5Render::iterator iter = m_vHtml5Renders.begin(); iter != m_vHtml5Renders.end(); iter++)
        {
            JCHtml5Render* pHtml5Render = iter->second;
            if (pHtml5Render)
            {
                pHtml5Render->invalidGLRes();
            }
        }
        return true;
    }
//------------------------------------------------------------------------------
}

//-----------------------------END FILE--------------------------------
