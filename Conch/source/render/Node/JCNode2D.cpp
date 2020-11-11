/**
@file			JCNode2D.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_13
*/

#include "JCNode2D.h"
#include <resource/JCResManager.h>
#include "../Image/JCImageManager.h"
#include <3DRS/JC2DShaderManager.h>
#include "../Html5Render/JCHtml5RenderManager.h"
#include <resource/text/JCTextManager.h>
#include <resource/text/JCFontManager.h>
#include "../Manager/JCBufferManager.h"
#include "../Manager/JCShaderManager.h"
#include "JCNode2DRenderer.h"
#include <util/Log.h>
#include "util/Log.h"
#include "../Html5Render/JCHtml5Render.h"
#include "JCGraphicsCmdDispath.h"

//------------------------------------------------------------------------------
namespace laya
{
    int JCNode2D::m_nCreateCacheCanvasID = 0xFFFFFF;//从这个ID号开始向后++，这个变量是为了CachCanvas用的
    JCNode2D::JCNode2D(int nID,JCResManager* pTextureManager, JCImageManager* pImageManager, JC2DShaderManager* p2DShaderManager,
        JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager,
        JCBufferManager* pBufferManager, JCShaderManager* pShaderManager, std::vector<JCICmdDispatch*>* vVectorCmdDispath)
        : JCNode( nID,NODE_TYPE_2D )
    {
        m_pTextureManager = pTextureManager;
        m_pImageManager = pImageManager;
        m_p2DShaderManager = p2DShaderManager;
        m_pHtml5RenderManager = pHtml5RenderManager;
        m_pTextManager = pTextManager;
        m_pFontManager = pFontManager;
        m_pBufferManager = pBufferManager;
        m_pShaderManager = pShaderManager;
		m_vVectorCmdDispath = vVectorCmdDispath;
        m_pMask = NULL;
        m_nRenderType = NRT_NO;
        m_nX = 0;
        m_nY = 0;
		m_nZOrder = 0;
        m_nWidth = 0;
        m_nHeight = 0;
        m_pGraphics = NULL;
        m_pCustomData = NULL;
        m_bTransformChange = m_bOptimizeScrollRect=false;
        m_pMatrix = NULL;
        m_pCacheCanvas = NULL;
        m_nCacheCanvasReason = 0;
    }
    JCNode2D::~JCNode2D()
    {
        if (m_pCacheCanvas != NULL)
        {
            m_pHtml5RenderManager->removeHtml5Render(m_pCacheCanvas->m_nID);
            delete m_pCacheCanvas;
            m_pCacheCanvas = NULL;
        }
        if (m_pMatrix)
        {
            delete m_pMatrix;
            m_pMatrix = NULL;
        }
        if (m_pGraphics)
            m_pGraphics->notifyDetachFrom(this);
        m_pGraphics = NULL;
        m_pCustomData = NULL;
    }
    void JCNode2D::render(JCHtml5Context* pContext, float x, float y)
    {
        JCNode2DRenderer* pNodeRenderer = JCNode2DRenderer::m_vVectorRenderers[m_nRenderType];
        pNodeRenderer->runRender(this, pContext, x+m_nX, y+m_nY);
        m_nRepaint = 0;
    }

	void JCNode2D::updateOrder()
	{
		if (m_vChildren.size() < 2)return;
		int i = 1,j, len = m_vChildren.size();
		float key;
		JCNode2D* c;
		while (i<len)
		{
			j = i;
			c = (JCNode2D*)m_vChildren[j];
			key=c->m_nZOrder;
			while (--j>-1)
			{
				if (((JCNode2D*)m_vChildren[j])->m_nZOrder>key)m_vChildren[j + 1] = m_vChildren[j];
				else break;
			}
			m_vChildren[j + 1] = c;
			i++;
		}
		repaint();
	}

