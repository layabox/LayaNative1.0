/**
@file			JCMaterialManager.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_30
*/

#include "JCMaterialManager.h"

//-----------------------------------------------------------------------------
namespace laya
{
    JCMaterialManager::JCMaterialManager()
    {
        m_nCurrentPos = 0;
    }
    JCMaterialManager::~JCMaterialManager()
    {
        for (std::vector<JCMaterial*>::iterator iter = m_vMaterials.begin(); iter != m_vMaterials.end(); iter++)
        {
            JCMaterial* pMaterial = *iter;
            if (pMaterial)
            {
                delete pMaterial;
                pMaterial = NULL;
            }
        }
        m_vMaterials.clear();
    }
    JCMaterial* JCMaterialManager::getMaterial()
    {
        JCMaterial* pMaterial = NULL;
        if (m_nCurrentPos < m_vMaterials.size() )
        {
            pMaterial = m_vMaterials[m_nCurrentPos];
            m_nCurrentPos++;
        }
        else
        {
            pMaterial = new JCMaterial();
            m_vMaterials.push_back(pMaterial);
            m_nCurrentPos++;
        }
        return pMaterial;
    }
    void JCMaterialManager::reset()
    {
        m_nCurrentPos = 0;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------