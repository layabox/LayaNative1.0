/**
@file			JCRenderTarget.h
@brief			
@author			James
@version		1.0
@date			2017_2_9
*/

#ifndef __JCRenderTarget_H__
#define __JCRenderTarget_H__

#include "JCTexture.h"

namespace laya
{
	class JCRenderTarget : public JCTexture
	{
	public:

        JCRenderTarget( JCResManager* pResManager, int nWidth,int nHeight,int nFromat = GL_RGBA,int nType = GL_UNSIGNED_BYTE);

		virtual ~JCRenderTarget();

        virtual void freeRes( bool bNeedRestore );

        virtual bool restoreRes();

        virtual bool mergeBitmapDataToTexture(BitmapData* pBitmapData, BOARDER_TYPE nBoarderFlag, int x, int y)
        {
            return false;
        }
		
    public:

        bool beginSampler();

        void endSampler();

        void getPixels(unsigned char* pPixels, int x, int y, int w, int h);

    public:

        GLuint					m_nFramebuffer;				///<新创建的frameBuffer
        GLint					m_nLastFrameBuffer;	        ///<记录上一次的FrameBuffer，为了恢复使用的
        int                     m_nLastViewPort[4];         ///<记录上一次ViewPort
        int                     m_nTextureFormat;           ///<TextureFormat
        int                     m_nTextureType;             ///<TextureType

	};
}
//------------------------------------------------------------------------------


#endif //__JCRenderTarget_H__

//-----------------------------END FILE--------------------------------