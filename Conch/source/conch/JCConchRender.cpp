/**
@file			JCConchRender.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#include "JCConchRender.h"
#include <util/Log.h>
#include <util/JCCommonMethod.h>
#include "JCScrpitRuntime.h"
#include "JCSystemConfig.h"
#include <downloadCache/JCFileSource.h>
#include <Html5Render/JCHtml5Context.h>
#include <Node/JCNode2DRenderer.h>
#include "JSWrapper/LayaWrap/JSConchConfig.h"
#include <downloadCache/JCServerFileCache.h>
#include <3DRS/JCConchMesh.h>
#include <3DRS/JCRenderer.h>

namespace laya
{
    std::string JCConchRender::s_s2DShaderTransparentDefine = "#define TRANSPARENT_MODE\n";
	JCConchRender::JCConchRender(JCFileResManager* pFileResManager)
	{
        m_fMouseMoveTime = 0.0f;
        m_nTransparentModeAlphaValue = 1.0;
        m_nWidth = 0;
        m_nHeight = 0;
        m_pAssetsRes = NULL;
        m_bExit = false;
        m_bLostDevice = false;
        m_nFrameCount = 0;
		m_bShowPerf = false;
        m_bStopRender = false;
        JCPerfHUD::init();
        m_pFileResManager = pFileResManager;
        m_pRootHtml5Render = NULL;
        m_pHtml5RenderManager = new JCHtml5RenderManager();
        m_p2DShaderManager = new JC2DShaderManager();
        m_pImageManager = new JCImageManager();
        m_pCurrentRenderCmds = new JCMemClass(MEM_RENDER_COMMAND_SIZE, MEM_RENDER_COMMAND_SIZE);
        m_pCurrentRenderCmds->setAlign(true);
        m_pTextMemoryCanvas = new JCTextMemoryCanvas();
        m_pWebGLContext = new WebGLRenderingContext();
        m_pTextureManager = new JCResManager(g_kSystemConfig.m_nMaxTextureMemSize);
        m_pAtlasManager = new JCAtlasManager(g_kSystemConfig.m_nAtlasWidth, g_kSystemConfig.m_nAtlasHeight, (float)g_kSystemConfig.m_nAtlasGridSize, g_kSystemConfig.m_nAtlasNum);
        m_pFontManager = new JCFontManager();
        m_pFreeTypeRender = new JCFreeTypeFontRender();
#ifdef __APPLE__
        m_pIOSFreeType = new JCIOSFreeType();
        m_pFreeTypeRender->setIOSFTInterface(m_pIOSFreeType);
#endif
        m_pTextManager = new JCTextManager(m_pAtlasManager);
        m_pTextManager->setFreeTypeRender(m_pFreeTypeRender);
        m_pRootNode = NULL;
        JCNode2DRenderer::initAll();
        m_pBufferManager = new JCBufferManager();
        m_pShaderManager = new JCShaderManager();
        m_pCmdDispathManager = new JCCmdDispathManager(m_pTextureManager,m_pImageManager, m_pTextMemoryCanvas, m_p2DShaderManager, m_pWebGLContext,m_pHtml5RenderManager, m_pTextManager, m_pFontManager, m_pBufferManager,m_pShaderManager);
        m_pWebGLRender = NULL;
        m_pLoadingViewNode2D = NULL;
        m_pFPSNode2D = NULL;
	}
	JCConchRender::~JCConchRender()
	{
        m_pRootNode = NULL;
        m_pRootHtml5Render = NULL;
        m_pLoadingViewNode2D = NULL;
        m_pFPSNode2D = NULL;
        JCNode2DRenderer::clearAll();
        //TODO 这段代码有点恶心
        //这个是在JCConch中传过来的，但是为了确保最后一帧，在这释放了，因为JCConchRender全局只有一个
        if (m_pFileResManager)
        {
            //这个fileCache也是同理，需要在这删除，
            if (m_pFileResManager->m_pFileCache)
            {
                delete m_pFileResManager->m_pFileCache;
                m_pFileResManager->m_pFileCache = NULL;
            }
            delete m_pFileResManager;
            m_pFileResManager = NULL;
        }
        if (m_pAtlasManager)
        {
            delete m_pAtlasManager;
            m_pAtlasManager = NULL;
        }
#ifdef __APPLE__
        if( m_pIOSFreeType){
            delete m_pIOSFreeType;
            m_pIOSFreeType = nullptr;
        }
#endif
        if (m_pTextManager)
        {
            delete m_pTextManager;
            m_pTextManager = NULL;
        }
        if (m_pFontManager)
        {
            delete m_pFontManager;
            m_pFontManager = NULL;
        }
        if (m_pImageManager)
        {
            delete m_pImageManager;
            m_pImageManager = NULL;
        }
        if (m_pCurrentRenderCmds)
        {
            delete m_pCurrentRenderCmds;
            m_pCurrentRenderCmds = NULL;
        }
        if (m_pTextMemoryCanvas)
        {
            delete m_pTextMemoryCanvas;
            m_pTextMemoryCanvas = NULL;
        }
        if (m_pCmdDispathManager)
        {
            delete m_pCmdDispathManager;
            m_pCmdDispathManager = NULL;
        }
        if (m_p2DShaderManager)
        {
            delete m_p2DShaderManager;
            m_p2DShaderManager = NULL;
        }
        if (m_pWebGLContext)
        {
            delete m_pWebGLContext;
            m_pWebGLContext = NULL;
        }
        if (m_pHtml5RenderManager)
        {
            delete m_pHtml5RenderManager;
            m_pHtml5RenderManager = NULL;
        }
        if (m_pTextureManager)
        {
            delete m_pTextureManager;
            m_pTextureManager = NULL;
        }
        if (m_pFreeTypeRender)
        {
            delete m_pFreeTypeRender;
            m_pFreeTypeRender = NULL;
        }
        if (m_pBufferManager != NULL)
        {
            delete m_pBufferManager;
            m_pBufferManager = NULL;
        }
        if (m_pShaderManager != NULL)
        {
            delete m_pShaderManager;
            m_pShaderManager = NULL;
        }
	}
    void JCConchRender::setSize(int w, int h)
    {
        m_nWidth = w;
        m_nHeight = h;
        m_kPerfRender.m_nScrWidth = w;
        m_kPerfRender.m_nScrHeight = h;
    }
    void JCConchRender::setAssetRes(JCFileSource* pAssetRes)
    {
        m_pAssetsRes = pAssetRes;
        load2DShader(m_nTransparentModeAlphaValue>=1.0?"": s_s2DShaderTransparentDefine.c_str());
    }
    bool JCConchRender::load2DShader(const char* shaderDefine)
    {
        if ( m_p2DShaderManager->getShader(SHADER_TYPE_FILLCOLOR) != NULL)return true;
        char* sDefvsBuffer = NULL;
        int nBufferLen = 0;
        m_pAssetsRes->loadFileContent("shaders/todevVS.glsl", sDefvsBuffer, nBufferLen);

        char* sFillColorPSBuffer = NULL;
        m_pAssetsRes->loadFileContent("shaders/fillColorPS.glsl", sFillColorPSBuffer, nBufferLen);
        m_p2DShaderManager->m_vShaders[SHADER_TYPE_FILLCOLOR] = (new JCGpuProgramTemplate(sDefvsBuffer, sFillColorPSBuffer))->getInst(shaderDefine,0);
        
        char* sDrawImagePSBuffer = NULL;
        m_pAssetsRes->loadFileContent("shaders/drawImagePS.glsl", sDrawImagePSBuffer, nBufferLen);
        m_p2DShaderManager->m_vShaders[SHADER_TYPE_DRAWIMAGE] = (new JCGpuProgramTemplate(sDefvsBuffer, sDrawImagePSBuffer))->getInst(shaderDefine,0);

        char* sFillImagePSBuffer = NULL;
        m_pAssetsRes->loadFileContent("shaders/fillImagePS.glsl", sFillImagePSBuffer, nBufferLen);
        m_p2DShaderManager->m_vShaders[SHADER_TYPE_FILLIMAGE] = (new JCGpuProgramTemplate(sDefvsBuffer, sFillImagePSBuffer))->getInst(shaderDefine,0);

        char* sFillImageMatfilterPSBuffer = NULL;
        m_pAssetsRes->loadFileContent("shaders/fillImageMatFilterPS.glsl", sFillImageMatfilterPSBuffer, nBufferLen);
        m_p2DShaderManager->m_vShaders[SHADER_TYPE_FILLIMAGE_MATFILTER] = (new JCGpuProgramTemplate(sDefvsBuffer, sFillImageMatfilterPSBuffer))->getInst(shaderDefine, 0);

        char* sDrawFilterImagePSBuffer = NULL;
        m_pAssetsRes->loadFileContent("shaders/drawFilterImagePS.glsl", sDrawFilterImagePSBuffer, nBufferLen);
        m_p2DShaderManager->m_vShaders[SHADER_TYPE_DRAWFILTERIMAGE] = (new JCGpuProgramTemplate(sDefvsBuffer, sDrawFilterImagePSBuffer))->getInst(shaderDefine, 0);

        char* sBlurImgPSBuffer = nullptr;
        m_pAssetsRes->loadFileContent("shaders/blurImg.glsl", sBlurImgPSBuffer, nBufferLen);
        m_p2DShaderManager->m_vShaders[SHADER_TYPE_BLURIMG] = (new JCGpuProgramTemplate(sDefvsBuffer, sBlurImgPSBuffer))->getInst(shaderDefine, 0);

        char* sGlowImgPSBuffer = nullptr;
        m_pAssetsRes->loadFileContent("shaders/glowImg.glsl", sGlowImgPSBuffer, nBufferLen);
        m_p2DShaderManager->m_vShaders[SHADER_TYPE_GLOWIMG] = (new JCGpuProgramTemplate(sDefvsBuffer, sGlowImgPSBuffer))->getInst(shaderDefine, 0);

        char* sMatFilterPSBuffer = nullptr;
        m_pAssetsRes->loadFileContent("shaders/drawMatFilterImagePS.glsl", sMatFilterPSBuffer, nBufferLen);
        m_p2DShaderManager->m_vShaders[SHADER_TYPE_MATFILTER] = (new JCGpuProgramTemplate(sDefvsBuffer, sMatFilterPSBuffer))->getInst(shaderDefine, 0);

        if (sDefvsBuffer)
        {
            delete sDefvsBuffer;
        }
        if (sFillColorPSBuffer)
        {
            delete sFillColorPSBuffer;
        }
        if (sDrawImagePSBuffer)
        {
            delete sDrawImagePSBuffer;
        }
        if (sFillImagePSBuffer)
        {
            delete sFillImagePSBuffer;
        }
        if (sDrawFilterImagePSBuffer)
        {
            delete sDrawFilterImagePSBuffer;
        }
        if (sBlurImgPSBuffer) delete sBlurImgPSBuffer;
        if (sGlowImgPSBuffer) delete sGlowImgPSBuffer;
        if (sMatFilterPSBuffer) delete sMatFilterPSBuffer;
        return true;
    }
    void JCConchRender::setTransparentMode(float alpha)
    {
        if (m_nTransparentModeAlphaValue == alpha)return;
        m_nTransparentModeAlphaValue = alpha;
        if (m_p2DShaderManager)
        {
            for (int i = 0; i < SHADER_TYPE_COUNT; i++)
            {
                if (m_p2DShaderManager->m_vShaders[i])
                {
                    m_p2DShaderManager->m_vShaders[i]->freeGLResource();
                    delete m_p2DShaderManager->m_vShaders[i];
                    m_p2DShaderManager->m_vShaders[i] = NULL;
                }
            }
        }
        load2DShader(m_nTransparentModeAlphaValue >= 1.0 ? "" : s_s2DShaderTransparentDefine.c_str());
    }
	void JCConchRender::onGLReady()
	{
        JCRenderer::init();
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
	}
    void JCConchRender::onGLDeviceLosted()
    {
        m_bLostDevice = true;
    }
    void JCConchRender::invalidGLRes()
    {
        //freeAll texture
        if (m_pTextureManager)
        {
            m_pTextureManager->freeAll();
            //下面的不用调用了，只用清空freeAll就OK了
            /*
            for (JCResManager::ResMap::iterator iter = m_pTextureManager->m_AllRes.begin(); iter != m_pTextureManager->m_AllRes.end(); iter++)
            {
                JCTexture* pTexture = (JCTexture*)(iter->second);
                if (pTexture)
                {
                    pTexture->freeRes(true);
                }
            }
            m_pTextureManager->m_nCurSize = 0;
            */
        }
        //2Dshader管理器
        if (m_p2DShaderManager)
        {
            for (int i = 0; i < SHADER_TYPE_COUNT; i++)
            {
                if(m_p2DShaderManager->m_vShaders[i])
                    m_p2DShaderManager->m_vShaders[i]->freeGLResource();
            }
        }
        //大图合集管理器
        if (m_pAtlasManager)
        {
            m_pAtlasManager->freeGLResource();
        }
        //html5Canvas的，其中包括了普通的vertextBuffer和frameBuffer
        m_pHtml5RenderManager->invalidGLRes();
        m_pShaderManager->invalidGLRes();

        m_kPerfRender.invalidGLRes();
        //通知JS为了单纯的 webgl模式
        JCScriptRuntime::s_JSRT->onInvalidGLRes();
    }
	void JCConchRender::clearAllData()
	{
        invalidGLRes();
        //文字的canvas要清空一下
        m_pTextMemoryCanvas->reset();
        //纯wengl渲染指令要清空一下
        if (m_pWebGLContext)
        {
            m_pWebGLContext->deleteAllGPURes();
        }
        //图片全部清空
        if (m_pImageManager)
        {
            m_pImageManager->resetRenderThread();
        }
        //大图合集没有要清空的内存，只用清空显存就可以了
        //m_pAtlasManager;

        //dispathManager reset一下
        m_pCmdDispathManager->reset();
        m_pHtml5RenderManager->clearAll();
        m_pRootHtml5Render = NULL;
        m_pRootNode = NULL;
        m_pFPSNode2D = NULL;
        m_pLoadingViewNode2D = NULL;
        JCNode2D::m_nCreateCacheCanvasID = 0xFFFFFF;

        //清空文字相关的
        m_pFontManager->clearAllData();
        m_pTextManager->clearAll();
        //freeType清空用户数据
        m_pFreeTypeRender->clearCustomFont();
        //一定要最后删除textureManager，因为ImageManager删除图片会删除Texture，JChtml5Context删除会删除RenderTarget
        //否则会有二次删除的过程
        if (m_pTextureManager)
        {
            m_pTextureManager->destroyAll(true);
        }
        m_pBufferManager->clear();
        m_pShaderManager->clear();
        m_pWebGLRender = NULL;
		JCShaderDefine::destory();
	}
    void JCConchRender::setRenderData(JCMemClass*& pRenderCmd,int& nGroupCount,int& nVertextCount, int& nNodeCount)
    {
        m_kRenderSem.waitUntilNoData();
        if (pRenderCmd) 
        {
            std::swap(pRenderCmd, m_pCurrentRenderCmds);
        }
        m_pTextMemoryCanvas->swapMemoryCanvasData();
        m_kRenderSem.setDataNum(1);
        nGroupCount = JCConchMesh::s_nCurrentFrameGroupNum;
        nVertextCount = JCConchMesh::s_nCurrentFrameVertextNum;
        nNodeCount = JCNode::s_nNodeCount;
        JCNode::s_nNodeCount = 0;
    }
    void JCConchRender::setInterruptFunc(std::function<void(void)> pFunc)
    {
        m_kRenderSem.waitUntilNoData();
        m_pInterruptFunc = pFunc;
        m_kRenderSem.setDataNum(2);
        m_kRenderSem.waitUntilNoData();
    }
