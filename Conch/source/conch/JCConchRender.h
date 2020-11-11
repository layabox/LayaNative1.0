/**
@file			JCConchRender.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCConchRender_H__
#define __JCConchRender_H__

#include <JCIGLRender.h>
#include <misc/JCWorkSemaphore.h>
#include <buffer/JCBuffer.h>
#include <WebGLRender/JCWebGLRender.h>
#include <Performance/JCPerfHUD.h>
#include <Html5Render/JCHtml5Render.h>
#include <buffer/JCBuffer.h>
#include "JCCmdDispatchManager.h"
#include <resource/JCResManager.h>
#include <resource/Atlas/JCAtlasManager.h>
#include <3DRS/JCGpuProgram.h>
#include <3DRS/JCMaterialManager.h>
#include <3DRS/JC2DShaderManager.h>
#include <3DRS/JCNamedData.h>
#include <Html5Render/JCHtml5RenderManager.h>
#include <resource/text/JCFontManager.h>
#include <resource/text/JCTextManager.h>
#include <resource/JCFileResManager.h>
#ifdef __APPLE__
    #include "IOSFreetype/JCIOSFreeType.h"
#endif
#include <Node/JCNode2D.h>

namespace laya
{
    #define MEM_RENDER_COMMAND_SIZE 256000
    class JCFileSource;
    class JCWorkerThread;
    class JCConchRender : public JCIGLRender
    {
    public:
        JCConchRender(JCFileResManager* pFileResManager);
        ~JCConchRender();
        void onGLReady();
        void setInterruptFunc(std::function<void(void)> pFunc);
        void setRenderData(JCMemClass*& pRenderCmd,int& nGroupCount, int& nVertextCount,int& nNodeCount);
        void renderFrame(long nCurrentFrame, bool bStopRender);
        void render2DMeshToGPU();
        void invalidGLRes();
        void clearAllData();
        void onGLDeviceLosted();
        void willExit();
        bool load2DShader( const char* shaderDefine );
        void setAssetRes(JCFileSource* pAssetRes);
        void setSize( int w,int h );
        void setTransparentMode(float alpha);
        void onTouchStart(double fTime);
    public:

        void pushHtml5Render( JCHtml5Render* pHtml5Render );
        bool removeHtml5Render( JCHtml5Render* pHtml5Render );

    public:
        int                     m_nWidth;                   ///<width
        int                     m_nHeight;                  ///<height
        JCWorkSemaphore		    m_kRenderSem;               ///<与js线程同步的对象。
        bool				    m_bShowPerf;                ///<是否显示性能测试
        long				    m_nFrameCount;              ///<frameCount
        bool                    m_bStopRender;              ///<这个变量仅仅为了IOS使用的，因为IOS主线程（UI线程）和渲染线程是一个，所以增加了这个标记
        bool                    m_bExit;                    ///<是否退出
        bool	                m_bLostDevice;              ///<是否丢失设备
        JCImageManager*         m_pImageManager;            ///<Image管理器
        JCCmdDispathManager*    m_pCmdDispathManager;       ///<cmd dispath 管理器
        JCTextMemoryCanvas*     m_pTextMemoryCanvas;        ///<专门用来绘制文字的
        JCFileResManager*       m_pFileResManager;          ///<FileResManager 外部设置的
        JCFreeTypeFontRender*   m_pFreeTypeRender;          ///<FreeTypeRender
    public:
        JCFontManager*          m_pFontManager;             ///<FontManager
        JCTextManager*          m_pTextManager;             ///<TextManager
        JCHtml5Render*          m_pRootHtml5Render;         ///<rootHtml5Render
        JCHtml5RenderManager*   m_pHtml5RenderManager;      ///<html5RenderManager
        WebGLRenderingContext*  m_pWebGLContext;            ///<webGLContext，这个可以认为是唯一的，由他传给WebGlRender
        JCPerfDataRender        m_kPerfRender;              ///<性能测试
        JCAtlasManager*         m_pAtlasManager;            ///<大图合集管理器
        JCResManager*           m_pTextureManager;          ///<TextureManager
        JC2DShaderManager*      m_p2DShaderManager;         ///<2DShaderManager
        JCBufferManager*        m_pBufferManager;           ///<bufferManager
        JCShaderManager*        m_pShaderManager;           ///<shaderManager
        JCWebGLRender*          m_pWebGLRender;             ///<webglRender 理论上全局就这一个
        JCNode2D*               m_pFPSNode2D;
        JCNode2D*               m_pLoadingViewNode2D;
        JCNode*                 m_pRootNode;                ///<RootNode2D
        JCWorkerThread*         m_pRenderThread;
    private:
        static std::string      s_s2DShaderTransparentDefine;   ///<2D全透明模式的宏定义
        float                   m_nTransparentModeAlphaValue;   ///<全透明模式的alpha值，不为1表示透明模式
        JCMemClass*             m_pCurrentRenderCmds;           ///<渲染指令流
        JCFileSource*           m_pAssetsRes;                   ///<读取文件用的fileSource
#ifdef __APPLE__
        JCIOSFreeType*          m_pIOSFreeType;                 ///<iosfreetype 
#endif
        double                  m_fMouseMoveTime;
        std::function<void(void)>   m_pInterruptFunc;
    };
}
//------------------------------------------------------------------------------
#endif //__JCConchRender_H__

//-----------------------------END FILE--------------------------------