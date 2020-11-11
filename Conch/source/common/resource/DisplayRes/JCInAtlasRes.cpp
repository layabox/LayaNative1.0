/**
@file			JCInAtlasRes.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_26
*/

#include "JCInAtlasRes.h"
#include "../Atlas/JCAtlas.h"

namespace laya
{
    JCInAtlasRes::JCInAtlasRes()
    {
        m_pAtlas = NULL;
    }
    JCInAtlasRes::~JCInAtlasRes()
    {
        m_pAtlas = NULL;
    }
    void JCInAtlasRes::setAtlas(JCAtlas* pAtlas)
    {
        m_pAtlas = pAtlas;
    }
    JCTexture* JCInAtlasRes::getTexture()
    {
        return m_pAtlas == NULL ? NULL : m_pAtlas->m_pTexture;
    }
    ENABLE_TYPE JCInAtlasRes::enable()
    {
        if (m_bNeedRefresh)
        {
            if( m_pRestoreFunction )
            {
                if( m_pRestoreFunction(this) )
                {
                    m_bNeedRefresh = false;
                    return ENALBE_TYPE_RESOTRE_SUCCESS;
                }
                else
                {
                    return ENALBE_TYPE_FAIL;
                }
            }
            else
            {
                return ENALBE_TYPE_FAIL;
            }
        }
        else
        {
            return ENABLE_TYPE_SUCCESS;
        }
    }
    bool JCInAtlasRes::mergeBitmapDataToTexture(BitmapData* pBitmapData, BOARDER_TYPE nBoarderFlag, int x, int y)
    {
        return m_pAtlas->mergeBitmapData(pBitmapData, nBoarderFlag, x - 1, y - 1);
    }
    bool JCInAtlasRes::isInAtlas()
    {
        return true;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
