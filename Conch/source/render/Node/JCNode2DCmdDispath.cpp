/**
@file			JCNode2DCmdDispath.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_14
*/

#include "JCNode2DCmdDispath.h"
#include <resource/JCResManager.h>
#include "../Image/JCImageManager.h"
#include <3DRS/JC2DShaderManager.h>
#include "../Html5Render/JCHtml5RenderManager.h"
#include <resource/text/JCTextManager.h>
#include <resource/text/JCFontManager.h>
#include "../Manager/JCBufferManager.h"
#include "../Manager/JCShaderManager.h"
#include <util/Log.h>
#include <util/JCCmdStruct.h>
#include "JCGraphicsCmdDispath.h"
#include "JCGraphics.h"
#include "../Html5Render/JCHtml5Render.h"

//------------------------------------------------------------------------------
namespace laya
{
    JCNode2DCmdDispath::JCNode2DCmdDispath(int nID,JCResManager* pTextureManager, JCImageManager* pImageManager, JC2DShaderManager* p2DShaderManager,
        JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager,
        JCBufferManager* pBufferManager, JCShaderManager* pShaderManager, std::vector<JCICmdDispatch*>* vVectorCmdDispath)
    {
        m_vVectorCmdDispath = vVectorCmdDispath;
        m_pNode2D = new JCNode2D(nID,pTextureManager, pImageManager, p2DShaderManager, pHtml5RenderManager, pTextManager,
            pFontManager, pBufferManager, pShaderManager, m_vVectorCmdDispath);
    }
    JCNode2DCmdDispath::~JCNode2DCmdDispath()
    {
        if (m_pNode2D)
        {
            delete m_pNode2D;
            m_pNode2D = NULL;
        }
    }
    bool JCNode2DCmdDispath::restartJSThread()
    {
        return true;
    }
    bool JCNode2DCmdDispath::dispatchRenderCmd(JCMemClass& pRenderCmd, int nFrameCount)
    {
        static const ProcFunction g_svProcFunctions[] =
        {
            &JCNode2DCmdDispath::_rendercmd_addChildAt,
            &JCNode2DCmdDispath::_rendercmd_removeChild,
            &JCNode2DCmdDispath::_rendercmd_size,
            &JCNode2DCmdDispath::_rendercmd_pos,
            &JCNode2DCmdDispath::_rendercmd_pivot,
            &JCNode2DCmdDispath::_rendercmd_scale,
            &JCNode2DCmdDispath::_rendercmd_skew,
            &JCNode2DCmdDispath::_rendercmd_rotate,
            &JCNode2DCmdDispath::_rendercmd_bgColor,
            &JCNode2DCmdDispath::_rendercmd_font,
            &JCNode2DCmdDispath::_rendercmd_text,
            &JCNode2DCmdDispath::_rendercmd_transform,
            &JCNode2DCmdDispath::_rendercmd_setTransform,
            &JCNode2DCmdDispath::_rendercmd_alpha,
            &JCNode2DCmdDispath::_rendercmd_filter,
            &JCNode2DCmdDispath::_rendercmd_visible,
            &JCNode2DCmdDispath::_rendercmd_blendMode,
            &JCNode2DCmdDispath::_rendercmd_scrollRect,
            &JCNode2DCmdDispath::_rendercmd_mask,
            &JCNode2DCmdDispath::_rendercmd_graphics,
            &JCNode2DCmdDispath::_rendercmd_custom,
            &JCNode2DCmdDispath::_rendercmd_removeType,
            &JCNode2DCmdDispath::_rendercmd_cacheAs,
            &JCNode2DCmdDispath::_rendercmd_border,
            &JCNode2DCmdDispath::_rendercmd_optimizeScrollRect,
            &JCNode2DCmdDispath::_rendercmd_blurFilter,
            &JCNode2DCmdDispath::_rendercmd_glowFilter,
            &JCNode2DCmdDispath::_rendercmd_filterMatrix,
            &JCNode2DCmdDispath::_rendercmd_repaint,
			&JCNode2DCmdDispath::_rendercmd_setZOrder,
			&JCNode2DCmdDispath::_rendercmd_updateZOrder,
            &JCNode2DCmdDispath::_rendercmd_drawToCanvas
        };
        static const int nFuncs = sizeof(g_svProcFunctions) / sizeof(g_svProcFunctions[0]);
        char* pData = pRenderCmd.getReadPtr();
        if (pData)
        {
            int nFunctionID = *(int*)pData;
            if (nFunctionID >= 0 && nFunctionID < nFuncs)
            {
                (this->*g_svProcFunctions[nFunctionID])(pRenderCmd);
            }
            else
            {
                LOGE("JCNode2DCmdDispath::dispatchRenderCmd Wrong command number!,cmd=%d", nFunctionID);
            }
        }
        return true;
    }
    bool JCNode2DCmdDispath::dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount)
    {
        return dispatchRenderCmd(pRenderCmd, nFrameCount);
    }
    void JCNode2DCmdDispath::_rendercmd_addChildAt(JCMemClass& cmdBuffer)
    {
        CMDInt2* cmdInt2 = cmdBuffer.popp<CMDInt2>();
        if (cmdInt2->x < m_vVectorCmdDispath->size())
        {
            JCNode2DCmdDispath* pNodeDispath = (JCNode2DCmdDispath*)(m_vVectorCmdDispath->operator[](cmdInt2->x));
            m_pNode2D->addChildAt(pNodeDispath->m_pNode2D, cmdInt2->y);
        }
        else
        {
            LOGE("JCNode2DCmdDispath::_rendercmd_addChildAt error can't find the node id=%d", cmdInt2->x);
        }
    }
    void JCNode2DCmdDispath::_rendercmd_removeChild(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt= cmdBuffer.popp<CMDInt>();
        if (cmdInt->i < m_vVectorCmdDispath->size())
        {
            JCNode2DCmdDispath* pNodeDispath = (JCNode2DCmdDispath*)(m_vVectorCmdDispath->operator[](cmdInt->i));
            m_pNode2D->removeChild(pNodeDispath->m_pNode2D);
        }
        else
        {
            LOGE("JCNode2DCmdDispath::_rendercmd_removeChild error can't find the node id=%d", cmdInt->i);
        }
    }
    void JCNode2DCmdDispath::_rendercmd_size(JCMemClass& cmdBuffer)
    {
        CMDFloatPoint* cmdFloatPoint = cmdBuffer.popp<CMDFloatPoint>();
        m_pNode2D->size(cmdFloatPoint->x,cmdFloatPoint->y);
    }
    void JCNode2DCmdDispath::_rendercmd_pos(JCMemClass& cmdBuffer)
    {
        CMDFloatPoint* cmdFloatPoint = cmdBuffer.popp<CMDFloatPoint>();
        m_pNode2D->pos(cmdFloatPoint->x, cmdFloatPoint->y);
    }
    void JCNode2DCmdDispath::_rendercmd_pivot(JCMemClass& cmdBuffer)
    {
        CMDFloatPoint* cmdFloatPoint = cmdBuffer.popp<CMDFloatPoint>();
        m_pNode2D->pivot(cmdFloatPoint->x, cmdFloatPoint->y);
    }
    void JCNode2DCmdDispath::_rendercmd_scale(JCMemClass& cmdBuffer)
    {
        CMDFloatPoint* cmdFloatPoint = cmdBuffer.popp<CMDFloatPoint>();
        m_pNode2D->scale(cmdFloatPoint->x, cmdFloatPoint->y);
    }
    void JCNode2DCmdDispath::_rendercmd_skew(JCMemClass& cmdBuffer)
    {
        CMDFloatPoint* cmdFloatPoint = cmdBuffer.popp<CMDFloatPoint>();
        m_pNode2D->skew(cmdFloatPoint->x, cmdFloatPoint->y);
    }
    void JCNode2DCmdDispath::_rendercmd_rotate(JCMemClass& cmdBuffer)
    {
        CMDFloat * cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pNode2D->rotate(cmdFloat->f);
    }
    void JCNode2DCmdDispath::_rendercmd_bgColor(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pNode2D->bgColor(cmdInt->i);
    }
    void JCNode2DCmdDispath::_rendercmd_font(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        char* sFont = cmdBuffer.readBuffer(cmdInt->i);
        m_pNode2D->font(sFont);
    }
    void JCNode2DCmdDispath::_rendercmd_text(JCMemClass& cmdBuffer)
    {
    }
    void JCNode2DCmdDispath::_rendercmd_transform(JCMemClass& cmdBuffer)
    {
        CMDMatrix* cmdMatrix = cmdBuffer.popp<CMDMatrix>();
        m_pNode2D->transform(cmdMatrix->a, cmdMatrix->b, cmdMatrix->c, cmdMatrix->d, cmdMatrix->tx, cmdMatrix->ty);
    }
    void JCNode2DCmdDispath::_rendercmd_setTransform(JCMemClass& cmdBuffer)
    {
        CMDMatrix* cmdMatrix = cmdBuffer.popp<CMDMatrix>();
        m_pNode2D->setTransform(cmdMatrix->a, cmdMatrix->b, cmdMatrix->c, cmdMatrix->d, cmdMatrix->tx, cmdMatrix->ty);
    }
    void JCNode2DCmdDispath::_rendercmd_alpha(JCMemClass& cmdBuffer)
    {
        CMDFloat * cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pNode2D->alpha(cmdFloat->f);
    }
    void JCNode2DCmdDispath::_rendercmd_filter(JCMemClass& cmdBuffer)
    {
        CMDFloatRect *cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
        m_pNode2D->filter(cmdFloatRect->x, cmdFloatRect->y, cmdFloatRect->w, cmdFloatRect->h);
    }
    void JCNode2DCmdDispath::_rendercmd_filterMatrix(JCMemClass& cmdBuffer)
    {
        CMDFilterMatrix* cmdFilter = cmdBuffer.popp<CMDFilterMatrix>();
        m_pNode2D->filterMatrix(cmdFilter->matrix, cmdFilter->alpha);
    }
    void JCNode2DCmdDispath::_rendercmd_visible(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pNode2D->visible(!!cmdInt->i);
    }
    void JCNode2DCmdDispath::_rendercmd_blendMode(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pNode2D->blendMode(cmdInt->i);
    }
    void JCNode2DCmdDispath::_rendercmd_scrollRect(JCMemClass& cmdBuffer)
    {
        CMDFloatRect *cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
        m_pNode2D->scrollRect(cmdFloatRect->x, cmdFloatRect->y, cmdFloatRect->w, cmdFloatRect->h);
    }
    void JCNode2DCmdDispath::_rendercmd_mask(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        if (cmdInt->i < m_vVectorCmdDispath->size())
        {
            JCNode2DCmdDispath* pNodeDispath = (JCNode2DCmdDispath*)(m_vVectorCmdDispath->operator[](cmdInt->i));
            m_pNode2D->mask(pNodeDispath->m_pNode2D);
        }
        else
        {
            m_pNode2D->mask(NULL);
        }
    }
    void JCNode2DCmdDispath::_rendercmd_graphics(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();

        if (cmdInt->i < m_vVectorCmdDispath->size())
        {
            JCGraphicsCmdDispath* pGraphicsDispath = (JCGraphicsCmdDispath*)(m_vVectorCmdDispath->operator[](cmdInt->i));
            m_pNode2D->graphics(pGraphicsDispath->m_pGraphics);
        }
        else
        {
            m_pNode2D->graphics(NULL);
            LOGE("JCNode2DCmdDispath::_rendercmd_graphics error can't find the graphics id=%d", cmdInt->i);
        }
    }
    void JCNode2DCmdDispath::_rendercmd_custom(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        if (cmdInt->i < m_vVectorCmdDispath->size())
        {
            JCHtml5Render* pHtml5Render = (JCHtml5Render*)(m_vVectorCmdDispath->operator[](cmdInt->i));
            if (pHtml5Render != NULL && pHtml5Render->m_pContext)
            {
                m_pNode2D->custom(pHtml5Render->m_pContext->m_pSaveCmd);
            }
        }
        else
        {
            m_pNode2D->custom(NULL);
            LOGE("JCNode2DCmdDispath::_rendercmd_custom error can't find the graphics id=%d", cmdInt->i);
        }
    }
    void JCNode2DCmdDispath::_rendercmd_removeType(JCMemClass&cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pNode2D->removeType(cmdInt->i);
    }
    void JCNode2DCmdDispath::_rendercmd_cacheAs(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pNode2D->cacheAs((CANVAS_TYPE)(cmdInt->i));
    }

    void JCNode2DCmdDispath::_rendercmd_border(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pNode2D->border(cmdInt->i);
    }

    void JCNode2DCmdDispath::_rendercmd_optimizeScrollRect(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pNode2D->optimizeScrollRect(!!cmdInt->i);
    }

    void JCNode2DCmdDispath::_rendercmd_blurFilter(JCMemClass& cmdBuffer)
    {
        CMDFloat *cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pNode2D->blurFilter(cmdFloat->f);
    }

    void JCNode2DCmdDispath::_rendercmd_glowFilter(JCMemClass& cmdBuffer)
    {
        CMDGlowFilterParam* cmdGlowFilterParam= cmdBuffer.popp<CMDGlowFilterParam>();
        m_pNode2D->glowFilter(cmdGlowFilterParam->color, cmdGlowFilterParam->blur, cmdGlowFilterParam->offX, cmdGlowFilterParam->offY);
    }

    void JCNode2DCmdDispath::_rendercmd_repaint(JCMemClass& cmdBuffer)
    {
        CMDNoParam* cmdNoParam= cmdBuffer.popp<CMDNoParam>();
        m_pNode2D->repaint();
    }

	void JCNode2DCmdDispath::_rendercmd_setZOrder(JCMemClass& cmdBuffer)
	{
		CMDFloat* cmdFloat = cmdBuffer.popp<CMDFloat>();
		m_pNode2D->m_nZOrder = cmdFloat->f;
	}

	void JCNode2DCmdDispath::_rendercmd_updateZOrder(JCMemClass& cmdBuffer)
	{
		CMDNoParam* cmdNoParam = cmdBuffer.popp<CMDNoParam>();
		m_pNode2D->updateOrder();
	}
    void JCNode2DCmdDispath::_rendercmd_drawToCanvas(JCMemClass& cmdBuffer)
    {
        CMDDrawToCanvas* cmdParams = cmdBuffer.popp<CMDDrawToCanvas>();
        if (cmdParams->canvasID < m_vVectorCmdDispath->size())
        {
            JCHtml5Render* pHtml5Render = (JCHtml5Render*)(m_vVectorCmdDispath->operator[](cmdParams->canvasID));
            m_pNode2D->drawToCanvas(pHtml5Render, cmdParams->offsetX, cmdParams->offsetY);
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------