#ifdef __APPLE__
    void JCConchRender::renderFrame( long p_nCurrentFrame, bool p_bStopRender)
    {
        if (m_bExit)return;
        PERF_INITVAR(nTime123);
        /*
        if( !m_bStopRender )
        {
            if (JCScriptRuntime::s_JSRT) {
                JCScriptRuntime::s_JSRT->m_ScriptThread.post(std::bind(
                    &JCScriptRuntime::onUpdate
                    , JCScriptRuntime::s_JSRT));
            }
        }
        */
        double synctm = JCPerfHUD::m_tmVSYNC;//因为会在中间被改变，所以先保存下来。注意不能改成float，否则会有很大的精度差。
        perfBarData* pRenderPerfBar = (perfBarData*)JCPerfHUD::getData(JCPerfHUD::PHUD_BAR_RENDER);
        float stofftm = (float)(tmGetCurms() - synctm);
        //进行绘制
        float waitendoff = 0;
        if (!m_bStopRender)
        {
            //线程等待
            while (true)
            {
                m_kRenderSem.waitUntilHasData();
                if (m_kRenderSem.getDataNum() == 2)
                {
                    m_pInterruptFunc();
                    m_kRenderSem.setDataNum(0);
                }
                else
                {
                    break;
                }
            }
        }
        auto glwait = (perfBarData*)JCPerfHUD::getData(JCPerfHUD::PHUD_BAR_GLWAIT);
        waitendoff = (float)(tmGetCurms() - synctm);
        if (glwait) {
            glwait->addData(synctm, stofftm, waitendoff);
        }

        
        m_nFrameCount++;
        PERF_UPDATE_DTIME(JCPerfHUD::PHUD_FRAME_DELAY, JCPerfHUD::m_tmCurRender);
        PERF_INITVAR(nTime);
        JCConchMesh::s_nCurrentFrameGroupNum = 0;
        JCConchMesh::s_nCurrentFrameVertextNum = 0;
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (m_pWebGLRender)
        {
            m_pWebGLRender->setIosViewport();
        }
        m_pCmdDispathManager->dispatchRenderCmd(*m_pCurrentRenderCmds, m_nFrameCount );
        if(!m_bStopRender)
        {
            if (m_pRootHtml5Render)
            {
                if (m_pRootNode)
                {
                    m_pRootNode->render(m_pRootHtml5Render->m_pContext, 0, 0);
                }
                if (m_pLoadingViewNode2D)
                {
                    m_pLoadingViewNode2D->render(m_pRootHtml5Render->m_pContext, 0, 0);
                }
                if (m_pFPSNode2D)
                {
                    m_pFPSNode2D->render(m_pRootHtml5Render->m_pContext, 0, 0);
                }
            }
            render2DMeshToGPU();
            m_pBufferManager->update();
        }
        //clearData
        m_pCurrentRenderCmds->clearData();
        m_pTextMemoryCanvas->clearRenderThreadData();
        //imageManager的update 不需要每帧都调用
        if (m_nFrameCount % 5 == 0)
        {
            m_pImageManager->update();
        }
        m_pAtlasManager->toGarbageCollection();
        //------------------------------------------------------------------------------
        m_kRenderSem.setDataNum(0);
        //性能测试
        PERF_UPDATE_DATA(JCPerfHUD::PHUD_RENDER_DELAY, (float)(tmGetCurms() - nTime));
        if (m_bShowPerf)
        {
            m_kPerfRender.drawData();
        }
        FRAME_TYPE nFrameType = (g_kSystemConfig.m_nFrameType == FT_MOUSE) ? (nTime123 - m_fMouseMoveTime < g_kSystemConfig.m_nFrameThreshold ? FT_FAST : FT_SLOW) : g_kSystemConfig.m_nFrameType;
        if (nFrameType == FT_SLOW)
        {
            int64_t nSleepTime = g_kSystemConfig.m_nSleepTime - (tmGetCurms() - nTime123);
            if (nSleepTime > 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(nSleepTime));
            }
        }
        if (pRenderPerfBar) {
            auto edofftm = tmGetCurms() - synctm;
            pRenderPerfBar->addData(synctm, waitendoff, edofftm);
        }
    }
