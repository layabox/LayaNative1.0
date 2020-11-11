/**
@file			JCCmdDispatchManager.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_27
*/

#include "JCCmdDispatchManager.h"
#include "JCPublicCmdDispath.h"
#include <util/Log.h>
#include <3DRS/JCMaterialManager.h>
#include "JCConchRender.h"

//------------------------------------------------------------------------------
namespace laya
{
    JCCmdDispathManager::JCCmdDispathManager(JCResManager* pTextureManager,JCImageManager* pImageManager,JCTextMemoryCanvas* pTextMemoryCanvas, JC2DShaderManager* p2DShaderManager,WebGLRenderingContext* pWebGLContext, 
        JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager, JCBufferManager* pBufferManager, JCShaderManager* pShaderManager)
    {
        m_pTextManager = pTextManager;
        m_pFontManager = pFontManager;
        m_pTextureManager = pTextureManager;
        m_p2DShaderManager = p2DShaderManager;
        m_pHtml5RenderManager = pHtml5RenderManager;
        m_pTextMemoryCanvas = pTextMemoryCanvas;
        m_pImageManager = pImageManager;
        m_pWebGLContext = pWebGLContext;
        m_pShaderManager = pShaderManager;
        m_pBufferManager = pBufferManager;
        JCPublicCmdDispath* pCmdPublicDispath = new JCPublicCmdDispath(this);
        pushCmdDispaths(PUBLIC_CMD_DISPATH, pCmdPublicDispath);
    }
    void JCCmdDispathManager::reset()
    {
        for (VectorCmdDispath::iterator iter = m_vCmdDispaths.begin(); iter != m_vCmdDispaths.end(); iter++)
        {
            JCICmdDispatch* pCmdDispath = *iter;
            if (pCmdDispath)
            {
                delete pCmdDispath;
                pCmdDispath = NULL;
            }
        }
        m_vCmdDispaths.clear();
        JCPublicCmdDispath* pCmdPublicDispath = new JCPublicCmdDispath(this);
        pushCmdDispaths(PUBLIC_CMD_DISPATH, pCmdPublicDispath);
    }
    JCCmdDispathManager::~JCCmdDispathManager()
    {
        for (VectorCmdDispath::iterator iter = m_vCmdDispaths.begin(); iter != m_vCmdDispaths.end(); iter++)
        {
            JCICmdDispatch* pCmdDispath = *iter;
            if (pCmdDispath)
            {
                delete pCmdDispath;
                pCmdDispath = NULL;
            }
        }
        m_vCmdDispaths.clear();
    }
    bool JCCmdDispathManager::pushCmdDispaths( int nID,JCICmdDispatch* pCmdDispatch )
    {
        if ( (int)m_vCmdDispaths.size() == nID)
        {
            m_vCmdDispaths.push_back(pCmdDispatch);
        }
        else if( nID < (int)m_vCmdDispaths.size() )
        {
            if(m_vCmdDispaths[nID])
            {
                delete m_vCmdDispaths[nID];
            }
            m_vCmdDispaths[nID] = pCmdDispatch;
        }
        else
        {
            m_vCmdDispaths.resize(nID + 1);
            m_vCmdDispaths[nID] = pCmdDispatch;
        }
        return true;
    }
    bool JCCmdDispathManager::deleteCmdDispaths(int nID)
    {
        if (nID > 0 && nID < (int)m_vCmdDispaths.size())
        {
            if (m_vCmdDispaths[nID])
            {
                delete m_vCmdDispaths[nID];
                m_vCmdDispaths[nID] = NULL;
                return true;
            }
        }
        return false;
    }
    bool JCCmdDispathManager::dispatchRenderCmd(JCMemClass& pRenderCmd,int nFrameCount)
    {
        while (true)
        {
            char* pData = pRenderCmd.getReadPtr();
            if (pData)
            {
                int nID = *(pRenderCmd.popp<int>());
                if ( nID >= 0 && nID < (int)m_vCmdDispaths.size())
                {
                    JCICmdDispatch* pCmdDispath = m_vCmdDispaths[nID];
                    if (pCmdDispath)
                    {
                        pCmdDispath->dispatchRenderCmd(pRenderCmd,nFrameCount);
                    }
                }
                else
                {
                    LOGE("JCCmdDispathManager::dispatchRenderCmd dispatchRenderCmd id error");
                }
            }
            else
            {
                break;
            }
        }
        return true;
    }
    bool JCCmdDispathManager::dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount)
    {
        while (true)
        {
            char* pData = pRenderCmd.getReadPtr();
            if (pData)
            {
                int nID = *(pRenderCmd.popp<int>());
                if (nID >= 0 && nID < (int)m_vCmdDispaths.size())
                {
                    JCICmdDispatch* pCmdDispath = m_vCmdDispaths[nID];
                    if (pCmdDispath)
                    {
                        pCmdDispath->dispatchRenderCmdWhenLostedDevice(pRenderCmd, nFrameCount);
                    }
                }
                else
                {
                    LOGE("JCCmdDispathManager::dispatchRenderCmdWhenLostedDevice id error");
                }
            }
            else
            {
                break;
            }
        }
        return true;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
