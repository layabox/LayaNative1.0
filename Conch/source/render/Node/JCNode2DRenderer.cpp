/**
@file			JCNode2DRenderer.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_13
*/

#include "JCNode2DRenderer.h"
#include "JCNode2D.h"
#include "../Html5Render/JCHtml5Context.h"
#include "../Html5Render/JCHtml5Render.h"
#include <util/Log.h>
#include <resource/DisplayRes/JCTexture.h>

const float biasXY = 4.0f;//避免“缝隙”
const float biasWH = 8.0f;//避免“缝隙”

//------------------------------------------------------------------------------
namespace laya
{
    #define CANVAS_EXPAN_POS 16.0f
    #define CANVAS_EXPAN_SIZE 32.0f
    JCNode2DRenderer::VectorRenderers JCNode2DRenderer::m_vVectorRenderers;
    JCNode2DRenderer* JCNode2DRenderer::NORENDER = new JCNode2DRenderer(NRT_NO,NULL);
    JCNode2DRenderer* JCNode2DRenderer::INITRENDER = new JCNode2DRenderer(NRT_INIT, NULL);
    void JCNode2DRenderer::initSame(int* pArray, int nArrayLen, JCNode2DRenderer* pRenderer)
    {
        int n = 0;
        for (int i = 0; i < nArrayLen; i++)
        {
            n |= pArray[i];
            m_vVectorRenderers[n] = pRenderer;
        }
    }
    bool inVec(std::vector<JCNode2DRenderer*>& vec, JCNode2DRenderer* pRenderer) {
        for (auto i : vec) {
            if (i == pRenderer)
                return true;
        }
        return false;
    }
    void JCNode2DRenderer::clearAll()
    {
        std::vector<JCNode2DRenderer*> vDeled;
        for (auto& cur : m_vVectorRenderers){
            if ( cur != INITRENDER && cur != NORENDER)
            {
                if (cur && !inVec(vDeled, cur)) {
                    //printf("f=%x", cur);
                    delete cur;
                    vDeled.push_back(cur);
                }
            }
            cur = nullptr;
        }
        m_vVectorRenderers.clear();
    }
    void JCNode2DRenderer::initAll()
    {
        ///clearAll();
        int nLen = NRT_CHILDS * 2;
        m_vVectorRenderers.resize(nLen);
        for (int i = 0; i < nLen; i++)
        {
            m_vVectorRenderers[i] = INITRENDER;
        }
        m_vVectorRenderers[0] = NORENDER;

        //这几个特殊的标志位
        int nArray[] = { NRT_IMAGE, NRT_GRAPHICS, NRT_TRANSFORM, NRT_ALPHA };
        initSame( &nArray[0],4, new JCNode2DRenderer(NRT_IMAGE, NULL));
        m_vVectorRenderers[NRT_IMAGE | NRT_GRAPHICS] = new JCNode2DRenderer(NRT_IMAGE | NRT_GRAPHICS, NULL);
        m_vVectorRenderers[NRT_IMAGE | NRT_TRANSFORM | NRT_GRAPHICS] = new JCNode2DRenderer(NRT_IMAGE | NRT_TRANSFORM | NRT_GRAPHICS, NULL );
    }
    JCNode2DRenderer* JCNode2DRenderer::getTypeRenderer(int type)
    {
        JCNode2DRenderer* pResultRenderer = NULL;
        int tType = NRT_CHILDS;
        while (tType > 1)
        {
            if (tType & type)
            {
                pResultRenderer = new JCNode2DRenderer(tType, pResultRenderer);
            }
            tType = tType >> 1;
        }
        return pResultRenderer;
    }
    JCNode2DRenderer::JCNode2DRenderer(int nType, JCNode2DRenderer* pNext)
    {
        m_pNext = pNext ? pNext : NORENDER;
        switch (nType) 
        {
        case NRT_NO:
            m_pFunction = &JCNode2DRenderer::no;
            return;
        case NRT_IMAGE:
            m_pFunction = &JCNode2DRenderer::image;
            return;
        case NRT_ALPHA:
            m_pFunction = &JCNode2DRenderer::alpha;
            return;
        case NRT_TRANSFORM:
            m_pFunction = &JCNode2DRenderer::transform;
            return;
        case NRT_BLEND:
            m_pFunction = &JCNode2DRenderer::blend;
            return;
        case NRT_CANVAS:
            m_pFunction = &JCNode2DRenderer::canvas;
            return;
		case NRT_MASK:
			m_pFunction = &JCNode2DRenderer::mask;
			return;
        case NRT_CLIP:
            m_pFunction = &JCNode2DRenderer::clip;
            return;
        case NRT_STYLE:
            m_pFunction = &JCNode2DRenderer::style;
            return;
        case NRT_GRAPHICS:
            m_pFunction = &JCNode2DRenderer::graphics;
            return;
        case NRT_CHILDS:
            m_pFunction = &JCNode2DRenderer::childs;
            return;
        case NRT_CUSTOM:
            m_pFunction = &JCNode2DRenderer::custom;
            return; 
        case NRT_IMAGE | NRT_GRAPHICS:
            m_pFunction = &JCNode2DRenderer::image2;
            return;
        case NRT_IMAGE | NRT_TRANSFORM | NRT_GRAPHICS:
            m_pFunction = &JCNode2DRenderer::image2;
            return;
        case NRT_FILTERS:
            m_pFunction = &JCNode2DRenderer::colorFilter;
            return;
        case NRT_INIT:
            m_pFunction = &JCNode2DRenderer::initRenderFunction;
            return;
        }
    }
    JCNode2DRenderer::~JCNode2DRenderer()
    {
        /*
        clearAll();
        if( NORENDER )
        {
            delete NORENDER;
            NORENDER = NULL;
        }
        if( INITRENDER )
        {
            delete INITRENDER;
            INITRENDER = NULL;
        }
         */
    }
    void JCNode2DRenderer::runRender(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        (this->*m_pFunction)(pNode, pRenderContext, x, y);
    }
    void JCNode2DRenderer::initRenderFunction(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        int nType = pNode->m_nRenderType;
        JCNode2DRenderer* pRenderer = getTypeRenderer(nType);
        m_vVectorRenderers[nType] = pRenderer;
        (pRenderer->*pRenderer->m_pFunction)(pNode, pRenderContext, x, y);
    }
    void JCNode2DRenderer::style(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        pRenderContext->save();
        JCTransform* pTransfrom = pNode->m_kNodeStyle.m_pTransform;

        if (pNode->m_kNodeStyle.m_nType&JCNodeStyle::BG_BGCOLOR)
        {
            pRenderContext->setFillStyle(pNode->m_kNodeStyle.m_nBgColor);
            pRenderContext->fillRect(x - pTransfrom->pivotX, y - pTransfrom->pivotY, pNode->m_nWidth, pNode->m_nHeight);
        }
        if (pNode->m_kNodeStyle.m_nType&JCNodeStyle::BG_BORDERCOLOR)
        {
            pRenderContext->setStrokeStyle(pNode->m_kNodeStyle.m_nBorderColor);
            pRenderContext->strokeRect(x - pTransfrom->pivotX, y - pTransfrom->pivotY, pNode->m_nWidth, pNode->m_nHeight);
        }
        //这个不是调用next->next函数，其实m_pNext->m_pFunction 是函数指针
        (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
        pRenderContext->restore();
    }
    void JCNode2DRenderer::no(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {

    }
    void JCNode2DRenderer::custom(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        JCMemClass* pCustomData = pNode->m_pCustomData;
        if (pCustomData)
        {
            JCTransform* pTransfrom = pNode->m_kNodeStyle.m_pTransform;
            pRenderContext->save();
            pRenderContext->translate(x - pTransfrom->pivotX, y - pTransfrom->pivotY);
            //从头开始读取
            pCustomData->setReadPos(0);
            while (true)
            {
                char* pData = pCustomData->getReadPtr();
                if (pData)
                {
                    pRenderContext->m_pHtml5Render->runRenderCmd(*pCustomData);
                }
                else
                {
                    break;
                }
            }
            pRenderContext->restore();
        }
        (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
    }
    void JCNode2DRenderer::clip(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        if (m_pNext == NORENDER)return;
        Rectangle* pRect = pNode->m_kNodeStyle.m_pClipRect;
        if (pRect)
        {
            pRenderContext->save();
            pRenderContext->clipRect(x, y, pRect->width, pRect->height);
            (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x - pRect->x, y - pRect->y);
            pRenderContext->restore();
        }
        else
        {
            (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
        }
    }
	void JCNode2DRenderer::mask(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
	{
		(m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
		if (pNode->m_pMask)
		{
            JCNode2D* pMaskNode = pNode->m_pMask;
            pMaskNode->cacheAs(CANVAS_TYPE_RENDER_TARGET);
            pMaskNode->m_nRenderType &= ~NRT_CANVAS;
            JCTransform* tf = pMaskNode->m_kNodeStyle.m_pTransform;

            JCHtml5Context* pCacheContext = pMaskNode->m_pCacheCanvas->m_pContext;
            Rectangle kRect;
            pMaskNode->getParentBound(kRect);
            kRect.intersectionThis(&pNode->m_kBoundingBox);
            kRect.width += CANVAS_EXPAN_SIZE;
            kRect.height += CANVAS_EXPAN_SIZE;
            if (kRect.width > 2048 || kRect.height > 2048)
            {
                LOGE("JCNode2DRenderer::mask error size > 2048");
                pCacheContext->setCanvasType(CANVAS_TYPE_MEMORY);
            }
            else
            {
                pCacheContext->setCanvasSize(kRect.width, kRect.height);
                pCacheContext->m_pContextData->reset(kRect.width, kRect.height);
                pCacheContext->clearSaveCmd();
                float nX = CANVAS_EXPAN_POS - kRect.x;
                float nY = CANVAS_EXPAN_POS - kRect.y;
                pMaskNode->render(pCacheContext, nX, nY);

                float fMaskSelfCanvasW = pCacheContext->m_nWidth - CANVAS_EXPAN_SIZE + biasWH;
                float fMaskSelfCanvasH = pCacheContext->m_nHeight - CANVAS_EXPAN_SIZE + biasWH;
                float fMaskSelfX = x - pNode->m_kNodeStyle.m_pTransform->pivotX + kRect.x - biasXY;
                float fMaskSelfY = y - pNode->m_kNodeStyle.m_pTransform->pivotY + kRect.y - biasXY;

                pRenderContext->save();
                pRenderContext->setGlobalCompositeOperation(BLEND_TYPE_MASK);
                pRenderContext->xDrawRenderTarget(pCacheContext, CANVAS_EXPAN_POS - biasXY, CANVAS_EXPAN_POS - biasXY, fMaskSelfCanvasW, fMaskSelfCanvasH,
                fMaskSelfX, fMaskSelfY, fMaskSelfCanvasW, fMaskSelfCanvasH);
                pRenderContext->restore(); 
            }
		}
	}
    void JCNode2DRenderer::blend(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        ContextDataInfo* pCurrentContextData = pRenderContext->m_pContextData->m_pCurrentContextData;
        int nSaveBlendType = pCurrentContextData->nBlendType;
        pCurrentContextData->nBlendType = pNode->m_kNodeStyle.m_nBlendMode;
        (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
        pCurrentContextData->nBlendType = nSaveBlendType;
        //TODO以后再优化 想着如果 node没有儿子，并且只有一条指令，mask不需要创建画布了
        /*
        ContextDataInfo* pCurrentContextData = pRenderContext->m_pContextData->m_pCurrentContextData;
        if (pNode->m_pMask)
        {
            pCurrentContextData->nBlendType = BLEND_TYPE_SOURCE_OVER;
            //pNode->m_pMask->cacheAs(CANVAS_TYPE_RENDER_TARGET);
            pNode->m_pMask->render(pRenderContext, x, y);
        }
        int nSaveBlendType = pCurrentContextData->nBlendType;
        pCurrentContextData->nBlendType = BLEND_TYPE_SOURCE_IN;
        (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
        pCurrentContextData->nBlendType = nSaveBlendType;
        */
    }
    void JCNode2DRenderer::colorFilter(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        JCFilterMatrix* pFilterMatrix = pNode->m_kNodeStyle.m_pFilterMatrix;
        if (pFilterMatrix)
        {
            pRenderContext->save();
            pRenderContext->setFilterMatrix(pFilterMatrix->matrix, pFilterMatrix->alpha );
            (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
            pRenderContext->restore();
        }
        else 
        {
            JCFilter* pFilter = pNode->m_kNodeStyle.m_pFilter;
            if (pFilter)
            {
                ContextDataInfo* pCurrentContextData = pRenderContext->m_pContextData->m_pCurrentContextData;
                int nSaveType = pCurrentContextData->nFilterType;
                JCFilter kFilter = pCurrentContextData->kFilter;
                pCurrentContextData->kFilter = *pFilter;
                pCurrentContextData->nFilterType = FILTER_TYPE_COLOR;
                (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
                pCurrentContextData->kFilter = kFilter;
                pCurrentContextData->nFilterType = nSaveType;
            }
            else
            {
                (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
            }
        }
    }
    void JCNode2DRenderer::graphics(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        if (pNode->m_pGraphics)
        {
            JCTransform* pTransfrom = pNode->m_kNodeStyle.m_pTransform;
            pNode->m_pGraphics->renderGraphics(pRenderContext,x- pTransfrom->pivotX,y-pTransfrom->pivotY );
        }
        (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
    }
    void JCNode2DRenderer::image(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        image2(pNode, pRenderContext, x, y);
    }
    void JCNode2DRenderer::image2(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        if (pNode->m_pGraphics && pNode->m_pGraphics->m_nImageId != -1)
        {
            JCNodeStyle* pStyle = &pNode->m_kNodeStyle;
            ContextDataInfo* pCurrentContextData = pRenderContext->m_pContextData->m_pCurrentContextData;
            Matrix32* pMatrix = NULL;
            if((pNode->m_nRenderType & NRT_TRANSFORM) == NRT_TRANSFORM)
            {
                pMatrix=pNode->getTransform();
            }
            float pivotX = pStyle->m_pTransform->pivotX;
            float pivotY = pStyle->m_pTransform->pivotY;
           
            if (pMatrix)
            {
                pRenderContext->save();
                pCurrentContextData = pRenderContext->m_pContextData->m_pCurrentContextData;
                if (pStyle->m_nBlendMode != BLEND_TYPE_NONE)
                {
                    pCurrentContextData->nBlendType = pStyle->m_nBlendMode;
                }
                float* m = pMatrix->m;
                pRenderContext->transform(m[0], m[1], m[2], m[3], m[4] + x, m[5] + y);
                float* args = pNode->m_pGraphics->m_pDrawImageArgs;
                pCurrentContextData->nAlpha *= (pStyle->m_fAlpha * args[8]);
                if (args[9] == 0)
                {
                    pRenderContext->drawImage(pNode->m_pGraphics->m_nImageId, args[0], args[1], args[2], args[3], args[4] - pivotX, args[5] - pivotY, args[6], args[7]);
                }
                else
                {
                    pRenderContext->drawCanvas(pNode->m_pGraphics->m_nImageId, args[0], args[1], args[2], args[3], args[4] - pivotX, args[5] - pivotY, args[6], args[7]);
                }
                pRenderContext->restore();
            }
            else
            {
                //alpha
                float fSaveAlpha = pCurrentContextData->nAlpha;
                //blend
                int nSaveBlendType = pCurrentContextData->nBlendType;
                if (pStyle->m_nBlendMode != BLEND_TYPE_NONE)
                {
                    pCurrentContextData->nBlendType = pStyle->m_nBlendMode;
                }
                float* args = pNode->m_pGraphics->m_pDrawImageArgs;
                pCurrentContextData->nAlpha *= (pStyle->m_fAlpha * args[8]);
                if (args[9] == 0)
                {
                    pRenderContext->drawImage(pNode->m_pGraphics->m_nImageId, args[0], args[1], args[2], args[3], args[4] + x - pivotX, args[5] + y - pivotY, args[6], args[7]);
                }
                else
                {
                    pRenderContext->drawCanvas(pNode->m_pGraphics->m_nImageId, args[0], args[1], args[2], args[3], args[4] + x - pivotX, args[5] + y - pivotY, args[6], args[7]);
                }
                //restore alpha blend
                pCurrentContextData->nAlpha = fSaveAlpha;
                pCurrentContextData->nBlendType = nSaveBlendType;
            }
        }
        (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
    }
    void JCNode2DRenderer::alpha(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        float fAlpha = pNode->m_kNodeStyle.m_fAlpha;
        ContextDataInfo* pCurrentContextData = pRenderContext->m_pContextData->m_pCurrentContextData;
        if (fAlpha > 0.01)
        {
            float tAlpha = pCurrentContextData->nAlpha;
            pCurrentContextData->nAlpha *= fAlpha;
            (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
             pCurrentContextData->nAlpha = tAlpha;
        }
    }
    void JCNode2DRenderer::transform(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        Matrix32* pMatrix = pNode->getTransform();
        if (pMatrix && m_pNext != NORENDER)
        {
            pRenderContext->save();
            float* m = pMatrix->m;
            pRenderContext->transform(m[0],m[1],m[2],m[3],m[4]+x,m[5]+y);
            (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, 0, 0);
            pRenderContext->restore();
        }
        else
        {
            (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
        }
    }
    void JCNode2DRenderer::childs(JCNode2D* pNode2D, JCHtml5Context* pRenderContext, float x, float y)
    {
        JCNodeStyle* pStyle = &pNode2D->m_kNodeStyle;
        x += -pStyle->m_pTransform->pivotX;
        y += -pStyle->m_pTransform->pivotY;
        Rectangle* pRect = pStyle->m_pClipRect;
        if (!pNode2D->m_bOptimizeScrollRect || pRect == NULL)
        {
            JCNode::s_nNodeCount += pNode2D->m_vChildren.size();
            for (int i = 0, n = pNode2D->m_vChildren.size(); i < n; i++)
            {
                JCNode* pChild = pNode2D->m_vChildren[i];
                if (pChild)
                {
                    if (pChild->m_nType == NODE_TYPE_2D)
                    {
                        JCNode2D* pTemp = (JCNode2D*)pChild;
                        if (pTemp && pTemp->m_kNodeStyle.m_bVisible)
                        {
                            pTemp->render(pRenderContext, x, y);
                        }
                    }
                    else
                    {
                        pChild->render(pRenderContext, x, y);
                    }
                }
            }
        }
        else
        {
            JCNode::s_nNodeCount += pNode2D->m_vChildren.size();
            for (int i = 0, n = pNode2D->m_vChildren.size(); i < n; i++)
            {
                JCNode* pChild = pNode2D->m_vChildren[i];
                if (pChild)
                {
                    if (pChild->m_nType == NODE_TYPE_2D)
                    {
                        JCNode2D* pTemp = (JCNode2D*)pChild;
                        if (pTemp && pTemp->m_kNodeStyle.m_bVisible)
                        {
                            Rectangle kTemp(pTemp->m_nX, pTemp->m_nY, pTemp->m_nWidth, pTemp->m_nHeight);
                            if (pRect->intersects(&kTemp))
                            {
                                pTemp->render(pRenderContext, x, y);
                            }
                        }
                    }
                    else
                    {
                        pChild->render(pRenderContext, x, y);
                    }
                }
            }
        }
    }
    void JCNode2DRenderer::canvas(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y)
    {
        if (pNode->m_pCacheCanvas == NULL)
        {
            LOGE("JCNode2DRenderer::canvas error");
            (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
            return;
        }
        JCHtml5Context* pCacheContext = pNode->m_pCacheCanvas->m_pContext;
        JCTransform* tf = pNode->m_kNodeStyle.m_pTransform;
        switch (pCacheContext->m_nCanvasType)
        {
        case CANVAS_TYPE_RENDER_TARGET:
        {
            JCTexture* pRenderTarget = pCacheContext->getRenderTargetTexture();
            bool bLostRenderTarget = (pRenderTarget && pRenderTarget->m_pGpuTexture <= 0);
            bool bMaskRepaint = false;
            if (pNode->m_pMask && pNode->m_pMask->m_pCacheCanvas )
            {
                JCHtml5Context* pMaskCacheContext = pNode->m_pMask->m_pCacheCanvas->m_pContext;
                JCTexture* pMaskRenderTarget = pMaskCacheContext->getRenderTargetTexture();
                bool bMaskLostRenderTarget = (pMaskRenderTarget && pMaskRenderTarget->m_pGpuTexture <= 0);
                bMaskRepaint = (pNode->m_pMask->m_nRepaint || bMaskLostRenderTarget || pMaskCacheContext->m_bRepaint);
            }
            if (pNode->m_nRepaint || bLostRenderTarget || pCacheContext->m_bRepaint || bMaskRepaint)
            {
                Rectangle kRect;
                pNode->getSelfBound(kRect);
                kRect.width += CANVAS_EXPAN_SIZE;
                kRect.height += CANVAS_EXPAN_SIZE;
                if (kRect.width > 2048 || kRect.height > 2048)
                {
                    LOGE("JCNode2DRenderer::canvas error size > 2048");
                    pCacheContext->setCanvasType(CANVAS_TYPE_MEMORY);
                }
                else
                {
                    pCacheContext->setCanvasSize(kRect.width, kRect.height);
                    pCacheContext->m_pContextData->reset(kRect.width, kRect.height);
                    pCacheContext->clearSaveCmd();
                    float nX = CANVAS_EXPAN_POS - kRect.x + tf->pivotX;
                    float nY = CANVAS_EXPAN_POS - kRect.y + tf->pivotY;
                    (m_pNext->*(m_pNext->m_pFunction))(pNode, pCacheContext, nX, nY);
                    pCacheContext->setAtlasCallback(pCacheContext->m_pDynamicMesh);
                }
            }
            if (pCacheContext->m_nCanvasType == CANVAS_TYPE_RENDER_TARGET)
            {
                float fSelfCanvasW = pCacheContext->m_nWidth - CANVAS_EXPAN_SIZE;
                float fSelfCanvasH = pCacheContext->m_nHeight - CANVAS_EXPAN_SIZE;
                float fSelfX = x - tf->pivotX + pNode->m_kBoundingBox.x;
                float fSelfY = y - tf->pivotY + pNode->m_kBoundingBox.y;
                if (pNode->m_pMask && pNode->m_pMask->m_pCacheCanvas)
                {
                    if (pCacheContext->m_nWidth > 0 && pCacheContext->m_nHeight > 0)
                    {
                        Rectangle rect = pNode->m_pMask->m_kParentBoundingBox;
                        rect.intersectionThis(&pNode->m_kBoundingBox);
                        float fClipCanvasW = rect.width + biasXY;
                        float fClipCanvasH = rect.height + biasXY;
                        float fClipX = x - tf->pivotX + rect.x - 0.5 * biasXY;
                        float fClipY = y - tf->pivotY + rect.y - 0.5 * biasXY;
                        if (fClipCanvasW > 0 && fClipCanvasH > 0)
                        {
                            pRenderContext->save();
                            pRenderContext->clipRect(fClipX, fClipY, fClipCanvasW, fClipCanvasH);
                            pRenderContext->xDrawRenderTarget(pCacheContext, CANVAS_EXPAN_POS - biasXY, CANVAS_EXPAN_POS - biasXY, fSelfCanvasW + biasWH, fSelfCanvasH + biasWH,
                            fSelfX - biasXY, fSelfY - biasXY, fSelfCanvasW + biasWH, fSelfCanvasH + biasWH);
                            pRenderContext->restore();
                        }
                    }
                }
                else if (pNode->m_kNodeStyle.m_pGlowFilter)
                {
                    JCGlowFilter* pGlow = pNode->m_kNodeStyle.m_pGlowFilter;
                    pRenderContext->xDrawGlowRenderTarget(pCacheContext, 0, 0, pCacheContext->m_nWidth, pCacheContext->m_nHeight,
                        fSelfX - CANVAS_EXPAN_POS,
                        fSelfY - CANVAS_EXPAN_POS,
                        pCacheContext->m_nWidth,
                        pCacheContext->m_nHeight,
                        pGlow->color, pGlow->blur, pGlow->offsetX, pGlow->offsetY);

                    pRenderContext->xDrawRenderTarget(pCacheContext, CANVAS_EXPAN_POS, CANVAS_EXPAN_POS, fSelfCanvasW, fSelfCanvasH,
                                    fSelfX,fSelfY,fSelfCanvasW, fSelfCanvasH );

                }
                else if (pNode->m_kNodeStyle.m_fBlurFilterValue > 0)
                {
                    pRenderContext->xDrawBlurRenderTarget(pCacheContext, 0, 0, pCacheContext->m_nWidth, pCacheContext->m_nHeight,
                        fSelfX - CANVAS_EXPAN_POS,
                        fSelfY - CANVAS_EXPAN_POS,
                        pCacheContext->m_nWidth,
                        pCacheContext->m_nHeight,
                        pNode->m_kNodeStyle.m_fBlurFilterValue);
                }
                else
                {
                    pRenderContext->xDrawRenderTarget(pCacheContext, CANVAS_EXPAN_POS, CANVAS_EXPAN_POS, fSelfCanvasW, fSelfCanvasH,
                                            fSelfX,fSelfY,fSelfCanvasW, fSelfCanvasH);
                }
            }
            else
            {
                LOGE("JCNode2DRenderer::canvas error size > 2048");
                (m_pNext->*(m_pNext->m_pFunction))(pNode, pRenderContext, x, y);
            }
            break;
        }
        case CANVAS_TYPE_MEMORY:
        {
            if (pNode->m_nRepaint || pCacheContext->m_bRepaint )
            {
                Rectangle kRect;
                pNode->getSelfBound(kRect);
                pCacheContext->setCanvasSize(kRect.width, kRect.height);
                pCacheContext->m_pContextData->reset(kRect.width, kRect.height);
                pCacheContext->clearSaveCmd();
                float nX = -kRect.x + tf->pivotX;
                float nY = -kRect.y + tf->pivotY;
                (m_pNext->*(m_pNext->m_pFunction))(pNode, pCacheContext, nX, nY);
                pCacheContext->setAtlasCallback(pCacheContext->m_pDynamicMesh);
            }
            {
                pRenderContext->save();
                float nTranslateX = x - tf->pivotX + pNode->m_kBoundingBox.x;
                float nTranslateY = y - tf->pivotY + pNode->m_kBoundingBox.y;
                Rectangle kRect(nTranslateX, nTranslateY, pCacheContext->m_nWidth, pCacheContext->m_nHeight);
                kRect.applyMatrix( &(pRenderContext->m_pContextData->m_pCurrentContextData->kMatrix ));
                pRenderContext->translate(nTranslateX, nTranslateY);
                pRenderContext->xDrawMemoryCanvas(pCacheContext, kRect.x,kRect.y,kRect.width,kRect.height, pNode->m_kNodeStyle.m_fAlpha);
                pRenderContext->restore();
            }
            break;
        }
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