#else
	void JCConchRender::renderFrame( long nCurrentFrame, bool bStopRender)
	{
        PERF_INITVAR(nTime123);
        bool glDriveJS = true;
#ifdef ANDROID
        glDriveJS = !g_kSystemConfig.m_bUseChoreographer;
#endif
        if (glDriveJS && JCScriptRuntime::s_JSRT) {
            JCScriptRuntime::s_JSRT->m_ScriptThread.post(std::bind(
                &JCScriptRuntime::onUpdate
                ,JCScriptRuntime::s_JSRT));
        }

        double synctm = JCPerfHUD::m_tmVSYNC;//因为会在中间被改变，所以先保存下来。注意不能改成float，否则会有很大的精度差。
        perfBarData* pRenderPerfBar = (perfBarData*)JCPerfHUD::getData(JCPerfHUD::PHUD_BAR_RENDER);
        float stofftm = (float)(tmGetCurms() - synctm);

        if (m_bExit)return;

        //线程等待
        while (true)
        {
            m_kRenderSem.waitUntilHasData();
            if (m_kRenderSem.getDataNum() == 2)
            {
                m_pInterruptFunc();
                m_kRenderSem.setDataNum(0);
            }
            else
            {
                break;
            }
        }

        auto glwait = (perfBarData*)JCPerfHUD::getData(JCPerfHUD::PHUD_BAR_GLWAIT);
        float waitendoff = (float)(tmGetCurms() - synctm);
        if (glwait) {
            glwait->addData(synctm, stofftm, waitendoff);
        }

        m_nFrameCount++;
        PERF_UPDATE_DTIME(JCPerfHUD::PHUD_FRAME_DELAY, JCPerfHUD::m_tmCurRender);
        PERF_INITVAR(nTime);
        JCConchMesh::s_nCurrentFrameGroupNum = 0;
        JCConchMesh::s_nCurrentFrameVertextNum = 0;
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_bLostDevice == true)
        {
            invalidGLRes();  
            m_pCmdDispathManager->dispatchRenderCmdWhenLostedDevice(*m_pCurrentRenderCmds, m_nFrameCount );
            m_bLostDevice = false;
        }
        else
        {
            m_pCmdDispathManager->dispatchRenderCmd(*m_pCurrentRenderCmds, m_nFrameCount );
        }
        if (m_pRootHtml5Render)
        {
            if (m_pRootNode)
            {
                m_pRootNode->render(m_pRootHtml5Render->m_pContext, 0, 0);
            }
            if (m_pLoadingViewNode2D)
            {
                m_pLoadingViewNode2D->render(m_pRootHtml5Render->m_pContext, 0, 0);
            }
            if (m_pFPSNode2D)
            {
                m_pFPSNode2D->render(m_pRootHtml5Render->m_pContext, 0, 0);
            }
        }
        render2DMeshToGPU();
        m_pBufferManager->update();
        //clearData
        m_pCurrentRenderCmds->clearData();
        m_pTextMemoryCanvas->clearRenderThreadData();
        //imageManager的update 不需要每帧都调用
        if (m_nFrameCount % 5 == 0)
        {
            m_pImageManager->update();
        }
        m_pAtlasManager->toGarbageCollection();
        //性能测试
        PERF_UPDATE_DATA(JCPerfHUD::PHUD_RENDER_DELAY, (float)(tmGetCurms() - nTime));
        //------------------------------------------------------------------------------
        m_kRenderSem.setDataNum(0);

        if (m_bShowPerf)
        {
            m_kPerfRender.drawData();
        }
        if (pRenderPerfBar) {
            float edofftm = (float)(tmGetCurms() - synctm);
            pRenderPerfBar->addData(synctm, waitendoff, edofftm);
        }
        FRAME_TYPE nFrameType = (g_kSystemConfig.m_nFrameType == FT_MOUSE) ? (nTime123 - m_fMouseMoveTime < g_kSystemConfig.m_nFrameThreshold ? FT_FAST : FT_SLOW) : g_kSystemConfig.m_nFrameType;
        if (nFrameType == FT_SLOW)
        {
            int64_t nSleepTime = g_kSystemConfig.m_nSleepTime - (tmGetCurms() - nTime123);
            if (nSleepTime > 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(nSleepTime));
            }
        }
    }
#endif
    void JCConchRender::render2DMeshToGPU()
    {
        if (m_pRootHtml5Render)
        {
            //JSConchConfig* pCfg = JSConchConfig::getInstance();
            JCHtml5Context* pRootCtx = m_pRootHtml5Render->m_pContext;
            pRootCtx->renderToGpu();
            pRootCtx->onRenderEnd();
        }
    }
	void JCConchRender::willExit()
	{
        LOGI("render will exit");
        //设置这个后，渲染流程就不要再跑了
        m_bExit = true;
		m_kRenderSem.notifyAllWait();
	}
    void JCConchRender::onTouchStart(double fTime)
    {
        m_fMouseMoveTime = fTime;
    }

}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
