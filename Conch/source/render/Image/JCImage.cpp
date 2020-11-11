/**
@file			JCImage.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#include "JCImage.h"
#include <resource/Atlas/JCAtlasManager.h>
#include <resource/JCResManager.h>
#include <util/Log.h>
#include "JCImageManager.h"
#include <util/JCCommonMethod.h>

namespace laya
{
    int JCImage::s_nMisoperationWarningTime = 5000;
    int JCImage::s_nPushAtlasLimitSize = 512;
    //------------------------------------------------------------------------------
    JCImage::JCImage()
    {
        m_pAltasManager = NULL;
        m_pTextureManager = NULL;
        m_pDisplayRes = NULL;
        m_pFileResManager = NULL;
        m_pImageManager = NULL;
        m_nID = 0;
        m_nTouchTime = 0;
        m_bPushBitmapData = false;
        m_bEnableMerageInAtlas = true;
        m_nReleaseTime = 0;
    }
    JCImage::~JCImage()
    {
        if (m_pDisplayRes)
        {
            if (m_pDisplayRes->isInAtlas())
            {
                JCInAtlasRes* pInAtlasRes = (JCInAtlasRes*)m_pDisplayRes;
                if(pInAtlasRes->m_pAtlas)
                {
                    pInAtlasRes->m_pAtlas->removeInAtlasRes(pInAtlasRes);
                }
            }
            delete m_pDisplayRes;
            m_pDisplayRes = NULL;
        }
        m_kBitmapData.releaseData();
    }
    void JCImage::releaseTexture()
    {
        if (m_pDisplayRes)
        {
            if (m_pDisplayRes->isInAtlas() == false)
            {
                JCTexture* pTexture = (JCTexture*)m_pDisplayRes;
                pTexture->m_pResManager->freeRes(pTexture,false);
            }
        }
        m_kBitmapData.releaseData();
        m_nReleaseTime = tmGetCurms();
    }
    int JCImage::getWidth()
    {
        return m_kBitmapData.m_nWidth;
    }
    int JCImage::getHeight()
    {
        return m_kBitmapData.m_nHeight;
    }
    int JCImage::getFormat()
    {
        return m_kBitmapData.m_nFormat;
    }
    char* JCImage::getImageData()
    {
        return m_kBitmapData.m_pImageData;
    }
    void JCImage::setImageID(int nID)
    {
        m_nID = nID;
    }
    void JCImage::setManager(JCAtlasManager* pAltasManager, JCResManager* pTextureManager,JCFileResManager* pFileResManager, JCImageManager* pImageManager)
    {
        m_pAltasManager = pAltasManager;
        m_pTextureManager = pTextureManager;
        m_pFileResManager = pFileResManager;
        m_pImageManager = pImageManager;
    }
    ENABLE_TYPE JCImage::enable()
    {
        if (m_pDisplayRes==NULL)
        {
            int nImageWidth = getWidth();
            int nImageHeight = getHeight();
            if ( m_kBitmapData.m_bIsCompressed || m_pAltasManager == NULL || m_bEnableMerageInAtlas == false || nImageWidth >= s_nPushAtlasLimitSize || nImageHeight >= s_nPushAtlasLimitSize)
            {
                m_pDisplayRes = new JCTexture( m_pTextureManager,TEXTURE_TYPE_NORMAL, m_kBitmapData.m_nTextureWidth, m_kBitmapData.m_nTextureHeight,getFormat());
                m_pDisplayRes->setRestoreFunction( std::bind( &JCImage::gpuRestoreRes,this,m_pDisplayRes ) );
            }
            else
            {
                m_pDisplayRes = new JCInAtlasRes();
                m_pDisplayRes->setRestoreFunction(std::bind(&JCImage::gpuRestoreRes, this, m_pDisplayRes));
            }
        }
        return m_pDisplayRes->enable();
    }
    bool JCImage::enableBitmapData()
    {
        if (m_kBitmapData.m_pImageData == NULL)
        {
            if (m_pFileResManager)
            {
                JCFileRes* pRes = m_pFileResManager->getRes(m_sUrl);
                JCBuffer buf;
                if (pRes && pRes->loadFromCache(buf, false))
                {
                    if (m_bPushBitmapData == false)
                    {
                        if (loadImageMemSync(buf.m_pPtr, buf.m_nLen, m_kBitmapData) == false)
                        {
                            LOGE("JCImage::gpuRestoreRes decode image error url=%s", m_sUrl.c_str());
                            m_kBitmapData.releaseData();
                            return false;
                        }
                    }
                    else
                    {
                        int nImageLenght = m_kBitmapData.m_nWidth * m_kBitmapData.m_nHeight * 4;
                        if (buf.m_nLen == nImageLenght)
                        {
                            m_kBitmapData.m_pImageData = new char[nImageLenght];
                            memcpy(m_kBitmapData.m_pImageData, buf.m_pPtr, nImageLenght);
                        }
                        else
                        {
                            LOGE("JCImage::gpuRestoreRes image lenght != buffer.lenght");
                            return false;
                        }
                    }
                }
                else
                {
                    LOGE("JCImage::gpuRestoreRes load file error url=%s", m_sUrl.c_str());
                    m_kBitmapData.releaseData();
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    bool JCImage::gpuRestoreRes(JCDisplayRes* pDisplayRes)
    {
        if (tmGetCurms() - m_nReleaseTime < s_nMisoperationWarningTime)
        {
            LOGW("This image has just been released and needs to be restored now. Url=%s",m_sUrl.c_str());
        }
        if (enableBitmapData() == false)
        {
            return false;
        }
        int nImageWidth = getWidth();
        int nImageHeight = getHeight();
        if (m_pDisplayRes->isInAtlas())
        {
            m_pAltasManager->pushData( (JCInAtlasRes*)m_pDisplayRes, &m_kBitmapData );
        }
        else
        {
            int nBorderType = BOARDER_TYPE_NO;
            JCTexture* pTexture = (JCTexture*)m_pDisplayRes;
            if (pTexture->getTextureWidth() > nImageWidth)
            {
                nBorderType |= BOARDER_TYPE_RIGHT;
            }
            if (pTexture->getTextureHeight() > nImageHeight)
            {
                nBorderType |= BOARDER_TYPE_BOTTOM;
            }
            m_pDisplayRes->mergeBitmapDataToTexture( &m_kBitmapData, (BOARDER_TYPE)nBorderType, 0, 0);
            m_pDisplayRes->setRect(m_kBitmapData.m_nOffsetX, m_kBitmapData.m_nOffsetY, nImageWidth, nImageHeight);
        }
        m_pImageManager->removeImageFromMap(m_nID);
        m_kBitmapData.releaseData();
        return true;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
