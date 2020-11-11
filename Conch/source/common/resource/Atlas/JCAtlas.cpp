/**
@file			JCAtlas.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_26
*/

#include "JCAtlas.h"
#include "../../util/Log.h"

namespace laya
{
	JCAtlas::JCAtlas(int nGridNumX, int nGridNumY, int nWidth, int nHeight )
		:JCAtlasGrid(nGridNumX, nGridNumY)
	{
		m_pTexture = new JCTexture( NULL, TEXTURE_TYPE_ATLAS,nWidth,nHeight,GL_RGBA,this );
        //m_pTexture->m_nMaxFiter = m_pTexture->m_nMinFiter = GL_NEAREST;
	}
	JCAtlas::~JCAtlas()
	{
		destroy();
	}
	bool JCAtlas::mergeBitmapData( BitmapData* pBitmapData, BOARDER_TYPE nBoarderFlag,int nOffsetX, int nOffsetY )
	{
		return m_pTexture->mergeBitmapDataToAtlas(pBitmapData, nBoarderFlag, nOffsetX, nOffsetY );
	}
	void JCAtlas::pushInAtlasRes( JCInAtlasRes* pInAtlasRes )
	{
		m_vInAtlasRes.push_back(pInAtlasRes);
	}
	bool JCAtlas::removeInAtlasRes( JCInAtlasRes* pInAtlasRes )
	{
		if( m_vInAtlasRes.size() <= 0 ) return true;
		for( std::vector<JCInAtlasRes*>::iterator iter = m_vInAtlasRes.begin(); iter != m_vInAtlasRes.end(); )
		{
			if( *iter == pInAtlasRes)
			{
				iter = m_vInAtlasRes.erase( iter );
				return true;
			}
			else
			{
				iter++;
			}
		}
		LOGE("JCAtlas::removeInAtlasRes error");
		return false;
	}
	void JCAtlas::clear()
	{
		for (int i= 0, sz=m_vInAtlasRes.size(); i < sz; i++)
		{
            m_vInAtlasRes[i]->m_bNeedRefresh = true;
            m_vInAtlasRes[i]->setAtlas(NULL);
		}
		m_vInAtlasRes.clear();
        //调用回调函数
        for (MapFunctions::iterator iter = m_vMapFunctions.begin(); iter != m_vMapFunctions.end(); iter++)
        {
            iter->second( this );
        }
        m_vMapFunctions.clear();
	}
	void JCAtlas::destroy()
	{
		clear();
		if( m_pTexture )
		{
			delete m_pTexture;
			m_pTexture = NULL;
		}
	}
    void JCAtlas::setFunction(int nIndex, std::function<void(JCAtlas*)> pFunction)
    {
        m_vMapFunctions[nIndex] = pFunction;
    }
    void JCAtlas::removeFunction(int nIndex)
    {
        MapFunctions::iterator iter = m_vMapFunctions.find(nIndex);
        if (iter != m_vMapFunctions.end())
        {
            iter = m_vMapFunctions.erase( iter );
        }
    }
}

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
