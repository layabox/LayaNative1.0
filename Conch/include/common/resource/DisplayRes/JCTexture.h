/**
@file			JCTexture.h
@brief			
@author			James
@version		1.0
@date			2016_5_26
*/

#ifndef __JCTexture_H__
#define __JCTexture_H__

#include "../JCResource.h"
#include "../JCResManager.h"
#include "../../imageLib/JCImageRW.h"
#if __APPLE__
    #include <OpenGLES/ES2/gl.h>
#else
    #include <GLES2/gl2.h>
#endif
#include "../DisplayRes/JCDisplayRes.h"
#include <thread>

namespace laya
{
    class JCAtlas;
    enum TEXTURE_TYPE
    {
        TEXTURE_TYPE_NORMAL = 0,                //普通纹理
        TEXTURE_TYPE_ATLAS = 1,                 //大图合集
        TEXTURE_TYPE_RENDER_TARGET = 2,         //Canvas
        TEXTURE_TYPE_CUBE = 3,                  //CUBE
    };
	class JCTexture : public JCResource , public JCDisplayRes
	{
	public:

        JCTexture( JCResManager* pResManager, TEXTURE_TYPE nType,int nWidth,int nHeight,int nFormat,JCAtlas* pAtlas = NULL );

		virtual ~JCTexture();

        virtual void freeRes( bool bNeedRestore );

        virtual bool restoreRes();

        virtual void touch( bool bRestoreRes = false );

        virtual ENABLE_TYPE enable();

        virtual JCTexture* getTexture();

        virtual bool mergeBitmapDataToTexture(BitmapData* pBitmapData, BOARDER_TYPE nBoarderFlag, int x, int y);

        virtual bool mergeBitmapDataToAtlas(BitmapData* pBitmapData, BOARDER_TYPE nBoarderFlag, int nOffsetX, int nOffsetY);
	public:

        void freeGLResource();

        void setSize(int nWidth, int nHeight);

		int getWidth();

		int getHeight();

		float getTextureWidth();

        float getTextureHeight();

        int getTextureID();

        void createGpuTexture();

        //返回gl贴图对象
        void allocVMem(int nWidth, int nHeight);

        void allocVMemFormat(int nWidth, int nHeight, int format, int type);

        void setWrapMode(int s, int t);

        void delFromTextureMap();

    private:

        void allocVMemWithData( BitmapData* pBitmapData );
    
        bool internalMergeTexture(BitmapData* pBitmapData, BOARDER_TYPE nBoarderFlag, int nOffsetX, int nOffsetY);

        void addToTextureMap();

	public:
        static int          ms_nTextureID;      //全局的ID
        int                 m_nTextureID;       //textureID
        GLuint              m_pGpuTexture;      //4
        short               m_nWidth;		    //2
        short               m_nHeight;		    //2
        float               m_nTextureWidth;	//2
        float               m_nTextureHeight;	//2
        TEXTURE_TYPE        m_nType;			//
        bool                m_bActive;
        JCAtlas*            m_pAtlas;
		bool				m_bMipMap;
		int					m_nMinFiter;
		int					m_nMaxFiter;
        int                 m_nSWrapMode = GL_CLAMP_TO_EDGE;// GL_REPEAT;
        int                 m_nTWrapMode = GL_CLAMP_TO_EDGE;// GL_REPEAT;

    private:
		
		std::thread::id   m_nCreateThread;    //保存线程ID，怕其他线程进行操作
        int m_nFormat;
	};
}

//------------------------------------------------------------------------------


#endif //__JCTexture_H__

//-----------------------------END FILE--------------------------------
