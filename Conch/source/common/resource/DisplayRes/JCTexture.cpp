/**
@file			JCTexture.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_26
*/

#include "JCTexture.h"
#include "../../util/Log.h"
#include "../../util/JCCommonMethod.h"
#include "../../3DRS/JCRenderer.h"
#include <algorithm>

namespace laya
{
    int JCTexture::ms_nTextureID = 0;
    JCTexture::JCTexture(JCResManager* pResManager, TEXTURE_TYPE nType,int nWidth, int nHeight,int nFormat,JCAtlas* pAtlas)
	{
        m_nTextureID = ms_nTextureID++;
        m_pResManager = pResManager;
        m_bActive = false;
		m_pGpuTexture = 0;
        m_nTextureWidth = m_nTextureHeight = m_nWidth = m_nHeight = 0;
		m_nType = nType;
        m_pAtlas = pAtlas;
		addToTextureMap();
        setSize(nWidth, nHeight);
		m_bMipMap = false;
		m_nMinFiter= GL_LINEAR;
		m_nMaxFiter= GL_LINEAR;
        m_nFormat = nFormat;
	}
    JCTexture::~JCTexture()
	{
        freeGLResource();
        delFromTextureMap();
        m_bActive = false;
	}
    void JCTexture::freeRes(bool bNeedRestore )
    {
        m_nUsedMem = 0;
        m_bNeedRefresh = true;
        if (m_bActive)
        {
            setResSize(0);
            m_bActive = false;
        }
        if (m_pGpuTexture > 0)
        {
            freeGLResource();
        }
    }
    bool JCTexture::restoreRes()
    {
        /*if (m_pGpuTexture == 0)
        {
            createGpuTexture();
            allocVMem(m_nTextureWidth, m_nTextureHeight);
        }*/
        if ( m_pRestoreFunction )
        {
            m_bActive = m_pRestoreFunction(this);
        }
        else
        {
            m_bActive = true;
        }
        setState(ready);
        return m_bActive;
    }
    void JCTexture::touch(bool bRestoreRes)
    {
        JCResource::touch(bRestoreRes);
    }
    ENABLE_TYPE JCTexture::enable()
    {
        touch(false);
        if(m_bNeedRefresh)
        {
            if (restoreRes())
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
            return ENABLE_TYPE_SUCCESS;
        }
    }
    JCTexture* JCTexture::getTexture()
    {
        return this;
    }
    bool JCTexture::mergeBitmapDataToTexture(BitmapData* pBitmapData, BOARDER_TYPE nBoarderFlag, int nOffsetX, int nOffsetY)
    {
        if (!pBitmapData)
            return false;

        if (!pBitmapData->m_bIsCompressed)
        {
            return internalMergeTexture(pBitmapData, nBoarderFlag, nOffsetX, nOffsetY);
        }

        if (m_pGpuTexture != 0)
            return false;

        createGpuTexture();
        glBindTexture(GL_TEXTURE_2D, m_pGpuTexture);
        
        int width = pBitmapData->m_nTextureWidth;
        int height = pBitmapData->m_nTextureHeight;

        //int resSize = calcTextureSize(pBitmapData->m_nFormat, width, height);
        //etResSize(resSize);
        //TODO
        setResSize(width * height * 3);

        glCompressedTexImage2D(GL_TEXTURE_2D, 0, pBitmapData->m_nFormat, width, height, 0, (int)pBitmapData->m_nLength, (const GLvoid*)(pBitmapData->m_pImageData));

        if (m_bMipMap)
        {
            //不管有没有mipmap这个都应该设置啊，所以挪到外面。
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_nMaxFiter);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_nMinFiter);
            if(m_nMinFiter==GL_LINEAR)
                m_nMinFiter = GL_LINEAR_MIPMAP_LINEAR;
            if (m_nMinFiter == GL_NEAREST)
                m_nMinFiter = GL_NEAREST_MIPMAP_NEAREST;
           
        }

        if (m_bMipMap)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_nMaxFiter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_nMinFiter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_nSWrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_nTWrapMode);

        glBindTexture(GL_TEXTURE_2D, 0);
        return true;
    }
    void JCTexture::setSize(int nWidth, int nHeight)
    {
        if (m_nWidth == nWidth && m_nHeight == nHeight ) return;
        m_nWidth = nWidth;
        m_nHeight = nHeight;
        if (JCRenderer::s_nGLCaps & GLC_NOPT)
        {
            m_nTextureWidth = nWidth;
            m_nTextureHeight = nHeight;
        }
        else
        {
            m_nTextureWidth = (float)mathCeilPowerOfTwo(nWidth);
            m_nTextureHeight = (float)mathCeilPowerOfTwo(nHeight);
        }
        m_bNeedRefresh = true;
        //TODO 这个可能在非gl线程调用么。如果能，则直接删除会出错
        freeGLResource();
    }
    int JCTexture::getWidth()
    {
        return m_nWidth;
    }
    int JCTexture::getHeight()
    {
        return m_nHeight;
    }
    float JCTexture::getTextureWidth()
    {
        return m_nTextureWidth;
    }
    float JCTexture::getTextureHeight()
    {
        return m_nTextureHeight;
    }
    int JCTexture::getTextureID()
    {
        return m_nTextureID;
    }
    void JCTexture::createGpuTexture()
    {
        m_nCreateThread = std::this_thread::get_id();
        glGenTextures(1, &m_pGpuTexture);
    }
    void JCTexture::freeGLResource()
    {
        if (m_pGpuTexture > 0)
        {
            if (m_nCreateThread != std::this_thread::get_id())
            {
                LOGW("JCTexture::freeGLResource thread error");
            }
            glDeleteTextures(1, &m_pGpuTexture);
        }
        m_pGpuTexture = 0;
        m_bNeedRefresh = true;
    }
    bool JCTexture::internalMergeTexture(BitmapData* pBitmapData, BOARDER_TYPE nBoarderFlag, int nOffsetX, int nOffsetY)
    {
        if (!pBitmapData)return false;
        if (m_pGpuTexture == 0)
        {
            if (m_nTextureWidth <= 0 || m_nTextureHeight <= 0)
            {
                LOGE("JCTexture::internalMergeTexture texture size error:%d,%d", m_nTextureWidth, m_nTextureHeight);
                throw - 221;
            }
            if (pBitmapData->m_nWidth == m_nTextureWidth && pBitmapData->m_nHeight == m_nTextureHeight)
            {
                createGpuTexture();
                allocVMemWithData(pBitmapData);
                return true;
            }
            else
            {
                createGpuTexture();
                allocVMem(m_nTextureWidth, m_nTextureHeight);
            }
        }
        static int texcoll[2048];   //用来暂存图片的一列。注意 线程非安全，图片大小不允许超过2048
        static int texcolr[2048];
        if (nBoarderFlag)
        {
            glBindTexture(GL_TEXTURE_2D, m_pGpuTexture);
            //这个不应该在这里设置吧。使用外部设置的wrapmode
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

            //拷贝图片
            int xoff = nOffsetX + (nBoarderFlag&BOARDER_TYPE_LEFT ? 1 : 0);
            int yoff = nOffsetY + (nBoarderFlag&BOARDER_TYPE_TOP ? 1 : 0);

            glTexSubImage2D(GL_TEXTURE_2D, 0, xoff, yoff, pBitmapData->m_nWidth, pBitmapData->m_nHeight, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)pBitmapData->premultiplyAlphaData());

            //添加上下两个保护边。这两个可以直接调用texSubImage2D
            if (nBoarderFlag&BOARDER_TYPE_TOP)
            {
                if (nOffsetY >= 0)
                {
                    glTexSubImage2D(GL_TEXTURE_2D, 0, xoff, nOffsetY, pBitmapData->m_nWidth, 1, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)pBitmapData->premultiplyAlphaData());        //上边
                }
            }
            if (nBoarderFlag&BOARDER_TYPE_BOTTOM)
            {
                int nY = nOffsetY + pBitmapData->m_nHeight + (nBoarderFlag&BOARDER_TYPE_TOP ? 1 : 0);
                if (nY < m_nTextureHeight)
                {
                    glTexSubImage2D(GL_TEXTURE_2D, 0, xoff, nY, pBitmapData->m_nWidth
                        , 1, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)(pBitmapData->premultiplyAlphaData() + pBitmapData->m_nWidth*(pBitmapData->m_nHeight - 1) * 4)); //下边
                }
            }
            //添加左右两个保护边。这个无法直接texSubImage2D,只能新构造一个图形
            int* pdstl = (int*)texcoll;
            int* pdstr = (int*)texcolr;
            int* psrc = (int*)pBitmapData->premultiplyAlphaData();
            if (pBitmapData->m_nWidth <= 2048 && pBitmapData->m_nHeight <= 2048)
            {
                for (int y = 0; y<pBitmapData->m_nHeight; y++)
                {
                    *pdstl++ = *psrc;
                    *pdstr++ = *(psrc + pBitmapData->m_nWidth - 1);
                    psrc += pBitmapData->m_nWidth;
                }
                if (nBoarderFlag&BOARDER_TYPE_LEFT)
                {
                    if (nOffsetX >= 0)
                    {
                        glTexSubImage2D(GL_TEXTURE_2D, 0, nOffsetX, yoff, 1, pBitmapData->m_nHeight, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)texcoll);       //左边
                    }
                }
                if (nBoarderFlag&BOARDER_TYPE_RIGHT)
                {
                    int nX = nOffsetX + pBitmapData->m_nWidth + (nBoarderFlag&BOARDER_TYPE_LEFT ? 1 : 0);
                    if (nX < m_nTextureWidth)
                    {
                        glTexSubImage2D(GL_TEXTURE_2D, 0, nX, yoff
                            , 1, pBitmapData->m_nHeight, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)texcolr);   //右边
                    }
                }
            }
            else
            {
                LOGE("错误：图片大小超过2048，无法处理！！！");
                return false;
            }
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, m_pGpuTexture);
            int dx = (nOffsetX + pBitmapData->m_nWidth) - m_nTextureWidth;
            int dy = (nOffsetY + pBitmapData->m_nHeight) - m_nTextureHeight;
            if (dx <= 0 && dy <= 0)
            {
                glTexSubImage2D(GL_TEXTURE_2D, 0, nOffsetX, nOffsetY, pBitmapData->m_nWidth, pBitmapData->m_nHeight, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)pBitmapData->premultiplyAlphaData());
            }
            else
            {
                //只拷贝一部分
                int w1 = pBitmapData->m_nWidth - (dx>0 ? dx : 0);
                int h1 = pBitmapData->m_nHeight - (dy>0 ? dy : 0);
                if (w1>0 && h1>0)
                {
                    char* tmpbuf = new char[w1*h1 * 4];
                    char* ptmpdt = tmpbuf;
                    char* psrcdt = pBitmapData->premultiplyAlphaData();
                    int tmppitch = w1 * 4;
                    int srcpitch = pBitmapData->m_nWidth * 4;
                    for (int y = 0; y<h1; y++) {
                        memcpy(ptmpdt, psrcdt, tmppitch);
                        ptmpdt += tmppitch;
                        psrcdt += srcpitch;
                    }
                    glTexSubImage2D(GL_TEXTURE_2D, 0, nOffsetX, nOffsetY, w1, h1, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)tmpbuf);
                    delete[] tmpbuf;
                }
            }
        }
        if (m_bMipMap)
        {
            //不管有没有mipmap这个都应该设置啊，所以挪到外面。
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_nMaxFiter);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_nMinFiter);
            if (m_nMinFiter == GL_LINEAR)
                m_nMinFiter = GL_LINEAR_MIPMAP_LINEAR;
            if (m_nMinFiter == GL_NEAREST)
                m_nMinFiter = GL_NEAREST_MIPMAP_NEAREST;
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_nMaxFiter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_nMinFiter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_nSWrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_nTWrapMode);

        glBindTexture(GL_TEXTURE_2D, 0);
        return true;
    }
    bool JCTexture::mergeBitmapDataToAtlas( BitmapData* pBitmapData, BOARDER_TYPE nBoarderFlag, int nOffsetX, int nOffsetY )
	{
        return internalMergeTexture(pBitmapData, nBoarderFlag, nOffsetX, nOffsetY);
	}

    void JCTexture::setWrapMode(int s, int t) {
        m_nSWrapMode = s;
        m_nTWrapMode = t;
        if (m_pGpuTexture > 0) {
            glBindTexture(GL_TEXTURE_2D, m_pGpuTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_nSWrapMode);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_nTWrapMode);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void JCTexture::addToTextureMap()
    {
        if (m_pResManager)
        {
            char sBuf[32];
            sprintf(sBuf, "%x", m_nTextureID);
            m_pResManager->addToMap(this, sBuf);
        }
    }
	void JCTexture::delFromTextureMap()
    {
		if(m_pResManager)
        {
			char sBuf[32];
			sprintf(sBuf,"%x",m_nTextureID);
			m_pResManager->delRes(sBuf,false);
		}
	}
    void JCTexture::allocVMem(int nWidth, int nHeight )
    {
        glBindTexture(GL_TEXTURE_2D, m_pGpuTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_nSWrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_nTWrapMode);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)0);
        glBindTexture(GL_TEXTURE_2D, 0);
        setResSize(nWidth*nHeight * 4);
	}
    void JCTexture::allocVMemFormat(int nWidth, int nHeight, int format, int type)
    {
        glBindTexture(GL_TEXTURE_2D, m_pGpuTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_nSWrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_nTWrapMode);
        glTexImage2D(GL_TEXTURE_2D, 0, format, nWidth, nHeight, 0, format, type, (const GLvoid*)0);
        glBindTexture(GL_TEXTURE_2D, 0);
        setResSize(nWidth*nHeight * 4);
    }
    void JCTexture::allocVMemWithData(BitmapData* pBitmapData)
    {
        glBindTexture(GL_TEXTURE_2D, m_pGpuTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_nSWrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_nTWrapMode);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pBitmapData->m_nWidth, pBitmapData->m_nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)pBitmapData->premultiplyAlphaData());
        glBindTexture(GL_TEXTURE_2D, 0);
        setResSize(pBitmapData->m_nWidth*pBitmapData->m_nHeight * 4);
	}
}

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