    void JCNode2D::size(float w, float h)
    {
        m_nWidth = w;
        m_nHeight = h;
        repaint();
    }
    void JCNode2D::pos(float x, float y)
    {
        m_nX = x;
        m_nY = y;
        repaint();
    }
    void JCNode2D::pivot(float x, float y)
    {
        m_kNodeStyle.setPoivt(x, y);
        repaint();
    }
    void JCNode2D::scale(float x, float y)
    {
        m_kNodeStyle.setScale(x, y);
        m_bTransformChange = true;
        m_nRenderType |= NRT_TRANSFORM;
        parentRepaint();
    }
    void JCNode2D::skew(float x, float y)
    {
        m_kNodeStyle.setSkewAngle(x, y);
        m_bTransformChange = true;
        m_nRenderType |= NRT_TRANSFORM;
        parentRepaint();
    }
    void JCNode2D::rotate(float r)
    {
        m_kNodeStyle.setRotateAngle(r);
        m_bTransformChange = true;
        m_nRenderType |= NRT_TRANSFORM;
        parentRepaint();
    }
    void JCNode2D::bgColor(int nColor)
    {
        m_kNodeStyle.m_nBgColor = nColor;
        m_nRenderType |= NRT_STYLE;
        m_kNodeStyle.m_nType |= JCNodeStyle::BG_BGCOLOR;
        repaint();
    }

    void JCNode2D::border(int nColor)
    {
        m_kNodeStyle.m_nBorderColor = nColor;
        m_nRenderType |= NRT_STYLE;
        m_kNodeStyle.m_nType |= JCNodeStyle::BG_BORDERCOLOR;
        repaint();
    }
    void JCNode2D::font(const char* sFont)
    {   
    }
    void JCNode2D::text()
    {
    }
    void JCNode2D::transform(float m11, float m12, float m21, float m22, float m31, float m32)
    {
        m_bTransformChange = false;
        if (m_pMatrix==NULL) m_pMatrix = new Matrix32();
        m_pMatrix->setTransform(m11,m12,m21,m22,0,0);
        m_nX = m31;
        m_nY = m32;
        if (m_pMatrix->isIdentity())
        {
            m_nRenderType &= ~NRT_TRANSFORM;
        }
        else
        {
            m_nRenderType |= NRT_TRANSFORM;
        }
        parentRepaint();
    }
    void JCNode2D::setTransform(float m11, float m12, float m21, float m22, float m31, float m32)
    {

    }
    void JCNode2D::alpha(float fAlpha)
    {
        fAlpha = fAlpha < 0 ? 0 : (fAlpha>1) ? 1 : fAlpha;
        m_kNodeStyle.m_fAlpha = fAlpha;
        if (fAlpha != 1)
        {
            m_nRenderType |= NRT_ALPHA;
        }
        else
        {
            m_nRenderType &= ~NRT_ALPHA;
        }
        repaint();
    }
    void JCNode2D::filter(float r, float g, float b, float gray)
    {
        if ( r == 1 && g == 1 && b == 1 && gray == 0)
        {
            m_nRenderType &= ~NRT_FILTERS;
            if (m_kNodeStyle.m_pFilter != NULL)
            {
                delete m_kNodeStyle.m_pFilter;
                m_kNodeStyle.m_pFilter = NULL;
            }
        }
        if (m_kNodeStyle.m_pFilter == NULL) m_kNodeStyle.m_pFilter = new JCFilter();
        JCFilter* pFilter = m_kNodeStyle.m_pFilter;
        pFilter->r = r; pFilter->g = g; pFilter->b = b; pFilter->gray = gray;
        m_nRenderType |= NRT_FILTERS;
        repaint();
    }
    void JCNode2D::filterMatrix(float* pColorMatrix,float* pAlpha)
    {
        if (m_kNodeStyle.m_pFilterMatrix == NULL) m_kNodeStyle.m_pFilterMatrix = new JCFilterMatrix();
        JCFilterMatrix* pFilterMatrix = m_kNodeStyle.m_pFilterMatrix;
        memcpy(pFilterMatrix->matrix, pColorMatrix, sizeof(float) * 16);
        memcpy(pFilterMatrix->alpha, pAlpha, sizeof(float) * 4);
        m_nRenderType |= NRT_FILTERS;
        repaint();
    }
    void JCNode2D::visible(bool bVisible)
    {
        m_kNodeStyle.m_bVisible = bVisible;
        parentRepaint();
    }
    void JCNode2D::blendMode(int nBlendMode)
    {
        m_kNodeStyle.m_nBlendMode = nBlendMode;
        m_nRenderType |= NRT_BLEND;
        parentRepaint();
    }
    void JCNode2D::scrollRect(float x, float y, float w, float h)
    {
        if (m_kNodeStyle.m_pClipRect == NULL) m_kNodeStyle.m_pClipRect = new Rectangle();
        m_kNodeStyle.m_pClipRect->setTo(x, y, w, h);
        m_nRenderType |= NRT_CLIP;
        repaint();
    }

