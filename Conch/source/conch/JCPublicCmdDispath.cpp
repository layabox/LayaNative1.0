/**
@file			JCPublicCmdDispath.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_27
*/

#include "JCPublicCmdDispath.h"
#include <util/Log.h>
#include "WebGLRender/JCWebGLRender.h"
#include "JCCmdDispatchManager.h"
#include "Html5Render/JCHtml5Render.h"
#include "JCConchRender.h"
#include "JCConch.h"
#include <Node/JCGraphicsCmdDispath.h>
#include <Node/JCNode2DCmdDispath.h>
#include <Particle2D/JCParticleTemplate2DCmdDispath.h>
#include "./3DRS/JCShaderDefine.h"
extern int g_nInnerWidth;
extern int g_nInnerHeight;
//------------------------------------------------------------------------------
namespace laya
{
    JCPublicCmdDispath::JCPublicCmdDispath(JCCmdDispathManager* pCmdDispathManager)
    {
        m_pCmdDispathManager = pCmdDispathManager;
        m_nID = 0;
    }
    JCPublicCmdDispath::~JCPublicCmdDispath()
    {

    }
    bool JCPublicCmdDispath::dispatchRenderCmd(JCMemClass& pRenderCmd,int nFrameCount)
    {
        static const ProcFunction g_svProcFunctions[] =
        {
            &JCPublicCmdDispath::_rendercmd_startJSThread,
            &JCPublicCmdDispath::_rendercmd_createImage,
            &JCPublicCmdDispath::_rendercmd_deleteImage,
            &JCPublicCmdDispath::_rendercmd_createHtml5Context,
            &JCPublicCmdDispath::_rendercmd_deleteHtml5Context,
            &JCPublicCmdDispath::_rendercmd_createWebGLContext,
            &JCPublicCmdDispath::_rendercmd_deleteWebGLContext,
            &JCPublicCmdDispath::_rendercmd_createNode,
            &JCPublicCmdDispath::_rendercmd_deleteNode,
            &JCPublicCmdDispath::_rendercmd_setContextType,
            &JCPublicCmdDispath::_rendercmd_setHtml5CanvasSize,
            &JCPublicCmdDispath::_rendercmd_createVetexBuffer,
            &JCPublicCmdDispath::_rendercmd_createShader,
            &JCPublicCmdDispath::_rendercmd_createGraphics,
            &JCPublicCmdDispath::_rendercmd_deleteGraphics,
            &JCPublicCmdDispath::_rendercmd_setRootNode,
            &JCPublicCmdDispath::_rendercmd_setMainCanvasScaleInfo,
            &JCPublicCmdDispath::_rendercmd_setMemoryCanvasClip,
            &JCPublicCmdDispath::_rendercmd_setFPSNode,
            &JCPublicCmdDispath::_rendercmd_setLoadingViewNode,
			&JCPublicCmdDispath::_rendercmd_createParticalTemplate2D,
			&JCPublicCmdDispath::_rendercmd_deleteParticalTemplate2D,
            &JCPublicCmdDispath::_rendercmd_registShaderDefine,
            &JCPublicCmdDispath::_rendercmd_setTransparentMode,
            &JCPublicCmdDispath::_rendercmd_setImageReleaseSpaceTime,
            &JCPublicCmdDispath::_rendercmd_PerfAddData,
            &JCPublicCmdDispath::_rendercmd_PerfUpdateDt,
            &JCPublicCmdDispath::_rendercmd_ImageEnableMerageInAtlas,
            &JCPublicCmdDispath::_rendercmd_ImageReleaseTexture,
            &JCPublicCmdDispath::_rendercmd_deleteVetexBuffer,
        };
        static const int nFuncs = sizeof(g_svProcFunctions) / sizeof(g_svProcFunctions[0]);
        char* pData = pRenderCmd.getReadPtr();
        if (pData)
        {
            int nFunctionID = *(int*)pData;
            if (nFunctionID >= 0 && nFunctionID < nFuncs)
            {
                if ((this->*g_svProcFunctions[nFunctionID])(pRenderCmd) == false)
                {
                    LOGE("JCPublicCmdDispath::dispatchRenderCmd Fail to execute public command : %d ", nFunctionID);
                    return false;
                }
            }
            else
            {
                LOGE("JCPublicCmdDispath::dispatchRenderCmd Wrong public command number!,cmd=%d", nFunctionID);
            }
        }
        return true;
    }
    bool JCPublicCmdDispath::dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount)
    {
        return dispatchRenderCmd(pRenderCmd, nFrameCount);
    }
    bool JCPublicCmdDispath::restartJSThread()
    {
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_startJSThread(JCMemClass& pRenderCmd)
    {
        int* pFuncID = pRenderCmd.popp<int>();

        //TODO 如果以后没有用，就可以删除掉了
        for ( JCCmdDispathManager::VectorCmdDispath::iterator iter = m_pCmdDispathManager->m_vCmdDispaths.begin(); iter != m_pCmdDispathManager->m_vCmdDispaths.end(); iter++)
        {
            JCICmdDispatch* pCmdDispath = *iter;
            if (pCmdDispath)
            {
                pCmdDispath->restartJSThread();
            }
        }
        //清空所有数据，包括显卡，内存中的
        JCConch::s_pConchRender->clearAllData();
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_createImage(JCMemClass& pRenderCmd)
    {
        ParamCreateImage* pParam = pRenderCmd.popp<ParamCreateImage>();
        if (pParam)
        {
            JCImage* pImage = (JCImage*)(pParam->imagePtr);
            if (m_pCmdDispathManager->m_pImageManager)
            {
                m_pCmdDispathManager->m_pImageManager->setImage(pParam->imageID, pImage);
            }
            return true;
        }
        return false;
    }
    bool JCPublicCmdDispath::_rendercmd_deleteImage(JCMemClass& pRenderCmd)
    {
        ParamDeleteImage* pParam = pRenderCmd.popp<ParamDeleteImage>();
        if (pParam)
        {
            if (m_pCmdDispathManager->m_pImageManager)
            {
                m_pCmdDispathManager->m_pImageManager->deleteImage(pParam->imageID);
            }
            return true;
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_createHtml5Context(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
        {
			JCHtml5Render* pHtml5Render = new JCHtml5Render(pParam->nID, m_pCmdDispathManager->m_pTextureManager, m_pCmdDispathManager->m_pImageManager, m_pCmdDispathManager->m_p2DShaderManager, m_pCmdDispathManager->m_pHtml5RenderManager, m_pCmdDispathManager->m_pTextManager, m_pCmdDispathManager->m_pFontManager, m_pCmdDispathManager->m_pBufferManager, m_pCmdDispathManager->m_pShaderManager, &m_pCmdDispathManager->m_vCmdDispaths);
            m_pCmdDispathManager->pushCmdDispaths(pHtml5Render->m_nID, pHtml5Render);
            m_pCmdDispathManager->m_pHtml5RenderManager->pushHtml5Render(pHtml5Render);
            return true;
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_deleteHtml5Context(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)
        {
            JCHtml5Render* pHtml5Render = (JCHtml5Render*)(m_pCmdDispathManager->m_vCmdDispaths[pParam->nID]);
            m_pCmdDispathManager->m_pHtml5RenderManager->removeHtml5Render(pParam->nID);
            m_pCmdDispathManager->deleteCmdDispaths(pParam->nID);
            return true;
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_createWebGLContext(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)
        {
            JCWebGLRender* pWebGLRender = new JCWebGLRender(pParam->nID,m_pCmdDispathManager->m_pImageManager, m_pCmdDispathManager->m_pTextMemoryCanvas,m_pCmdDispathManager->m_pWebGLContext );
            m_pCmdDispathManager->pushCmdDispaths(pWebGLRender->m_nID, pWebGLRender);
            JCConch::s_pConchRender->m_pWebGLRender = pWebGLRender;
            return true;
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_deleteWebGLContext(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)
        {
            JCWebGLRender* pWebGLRender = (JCWebGLRender*)(m_pCmdDispathManager->m_vCmdDispaths[pParam->nID]);
            if (JCConch::s_pConchRender->m_pWebGLRender == pWebGLRender)
            {
                JCConch::s_pConchRender->m_pWebGLRender = NULL;
            }
            m_pCmdDispathManager->deleteCmdDispaths(pParam->nID);
            return true;
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_createNode(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)
        {
            JCNode2DCmdDispath* pNode2DCmdDispath = new JCNode2DCmdDispath(pParam->nID, m_pCmdDispathManager->m_pTextureManager, m_pCmdDispathManager->m_pImageManager, m_pCmdDispathManager->m_p2DShaderManager, m_pCmdDispathManager->m_pHtml5RenderManager, m_pCmdDispathManager->m_pTextManager, m_pCmdDispathManager->m_pFontManager, m_pCmdDispathManager->m_pBufferManager, m_pCmdDispathManager->m_pShaderManager,&m_pCmdDispathManager->m_vCmdDispaths);
            m_pCmdDispathManager->pushCmdDispaths(pParam->nID, pNode2DCmdDispath);
            return true;
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_deleteNode(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)
        {
            JCNode2DCmdDispath* pNode2DCmdDispath = (JCNode2DCmdDispath*)(m_pCmdDispathManager->m_vCmdDispaths[pParam->nID]);
            if (JCConch::s_pConchRender->m_pRootNode && pParam->nID == JCConch::s_pConchRender->m_pRootNode->m_nID)
            {
                JCConch::s_pConchRender->m_pRootNode = NULL;
            }
            m_pCmdDispathManager->deleteCmdDispaths(pParam->nID);
            return true;
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_setContextType(JCMemClass& pRenderCmd)
    {
        ParamInt2* pParam = pRenderCmd.popp<ParamInt2>();
        if (pParam)
        {
            if (pParam->nID < m_pCmdDispathManager->m_vCmdDispaths.size())
            {
                JCHtml5Render* pHtml5Render = (JCHtml5Render*)(m_pCmdDispathManager->m_vCmdDispaths[pParam->nID]);
                pHtml5Render->setCanvasType( (CANVAS_TYPE)pParam->nType );
                if( pParam->nType == CANVAS_TYPE_MAIN )
                {
                    JCConch::s_pConchRender->m_pRootHtml5Render = pHtml5Render;
                }
            }
            return true;
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_setHtml5CanvasSize(JCMemClass& pRenderCmd)
    {
        ParamSetCanvasSize* pParam = pRenderCmd.popp<ParamSetCanvasSize>();
        if (pParam)
        {
             JCHtml5Render* pHtml5Render = (JCHtml5Render*)(m_pCmdDispathManager->m_vCmdDispaths[pParam->nID]);
             if (pHtml5Render)
             {
                 pHtml5Render->setCanvasSize(pParam->nWidth, pParam->nHeight);
             }
             return true;
        }
        return false;
    }

    bool JCPublicCmdDispath::_rendercmd_createVetexBuffer(JCMemClass& pRenderCmd)
    {
        ParamInt2 * pParm= pRenderCmd.popp<ParamInt2>();
        char * p_sBuffer =(char *)pRenderCmd.readBuffer(pParm->nType);
        if (m_pCmdDispathManager->m_pBufferManager)
        {
            m_pCmdDispathManager->m_pBufferManager->createOrUpdateBuffer(pParm->nID, pParm->nType, p_sBuffer);
        }
        else
        {
            LOGE("JCPublicCmdDispath::_rendercmd_createVetexBuffer m_pBufferManager is NULL");
        }
        return true;
    }

    bool JCPublicCmdDispath::_rendercmd_createShader(JCMemClass& pRenderCmd)
    {
        ParamInt2 * pParm = pRenderCmd.popp<ParamInt2>();
        char * p_sVs = pRenderCmd.readBuffer(pParm->nType);
        std::string strVS;
        strVS.append(p_sVs, pParm->nType);
        int nPsLength =*(int*)pRenderCmd.readBuffer(4);
        char * p_sPs = pRenderCmd.readBuffer(nPsLength);
        std::string strPS;
        strPS.append(p_sPs, nPsLength);
        if (m_pCmdDispathManager->m_pShaderManager)
        {
            m_pCmdDispathManager->m_pShaderManager->createOrUpdateShader(pParm->nID, (char*)strPS.c_str(),
                (char*)strVS.c_str());
        }
        else
        {
            LOGE("JCPublicCmdDispath::_rendercmd_createShader m_pShaderManager is NULL");
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_createGraphics(JCMemClass& pRenderCmd)
    {
        ParamInt2* pParam = pRenderCmd.popp<ParamInt2>();
        if (pParam)
        {
            JCGraphicsCmdDispath* pGraphicsCmdDispath = new JCGraphicsCmdDispath(pParam->nID,m_pCmdDispathManager->m_pTextureManager, m_pCmdDispathManager->m_pImageManager, m_pCmdDispathManager->m_p2DShaderManager, m_pCmdDispathManager->m_pHtml5RenderManager, m_pCmdDispathManager->m_pTextManager, m_pCmdDispathManager->m_pFontManager, m_pCmdDispathManager->m_pBufferManager, m_pCmdDispathManager->m_pShaderManager, (TEXT_BASE_LINE_TYPE)pParam->nType);
            m_pCmdDispathManager->pushCmdDispaths(pParam->nID, pGraphicsCmdDispath);
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_deleteGraphics(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)
        {
            JCGraphicsCmdDispath* pGraphicsCmdDispath = (JCGraphicsCmdDispath*)(m_pCmdDispathManager->m_vCmdDispaths[pParam->nID]);
            m_pCmdDispathManager->deleteCmdDispaths(pParam->nID);
            return true;
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_setRootNode(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)
        {
            JCNode2DCmdDispath* pNode2DDispath = (JCNode2DCmdDispath*)(m_pCmdDispathManager->m_vCmdDispaths[pParam->nID]);
            if(JCConch::s_pConchRender->m_pRootNode == NULL )
            {
                JCConch::s_pConchRender->m_pRootNode = pNode2DDispath->m_pNode2D;
            }
            else
            {
                LOGE("JCPublicCmdDispath::_rendercmd_setRootNode you already set root node");
            }
        }
        return true;
    }

    bool JCPublicCmdDispath::_rendercmd_setMainCanvasScaleInfo(JCMemClass& pRenderCmd) {
        int fid;
        float sx, sy, tx, ty;
        pRenderCmd.popv(fid);
        pRenderCmd.popv(sx);
        pRenderCmd.popv(sy);
        pRenderCmd.popv(tx);
        pRenderCmd.popv(ty);
        if (JCConch::s_pConchRender && 
            JCConch::s_pConchRender->m_pRootHtml5Render &&
            JCConch::s_pConchRender->m_pRootHtml5Render->m_pContext) {
            JCHtml5Context* pCtx = JCConch::s_pConchRender->m_pRootHtml5Render->m_pContext;
            pCtx->m_fScaleX = sx;
            pCtx->m_fScaleY = sy;
            pCtx->m_fTx = tx;
            pCtx->m_fTy = ty;
            //pCtx->setCanvasSize(g_nInnerWidth/sx, g_nInnerHeight/sy); 不用了，用实际大小
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_setMemoryCanvasClip(JCMemClass& pRenderCmd)
    {
        ParamInt2* pParam = pRenderCmd.popp<ParamInt2>();
        if (pParam)
        {
            if (pParam->nID < m_pCmdDispathManager->m_vCmdDispaths.size())
            {
                JCHtml5Render* pHtml5Render = (JCHtml5Render*)(m_pCmdDispathManager->m_vCmdDispaths[pParam->nID]);
                pHtml5Render->setMemoryCanvasClip( pParam->nType != 0 );
            }
            return true;
        }
        return true;
    }

    bool JCPublicCmdDispath::_rendercmd_setFPSNode(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)
        {
            JCNode2DCmdDispath* pNode2DDispath = (JCNode2DCmdDispath*)(m_pCmdDispathManager->m_vCmdDispaths[pParam->nID]);
            if (JCConch::s_pConchRender->m_pFPSNode2D == NULL)
            {
                JCConch::s_pConchRender->m_pFPSNode2D = pNode2DDispath->m_pNode2D;
            }
            else
            {
                LOGE("JCPublicCmdDispath::_rendercmd_setFPSNode you already set fps node");
            }
        }
        return true;
    }

    bool JCPublicCmdDispath::_rendercmd_setLoadingViewNode(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)
        {
            JCNode2DCmdDispath* pNode2DDispath = (JCNode2DCmdDispath*)(m_pCmdDispathManager->m_vCmdDispaths[pParam->nID]);
            if (JCConch::s_pConchRender->m_pLoadingViewNode2D == NULL)
            {
                JCConch::s_pConchRender->m_pLoadingViewNode2D = pNode2DDispath->m_pNode2D;
            }
            else
            {
                JCConch::s_pConchRender->m_pLoadingViewNode2D = NULL;
            }
        }
        return true;
    }

	bool JCPublicCmdDispath::_rendercmd_createParticalTemplate2D(JCMemClass& pRenderCmd)
	{
		ParamInt* pParam = pRenderCmd.popp<ParamInt>();
		if (pParam)
		{
			JCParticleTemplate2DCmdDispath* pParticleTemplate2DCmdDispath = new JCParticleTemplate2DCmdDispath(pParam->nID);
			m_pCmdDispathManager->pushCmdDispaths(pParam->nID, pParticleTemplate2DCmdDispath);
		}
		return true;
	}

	bool JCPublicCmdDispath::_rendercmd_deleteParticalTemplate2D(JCMemClass& pRenderCmd)
	{
		ParamInt* pParam = pRenderCmd.popp<ParamInt>();
		if (pParam)
		{
			JCParticleTemplate2DCmdDispath* pParticleTemplate2DCmdDispath = (JCParticleTemplate2DCmdDispath*)(m_pCmdDispathManager->m_vCmdDispaths[pParam->nID]);
			m_pCmdDispathManager->deleteCmdDispaths(pParam->nID);
			return true;
		}
		return true;
	}
	bool JCPublicCmdDispath::_rendercmd_registShaderDefine(JCMemClass& pRenderCmd)
	{
		ParamInt* pParam= pRenderCmd.popp<ParamInt>();
		char* name = pRenderCmd.readBuffer(pParam->nID);
		long long nID = *(pRenderCmd.popp<long long>());
		JCShaderDefine::registShaderDefine(name, nID);
		return true;
	}
    bool JCPublicCmdDispath::_rendercmd_setTransparentMode(JCMemClass& pRenderCmd)
    {
        ParamFloat* pParam = pRenderCmd.popp<ParamFloat>();
        if (pParam)
        {
            JCConch::s_pConchRender->setTransparentMode(pParam->value);
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_setImageReleaseSpaceTime(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)
        {
            JCConch::s_pConchRender->m_pImageManager->setReleaseSpaceTime(pParam->nID);
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_PerfAddData(JCMemClass& pRenderCmd)
    {
        ParamPerfAddData* pParam = pRenderCmd.popp<ParamPerfAddData>();
        if (pParam)
        {
            JCPerfHUD::addData(pParam->nID, pParam->nColor, pParam->fScale, pParam->fAlert);
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_PerfUpdateDt(JCMemClass& pRenderCmd)
    {
        PerfUpdateDt* pParam = pRenderCmd.popp<PerfUpdateDt>();
        if (pParam)
        {
            JCPerfHUD::updateData(pParam->nID, pParam->fTimeSpace);
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_ImageEnableMerageInAtlas(JCMemClass& pRenderCmd)
    {
        ParamInt2* pParam = pRenderCmd.popp<ParamInt2>();
        if (pParam)
        {
            if (m_pCmdDispathManager->m_pImageManager)
            {
                JCImage* pImage = m_pCmdDispathManager->m_pImageManager->getImage(pParam->nID);
                pImage->m_bEnableMerageInAtlas = (pParam->nType > 0);
            }
            return true;
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_ImageReleaseTexture(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)
        {
            if (m_pCmdDispathManager->m_pImageManager)
            {
                JCImage* pImage = m_pCmdDispathManager->m_pImageManager->getImage(pParam->nID);
                pImage->releaseTexture();
            }
            return true;
        }
        return true;
    }
    bool JCPublicCmdDispath::_rendercmd_deleteVetexBuffer(JCMemClass& pRenderCmd)
    {
        ParamInt* pParam = pRenderCmd.popp<ParamInt>();
        if (pParam)
        {
            if (m_pCmdDispathManager->m_pBufferManager)
            {
                m_pCmdDispathManager->m_pBufferManager->deleteBuffer(pParam->nID);
            }
            return true;
        }
        return true;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------