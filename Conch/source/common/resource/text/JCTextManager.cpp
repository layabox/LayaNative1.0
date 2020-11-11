/**
@file			JCTextManager.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_2
*/

#include "JCTextManager.h"
#include "../../util/Log.h"

//------------------------------------------------------------------------------
namespace laya
{
    JCTextManager::JCTextManager(JCAtlasManager* pAtlasManager)
    {
        m_pAtlasManager = pAtlasManager;
    }
    JCTextManager::~JCTextManager()
    {
        clearAll();
    }
    void JCTextManager::setFreeTypeRender(JCFreeTypeFontRender* pFreeTypeRender)
    {
        m_pFreeTypeRender = pFreeTypeRender;
    }
    void JCTextManager::clearAll()
    {
        for (MapWordRes::iterator iter = m_vWordRes.begin(); iter != m_vWordRes.end(); iter++)
        {
            JCWordRes* pWordRes = iter->second;
            if (pWordRes)
            {
                delete pWordRes;
                pWordRes = NULL;
            }
        }
        m_vWordRes.clear();
    }
    JCWordRes* JCTextManager::getWordRes(JCFontInfo* pFontInfo, int nColor, const char* sWord, float scaleX, float scaleY)
    {
        if (m_pFreeTypeRender == NULL)
        {
            LOGE("JCTextManager::getWordRes m_pFreeTypeRender == NULL");
            return NULL;
        }
        WordResKey key(pFontInfo->m_nFontId, nColor, sWord, pFontInfo->m_nFontSize * scaleX, pFontInfo->m_nFontSize * scaleY);
        MapWordRes::iterator iter = m_vWordRes.find(key);
        if (iter != m_vWordRes.end())
        {
            return iter->second;
        }
        else
        {
            JCWordRes* pWordRes = new JCWordRes(m_pAtlasManager, m_pFreeTypeRender, pFontInfo, nColor, sWord, scaleX, scaleY);
            m_vWordRes.insert(std::make_pair(key, pWordRes));
            return pWordRes;
        }
    }   
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