    void JCNode2D::optimizeScrollRect(bool b)
    {
        m_bOptimizeScrollRect = b;
        repaint();
    }

    void JCNode2D::mask(JCNode2D* pNodeID)
    {
        m_pMask = pNodeID;
        if (m_pMask)
        {
            m_nCacheCanvasReason |= CCR_MASK;
            _cacheAs(CANVAS_TYPE_RENDER_TARGET);
            m_nRenderType |= NRT_MASK;
        }
        else
        {
            removeCacheCanvasFromType( CCR_MASK );
        }
        parentRepaint();
    }
    void JCNode2D::graphics(JCGraphics* pGraphics, bool bNotifyDetach/* = true*/)
    {
        if (m_pGraphics && bNotifyDetach)
            m_pGraphics->notifyDetachFrom(this);

        m_pGraphics = pGraphics;
        if (m_pGraphics)
        {
            if (m_pGraphics->m_nImageId != -1)
            {
                m_nRenderType |= NRT_IMAGE;
            }
            else
            {
                m_nRenderType &= ~NRT_IMAGE;
            }
            m_nRenderType |= NRT_GRAPHICS;
            m_pGraphics->notifyAttachTo(this);
        }
        else
        {
            m_nRenderType &= ~NRT_IMAGE;
            m_nRenderType &= ~NRT_GRAPHICS;
        }
        repaint();
    }
    void JCNode2D::removeType(int type)
    {
        switch (type)
        {
        case NRT_GRAPHICS:
            if (m_pGraphics)
                m_pGraphics->notifyDetachFrom(this);
            m_pGraphics = NULL;
            m_nRenderType &= ~NRT_GRAPHICS;
            //m_nRenderType &= ~NRT_IMAGE;
            repaint();
            break;
        case NRT_CLIP:
            m_nRenderType &= ~NRT_CLIP;
            if (m_kNodeStyle.m_pClipRect)
            {
                delete m_kNodeStyle.m_pClipRect;
                m_kNodeStyle.m_pClipRect = NULL;
            }
            repaint();
            break;
        case NRT_TRANSFORM:
            m_nRenderType &= ~NRT_TRANSFORM;
            if (m_pMatrix)
            {
                delete m_pMatrix;
                m_pMatrix = NULL;
            }
            m_bTransformChange = true;
            parentRepaint();
            break;
        case NRT_BLEND:
            m_kNodeStyle.m_nBlendMode = BLEND_TYPE_NONE;
            m_nRenderType &= ~NRT_BLEND;
            parentRepaint();
            break;
        case 0x81:
            m_kNodeStyle.m_nBgColor = 0;
            m_kNodeStyle.m_nType &= ~JCNodeStyle::BG_BGCOLOR;
            if (m_kNodeStyle.m_nType==0)
            {
                m_nRenderType &= ~NRT_STYLE;
            }
            repaint();
            break;
        case 0x82:
            m_kNodeStyle.m_nBorderColor = 0;
            m_kNodeStyle.m_nType &= ~JCNodeStyle::BG_BORDERCOLOR;
            if (m_kNodeStyle.m_nType == 0)
            {
                m_nRenderType &= ~NRT_STYLE;
            }
            repaint();
            break;
        case NRT_CUSTOM:
            m_pCustomData = NULL;
            m_nRenderType &= ~NRT_CUSTOM;
            repaint();
            break;
        case NRT_CANVAS:
            removeCacheCanvasFromType(CCR_ORIGINAL);
            repaint();
            break;
        case NRT_FILTERS:
            m_nRenderType &= ~NRT_FILTERS;
            m_kNodeStyle.m_fBlurFilterValue = 0;
            if (m_kNodeStyle.m_pGlowFilter)
            {
                delete m_kNodeStyle.m_pGlowFilter;
                m_kNodeStyle.m_pGlowFilter = NULL;
            }
            if (m_kNodeStyle.m_pFilter)
            {
                delete m_kNodeStyle.m_pFilter;
                m_kNodeStyle.m_pFilter = NULL;
            }
            if (m_kNodeStyle.m_pFilterMatrix)
            {
                delete m_kNodeStyle.m_pFilterMatrix;
                m_kNodeStyle.m_pFilterMatrix = NULL;
            }
            removeCacheCanvasFromType(CCR_FILTER);
            break;
        default:
            break;
        }
    }
    void JCNode2D::removeCacheCanvas()
    {
        m_nRenderType &= ~NRT_CANVAS;
        if (m_pCacheCanvas)
        {
            m_pHtml5RenderManager->removeHtml5Render(m_pCacheCanvas->m_nID);
            delete m_pCacheCanvas;
            m_pCacheCanvas = NULL;
        }   
    }
    void JCNode2D::removeCacheCanvasFromType(CACHE_CANVAS_REASON nType)
    {
        m_nCacheCanvasReason &= ~nType;
        if (m_nCacheCanvasReason <= 0)
        {
            removeCacheCanvas();
        }
    }
    void JCNode2D::custom( JCMemClass* pCustomData )
    {
        m_pCustomData = pCustomData;
        if (m_pCustomData)
        {
            m_nRenderType |= NRT_CUSTOM;
        }
        else
        {
            m_nRenderType &= ~NRT_CUSTOM;
        }
        repaint();
    }
    void JCNode2D::blurFilter(float fValue)
    {
        //if (fValue > 3)fValue = 3;
        m_kNodeStyle.m_fBlurFilterValue = fValue;
        m_nCacheCanvasReason |= CCR_FILTER;
        _cacheAs(CANVAS_TYPE_RENDER_TARGET);
        repaint();
    }
    void JCNode2D::glowFilter(int color, float blur, float offsetX, float offsetY)
    {
        if (m_kNodeStyle.m_pGlowFilter == NULL)
        {
            m_kNodeStyle.m_pGlowFilter = new JCGlowFilter(color, blur, offsetX, offsetY);
        }
        else
        {
            JCGlowFilter* pGlowFilter = m_kNodeStyle.m_pGlowFilter;
            pGlowFilter->color = color;
            pGlowFilter->blur = blur;
            pGlowFilter->offsetX = offsetX;
            pGlowFilter->offsetY = offsetY;
        }
        m_nCacheCanvasReason |= CCR_FILTER;
        _cacheAs(CANVAS_TYPE_RENDER_TARGET);
        repaint();
    }
    Matrix32* JCNode2D::getTransform()
    {
        if (m_bTransformChange)
        {
            applyTransform();
        }
        return m_pMatrix;
    }
    void JCNode2D::applyTransform()
    {
        m_bTransformChange = false;
        JCTransform* tf = m_kNodeStyle.m_pTransform;
        float sx = tf->scaleX;
        float sy = tf->scaleY;
        if (tf->rotate || sx != 1 || sy != 1 || tf->skewX || tf->skewY)
        {
            if (m_pMatrix == NULL)m_pMatrix = new Matrix32();
            if (tf->rotate)
            {
                //Matrix32::fromRST(m_pMatrix, sx, sy, tf->rotate, 0, 0);
                Matrix32::fromSRT(m_pMatrix, sx, sy, tf->rotate, 0, 0);
            }
            else {
                m_pMatrix->identity();
                m_pMatrix->m[0] = sx;
                m_pMatrix->m[3] = sy;
                m_pMatrix->calcTSR();
                
            }
            //else  因为可能既有旋转又有skew
            {
                if (tf->skewX || tf->skewY){
                    Matrix32 m;
                    m.skew(tf->skewX, tf->skewY);
                    Matrix32::mul(m_pMatrix, m_pMatrix, &m);    //要保证先skew才能与layaair一致。
                }
            }
        }
        else
        {
            if (m_pMatrix)
            {
                delete m_pMatrix;
                m_pMatrix = NULL;
            }
            m_nRenderType &= ~NRT_TRANSFORM;
        }
    }
    void JCNode2D::getSelfAllPoint(std::vector<float>& pResultArrays, const Matrix32& kMatrix,bool bFirstNode)
    { 
        Matrix32 kCurrentMatrix;
        //计算矩阵
        if (bFirstNode == false)
        {
            JCTransform* tf = m_kNodeStyle.m_pTransform; 
            kCurrentMatrix.translate(- tf->pivotX, - tf->pivotY);
            Matrix32* pSRMatrix = getTransform();
            if (pSRMatrix)
            {
                kCurrentMatrix.concat(pSRMatrix);
            }
            Matrix32 kTPMatrix;
            kTPMatrix.translate(m_nX, m_nY);
            kCurrentMatrix.concat(&kTPMatrix);
            kCurrentMatrix.concat(const_cast<Matrix32*>(&kMatrix));
       }
        else {
            kCurrentMatrix = kMatrix;
        }
            
        Rectangle* pRect = m_kNodeStyle.m_pClipRect;
        if (pRect)
        {
            //如果有裁剪不用循环遍历了
            JCGraphicsCmdDispath::addPointResult(pResultArrays, pRect->x, pRect->y, pRect->width, pRect->height, &kCurrentMatrix);
        }
        else
        {
           
            //自己的宽高
            JCGraphicsCmdDispath::addPointResult(pResultArrays, 0, 0, m_nWidth, m_nHeight, &kCurrentMatrix);

            //getGraphics
            if (m_pGraphics)
            {
                std::vector<Matrix32> vMatrixs;
                Matrix32 kCurrent = kCurrentMatrix;
                m_pGraphics->getCmdPoints(pResultArrays, vMatrixs, kCurrent);
            }

            //遍历子节点
            for (int i = 0, n = m_vChildren.size(); i < n; i++ )
            {
                JCNode2D* pChild = (JCNode2D*)m_vChildren[i];
                if (pChild && pChild->m_kNodeStyle.m_bVisible )
                {
                    pChild->getSelfAllPoint(pResultArrays, kCurrentMatrix, false);
                }
            }
        }
    }
    void JCNode2D::getSelfBound(Rectangle& kResultRect )
    {
        std::vector<float> vPoints;
        Matrix32 kCurrent;
        getSelfAllPoint(vPoints, kCurrent, true);
        int n = vPoints.size();
        if (n < 1)
        {
            kResultRect.setTo(0, 0, 0, 0);
            m_kBoundingBox = kResultRect;
            return;
        }

        float minX = 99999, minY = 99999;
        float maxX = -minX, maxY = -minX;
        for (int i = 0; i < n; i+=2 )
        {
            float x = vPoints[i];
            float y = vPoints[i + 1];
            minX = minX < x ? minX : x;
            minY = minY < y ? minY : y;
            maxX = maxX > x ? maxX : x;
            maxY = maxY > y ? maxY : y;
        }
        kResultRect.setTo(minX,minY,maxX-minX,maxY-minY);
        m_kBoundingBox = kResultRect;
    }
    void JCNode2D::getParentBound(Rectangle& kResultRect)
    {
        std::vector<float> vPoints;
        Matrix32 kCurrent;
        getSelfAllPoint(vPoints, kCurrent, false);
        int n = vPoints.size();
        if (n < 1)
        {
            kResultRect.setTo(0, 0, 0, 0);
            m_kParentBoundingBox = kResultRect;
            return;
        }

        float minX = 99999, minY = 99999;
        float maxX = -minX, maxY = -minX;
        for (int i = 0; i < n; i += 2)
        {
            float x = vPoints[i];
            float y = vPoints[i + 1];
            minX = minX < x ? minX : x;
            minY = minY < y ? minY : y;
            maxX = maxX > x ? maxX : x;
            maxY = maxY > y ? maxY : y;
        }
        kResultRect.setTo(minX, minY, maxX - minX, maxY - minY);
        m_kParentBoundingBox = kResultRect;
    }
    int JCNode2D::getCreateCacheCanvasID()
    {
        return m_nCreateCacheCanvasID++;
    }
    void JCNode2D::cacheAs(CANVAS_TYPE nCanvasType)
    {
        m_nCacheCanvasReason |= CCR_ORIGINAL;
        _cacheAs(nCanvasType);
    }
    void JCNode2D::_cacheAs(CANVAS_TYPE nCanvasType)
    {
        switch (nCanvasType)
        {
        case laya::CANVAS_TYPE_RENDER_TARGET:
            {
                m_nRenderType |= NRT_CANVAS;
                if (m_pCacheCanvas == NULL )
                {
                    int nID = getCreateCacheCanvasID();
                    m_pCacheCanvas = new JCHtml5Render(nID, m_pTextureManager, m_pImageManager, m_p2DShaderManager, m_pHtml5RenderManager, m_pTextManager, m_pFontManager, m_pBufferManager, m_pShaderManager,m_vVectorCmdDispath);
                    m_pCacheCanvas->setCanvasType(CANVAS_TYPE_RENDER_TARGET);
                    m_pHtml5RenderManager->pushHtml5Render(m_pCacheCanvas);
                }
            }
            break;
        case laya::CANVAS_TYPE_MEMORY:
            {
                m_nRenderType |= NRT_CANVAS;
                if (m_pCacheCanvas == NULL)
                {
                    int nID = getCreateCacheCanvasID();
                    m_pCacheCanvas = new JCHtml5Render(nID, m_pTextureManager, m_pImageManager, m_p2DShaderManager, m_pHtml5RenderManager, m_pTextManager, m_pFontManager, m_pBufferManager, m_pShaderManager,m_vVectorCmdDispath);
                    m_pCacheCanvas->setCanvasType(CANVAS_TYPE_MEMORY);
                    m_pHtml5RenderManager->pushHtml5Render(m_pCacheCanvas);
                }
            }
            break;
        default:
            {
                removeCacheCanvasFromType(CCR_ORIGINAL);
                m_nRenderType &= ~NRT_CANVAS;
                if (m_pCacheCanvas)
                {
                    m_pHtml5RenderManager->removeHtml5Render(m_pCacheCanvas->m_nID);
                    delete m_pCacheCanvas;
                    m_pCacheCanvas = NULL;
                }
            }
            break;
        }
        repaint();
    }
    void JCNode2D::drawToCanvas(JCHtml5Render* pHtml5Render, float offsetX, float offsetY)
    {
        JCNode2DRenderer* pNodeRenderer = JCNode2DRenderer::m_vVectorRenderers[m_nRenderType];
        pNodeRenderer->runRender(this, pHtml5Render->m_pContext, offsetX, offsetY);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
