/**
@file			JCAtlasManager.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_26
*/

#include "JCAtlasManager.h"
#include "../../util/Log.h"
#include <cmath>
namespace laya
{
    JCAtlasManager::JCAtlasManager( int nWidth, int nHeight, float nGridSize,int nMaxTexNum )
    {
	    m_bSetAtlasParam=true;
	    m_nWidth = nWidth;
	    m_nHeight = nHeight;
	    m_nGridSize = nGridSize;
	    m_nMaxAtlasNum = nMaxTexNum;
	    m_nGridNumX = (int)(nWidth / nGridSize );
	    m_nGridNumY = (int)(nHeight / nGridSize );
	    m_nCurAtlasIndex = 0;
	    m_bNeedUploadToGPU = false;
	    Initialize();
    }
    JCAtlasManager::~JCAtlasManager()
    {
	    for( std::vector<JCAtlas*>::iterator iter = m_vAtlasArray.begin(); iter != m_vAtlasArray.end(); iter++ )
        {
		    JCAtlas* pAtlas = *iter;
		    if( pAtlas )
            {
			    delete pAtlas; 
			    pAtlas = NULL;
		    }
	    }
	    m_vAtlasArray.clear();
    }
    bool JCAtlasManager::setAtlasParam( int nWidth, int nHeight, float nGridSize,int nMaxTexNum )
    {
	    if( m_bSetAtlasParam == true )
	    {
		    for( std::vector<JCAtlas*>::iterator iter = m_vAtlasArray.begin(); iter != m_vAtlasArray.end(); iter++ )
		    {
                JCAtlas* pAtlas = *iter;
			    if( pAtlas )
			    {
				    delete pAtlas; pAtlas = NULL;
			    }
		    }
		    m_vAtlasArray.clear();
		    m_nWidth = nWidth;
		    m_nHeight = nHeight;
		    m_nGridSize = nGridSize;
		    m_nMaxAtlasNum = nMaxTexNum;
		    m_nGridNumX = (int)(nWidth / nGridSize );
		    m_nGridNumY = (int)(nHeight / nGridSize );
		    m_nCurAtlasIndex = 0;
		    m_bNeedUploadToGPU = false;
		    Initialize();
		    return true;
	    }
	    else
	    {
		    LOGE("设置大图合集参数错误，只能在开始页面设置各种参数");
		    return false;
	    }
	    return false;
    }
    bool JCAtlasManager::Initialize()
    {
	    m_vAtlasArray.clear();
	    for( int i = 0; i < m_nMaxAtlasNum; i++ )
	    {
		    m_vAtlasArray.push_back(  new JCAtlas( m_nGridNumX,m_nGridNumY,m_nWidth,m_nHeight ) );
	    }
	    return true;
    }
    void JCAtlasManager::setInAltasResValues( JCInAtlasRes* pInAltasRes,int nOffsetX,int nOffsetY,int nAtlasNum, BitmapData* pBitmapData)
    {
        pInAltasRes->setRect( nOffsetX,nOffsetY,pBitmapData->m_nWidth,pBitmapData->m_nHeight );
        m_vAtlasArray[nAtlasNum]->pushInAtlasRes(pInAltasRes);
        pInAltasRes->setAtlas(m_vAtlasArray[nAtlasNum]);
        pInAltasRes->mergeBitmapDataToTexture(pBitmapData, BOARDER_TYPE_ALL, nOffsetX, nOffsetY);
    }
    bool JCAtlasManager::pushData( JCInAtlasRes* pInAltasRes, BitmapData* pBitmapData )
    {
        if (!pBitmapData) return false;
	    m_bSetAtlasParam = false;
	    bool bFound = false;
	    m_bNeedUploadToGPU = true;
	    int nImageGridX = (int)(ceil( ( pBitmapData->m_nWidth + 2 ) / m_nGridSize ));
	    int nImageGridY = (int)(ceil( ( pBitmapData->m_nHeight + 2 ) / m_nGridSize ));
	    bool bSuccess = false;
	    MergeFillInfo kFillInfo;
	    //这个for循环是为了 如果 贴图满了，再创建一张，继续放置
	    for ( int k = 0; k < 2 ; k++ )
	    {
		    int nAtlasSize = m_vAtlasArray.size();
		    for( int i = 0; i < nAtlasSize; ++i )
		    {
			    int nAltasNum = ( m_nCurAtlasIndex + i ) % nAtlasSize;
			    bool bRet = m_vAtlasArray[nAltasNum]->AddTex( 1,nImageGridX,nImageGridY,kFillInfo ); 
			    if( bRet )
			    {
				    setInAltasResValues( pInAltasRes, (int)(kFillInfo.m_nX * m_nGridSize), (int)(kFillInfo.m_nY * m_nGridSize), nAltasNum,pBitmapData );
				    bSuccess = true;
				    m_nCurAtlasIndex = nAltasNum;
				    break;
			    }
		    }
		    if (bSuccess) break;
		    m_vAtlasArray.push_back(  new JCAtlas( m_nGridNumX,m_nGridNumY,m_nWidth,m_nHeight ) );
		    m_nCurAtlasIndex = m_vAtlasArray.size()-1;
	    }
	    if (!bSuccess)
	    {
	    }
	    return bSuccess;
    }
    bool JCAtlasManager::toGarbageCollection()
    {
	    int n = m_vAtlasArray.size() - m_nMaxAtlasNum;
        if (n > 0)
        {
            LOGI(">>>>>>>>>>>>>JCAtlasManager::toGarbageCollection n=%d",n);
            int i = 0;
            std::vector<JCAtlas*>::iterator iter = m_vAtlasArray.begin();
            while (i < n)
            {
                JCAtlas* pAtlas = *iter;
                if (pAtlas)
                {
                    delete pAtlas;
                    pAtlas = NULL;
                }
                iter = m_vAtlasArray.erase(iter);
                i++;
            }
        }
	    return false;
    }
    void JCAtlasManager::freeGLResource()
    {
	    m_bSetAtlasParam = true;
	    int iSz = m_vAtlasArray.size();
	    for ( int i = 0; i < iSz; ++i )
	    {
            if( m_vAtlasArray[i]->m_pTexture )
            {
                m_vAtlasArray[i]->m_pTexture->freeGLResource();
            }
		    m_vAtlasArray[i]->clear();
		    m_vAtlasArray[i]->reset();
	    }
    }
    void JCAtlasManager::resetAtlas()
    {
        m_bSetAtlasParam = true;
        int iSz = m_vAtlasArray.size();
        for ( int i = 0; i < iSz; ++i )
        {
            m_vAtlasArray[i]->clear();
            m_vAtlasArray[i]->reset();
        }
    }
}

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
