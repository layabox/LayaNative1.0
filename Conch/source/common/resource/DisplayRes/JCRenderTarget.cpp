/**
@file			JCRenderTarget.cpp
@brief			
@author			James
@version		1.0
@date			2017_2_9
*/

#include "JCRenderTarget.h"
#include "../../util/Log.h"
#include "../../util/JCCommonMethod.h"
namespace laya
{
    JCRenderTarget::JCRenderTarget(JCResManager* pResManager, int nWidth, int nHeight, int nFromat, int nType)
        : JCTexture(pResManager, TEXTURE_TYPE_RENDER_TARGET, nWidth, nHeight, nFromat)
    {
        m_nFramebuffer = 0;
        m_nLastFrameBuffer = 0;
        memset(m_nLastViewPort, 0, sizeof(m_nLastViewPort));
        m_nTextureFormat = nFromat;
        m_nTextureType = nType;
    }
    JCRenderTarget::~JCRenderTarget()
    {
        //只用删除FrameBuffer，texture会在父类删除
        if (m_nFramebuffer > 0)
        {
            glDeleteFramebuffers(1, (const GLuint*)&m_nFramebuffer);
            m_nFramebuffer = 0;
        }
    }
    void JCRenderTarget::freeRes(bool bNeedRestore)
    {
        JCTexture::freeRes(bNeedRestore);
        if (m_nFramebuffer > 0)
        {
            glDeleteFramebuffers(1, (const GLuint*)&m_nFramebuffer);
            m_nFramebuffer = 0;
        }
    }
    bool JCRenderTarget::restoreRes()
    {
        if (m_pGpuTexture == 0)
        {
            createGpuTexture();
            allocVMemFormat(m_nTextureWidth, m_nTextureHeight, m_nTextureFormat, m_nTextureType);
            /*这个地方的贴图，完全没有按照LayaTexture的流程去做，
            原因是 在这如果调用enable或者restoreRes 就会从文件中把贴图恢复，但是下面又被清空了
            所以必须在glClearColor(0, 0, 0, 0);  之后再从文件中恢复*/
            /*
            删除FrameBuffer
            if (m_nFramebuffer > 0)
            {
                glDeleteFramebuffers(1, &m_nFramebuffer);
                m_nFramebuffer = 0;
            }
            */
            if (m_nFramebuffer <= 0)
            {
                glGenFramebuffers(1, &m_nFramebuffer);
            }
            GLint nLastFrameBuffer;
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &nLastFrameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, m_nFramebuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pGpuTexture, 0);
            GLenum nStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (nStatus != GL_FRAMEBUFFER_COMPLETE)
            {
                LOGE("JCRenderTarget::restoreRes failed! %d %d %d %d %d", nStatus, m_nTextureWidth, m_nTextureHeight, m_nWidth, m_nHeight);
                glBindFramebuffer(GL_FRAMEBUFFER, nLastFrameBuffer);
                return false;
            }
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBindFramebuffer(GL_FRAMEBUFFER, nLastFrameBuffer);
            m_nLastFrameBuffer = nLastFrameBuffer;
        }
        m_bActive = true;
        setState(ready);
        return m_bActive;
    }
    bool JCRenderTarget::beginSampler()
    {
        enable();
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nLastFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_nFramebuffer);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int nDeltaY = m_nTextureHeight - m_nHeight;
        glGetIntegerv(GL_VIEWPORT, m_nLastViewPort);
        //这里的下移对应贴图的上移
        glViewport(0, nDeltaY, m_nWidth, m_nHeight);
        return true;
    }
    void JCRenderTarget::endSampler()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, m_nLastFrameBuffer);
        glViewport(m_nLastViewPort[0], m_nLastViewPort[1], m_nLastViewPort[2], m_nLastViewPort[3]);
    }
    void JCRenderTarget::getPixels(unsigned char* pPixels, int x, int y, int w, int h)
    {
        int nDeltaY = m_nTextureHeight - m_nHeight;
        GLint preFBO = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &preFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_nFramebuffer);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(x, y + nDeltaY, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pPixels);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glBindFramebuffer(GL_FRAMEBUFFER, preFBO);
    }
}

//-----------------------------END FILE--------------------------------
