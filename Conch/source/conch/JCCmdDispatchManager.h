
/**
@file			JCCmdDispatchManager.h
@brief			
@author			James
@version		1.0
@date			2016_5_27
*/

#ifndef __JCCmdDispatchManager_H__
#define __JCCmdDispatchManager_H__

#include <JCICmdDispatch.h>
#include <vector>
#include "Image/JCImageManager.h"
#include "Manager/JCBufferManager.h"
#include "Manager/JCShaderManager.h"
#include "JCPublicCmdDispath.h"
#include "Text/JCTextMemoryCanvas.h"
#include <3DRS/JC2DShaderManager.h>
#include <WebGLRender/WebGL.h>
#include <Html5Render/JCHtml5RenderManager.h>
#include <resource/JCResManager.h>
#include <resource/text/JCFontManager.h>
#include <resource/text/JCTextManager.h>

namespace laya
{
    class JCConchRender;
    #define PUBLIC_CMD_DISPATH 0
    /** 
     * @brief 
    */
    class JCCmdDispathManager
    {
    public:

        typedef std::vector<JCICmdDispatch*>    VectorCmdDispath;

    public:

        /** @brief构造函数
        */
        JCCmdDispathManager(JCResManager* pTextureManager,JCImageManager* pImageManager,JCTextMemoryCanvas* pTextMemoryCanvas,
                JC2DShaderManager* p2DShaderManager,WebGLRenderingContext* pWebGLContext, 
                JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager,
                JCBufferManager* pBufferManager,JCShaderManager* pShaderManager);

        /** @brief析构函数
        */
        ~JCCmdDispathManager();

        bool pushCmdDispaths( int nID,JCICmdDispatch* pCmdDispatch );

        bool deleteCmdDispaths( int nID );

        bool dispatchRenderCmd(JCMemClass& pRenderCmd,int nFrameCount);

        bool dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount);

        void reset();

    public:

        VectorCmdDispath        m_vCmdDispaths;
        JCResManager*           m_pTextureManager;
        JCImageManager*         m_pImageManager;
        JCTextMemoryCanvas*     m_pTextMemoryCanvas;
        JC2DShaderManager*      m_p2DShaderManager;
        WebGLRenderingContext*  m_pWebGLContext;
        JCHtml5RenderManager*   m_pHtml5RenderManager;
        JCTextManager*          m_pTextManager;
        JCFontManager*          m_pFontManager;
        JCBufferManager*        m_pBufferManager;
        JCShaderManager*        m_pShaderManager;
    };
}

#endif //__JCCmdDispatchManager_H__

//-----------------------------END FILE--------------------------------