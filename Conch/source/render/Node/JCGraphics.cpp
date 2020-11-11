/**
@file			JCGraphics.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_13
*/

#include "JCGraphics.h"
#include "JCNode2D.h"
#include "JCGraphicsCmdDispath.h"
#include <resource/JCResManager.h>
#include "../Image/JCImageManager.h"
#include <3DRS/JC2DShaderManager.h>
#include "../Html5Render/JCHtml5RenderManager.h"
#include <resource/text/JCTextManager.h>
#include <resource/text/JCFontManager.h>
#include "../Manager/JCBufferManager.h"
#include "../Manager/JCShaderManager.h"
#include "util/Log.h"
#include "../Html5Render/JCHtml5Context.h"
#include "JCNodeStyle.h"
#include "JCNode2DRenderer.h"
#include <algorithm>

namespace laya
{
    #define GRAPHICES_SAVE_CMD_SIZE 256
    #define IMAGE_WITH_CHILD  NRT_CHILDS|NRT_IMAGE|NRT_GRAPHICS
    //------------------------------------------------------------------------------
    JCGraphics::JCGraphics(int nID,JCResManager* pTextureManager, JCImageManager* pImageManager, JC2DShaderManager* p2DShaderManager,
        JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager,
        JCBufferManager* pBufferManager, JCShaderManager* pShaderManager, TEXT_BASE_LINE_TYPE nBaseLineType)
    {
        m_nWordPos = 0;
        m_pRenderContext = NULL;
        m_nID = nID;
        m_bIsColorReplace = false;
        m_nReplaceColor = 0;
        m_nImageId = -1;
        m_nCmdNum = 0;
        m_pDrawImageArgs = NULL;
        m_pTextManager = pTextManager;
        m_pFontManager = pFontManager;
        m_pSaveCmd = new JCMemClass(GRAPHICES_SAVE_CMD_SIZE, GRAPHICES_SAVE_CMD_SIZE);
        m_pSaveCmd->setAlign(true);
        m_nBaseLineType = nBaseLineType;
    }
    JCGraphics::~JCGraphics()
    {
        destory();
    }
    void JCGraphics::setGraphicsDispath(JCGraphicsCmdDispath* pGraphicsDispath)
    {
        m_pGraphicsDispath = pGraphicsDispath;
    }
    void JCGraphics::renderGraphics(JCHtml5Context* pContext, float x, float y)
    {
        m_nWordPos = 0;
        m_pRenderContext = pContext;
        m_nX = x;
        m_nY = y;
        //运行所有执行
        m_pSaveCmd->setReadPos(0);
        while (true)
        {
            char* pData = m_pSaveCmd->getReadPtr();
            if (pData)
            {
                m_pGraphicsDispath->runRenderCmd(*m_pSaveCmd);
            }
            else
            {
                break;
            }
        }
    }
    void JCGraphics::clear()
    {
        m_pSaveCmd->clearData();
        m_bIsColorReplace = false;
        m_nReplaceColor = 0;
        m_nCmdNum = 0;
        m_nImageId = -1;
        for (VectorWordInfos::iterator iter = m_vWordInfos.begin(); iter != m_vWordInfos.end(); iter++)
        {
            JCGraphicsWordInfo* pWordInfo = *iter;
            if (pWordInfo != NULL)
            {
                delete pWordInfo;
                pWordInfo = NULL;
            }
        }
        m_vWordInfos.clear();
        m_nWordPos = 0;
    }
    void JCGraphics::destory()
    {
        m_pGraphicsDispath = NULL;
        m_pRenderContext = NULL;

        for (std::vector<JCNode2D*>::iterator it = m_vecNode2D.begin(); it != m_vecNode2D.end(); it++)
        {
            (*it)->graphics(NULL, false);
        }
        m_vecNode2D.clear();
        m_nCmdNum = 0;
        if (m_pSaveCmd)
        {
            delete m_pSaveCmd;
            m_pSaveCmd = NULL;
        }
        m_nImageId = -1;
        if (m_pDrawImageArgs)
        {
            delete[] m_pDrawImageArgs;
            m_pDrawImageArgs = NULL;
        }
        for (VectorWordInfos::iterator iter = m_vWordInfos.begin(); iter != m_vWordInfos.end(); iter++)
        {
            JCGraphicsWordInfo* pWordInfo = *iter;
            if (pWordInfo != NULL)
            {
                delete pWordInfo;
                pWordInfo = NULL;
            }
        }
        m_vWordInfos.clear();
    }
    void JCGraphics::alpha(float alpha)
    {
        m_pRenderContext->m_pContextData->m_pCurrentContextData->nAlpha *= alpha;
    }
    void JCGraphics::rotate(float r, float pivotX, float pivotY)
    {
        m_pRenderContext->translate( m_nX + pivotX, m_nY + pivotY);
        m_pRenderContext->rotate( r );
        m_pRenderContext->translate( -m_nX - pivotX, -m_nY - pivotY );
    }
    void JCGraphics::transform(float m11, float m12, float m21, float m22, float m31, float m32)
    {
        m_pRenderContext->transform(m11, m12, m21, m22, m31,m32);
    }
    void JCGraphics::scale(float x, float y, float pivotX, float pivotY)
    {
        m_pRenderContext->translate(m_nX + pivotX, m_nY + pivotY);
        m_pRenderContext->scale(x,y);
        m_pRenderContext->translate(-m_nX - pivotX, -m_nY - pivotY);
    }
    void JCGraphics::translate(float x, float y)
    {
        m_pRenderContext->translate(x, y);
    }
    void JCGraphics::save()
    {
        m_pRenderContext->save();
    }
    void JCGraphics::restore()
    {
        m_pRenderContext->restore();
    }
    void JCGraphics::clipRect(float x, float y, float w, float h)
    {
        m_pRenderContext->beginPath();
        m_pRenderContext->rect(m_nX+x, m_nY+y, w, h);
        m_pRenderContext->clip();
    }
    void JCGraphics::blendMode(int nBlendMode)
    {
        m_pRenderContext->setGlobalCompositeOperation((BLEND_TYPE)nBlendMode);
    }
    //这两个函数不建议用，效率太低了
    void JCGraphics::fillText(const char* sText, float x, float y, const char* sFont, int nColor, int nTextAlign)
    {
        nColor = m_bIsColorReplace ? m_nReplaceColor : nColor;
        m_pRenderContext->fillText(sText, sFont, m_nX + x, m_nY + y, nColor, (TEXT_ALIGN_TYPE)nTextAlign);
    }
    //这两个函数不建议用，效率太低了
    void JCGraphics::fillBorderText(const char* sText, float x, float y, const char* sFont, int nColor, int nBorderColor, float nLineWidth, int nTextAlign)
    {
        nColor = m_bIsColorReplace ? m_nReplaceColor : nColor;
        m_pRenderContext->fillText(sText, sFont, m_nX + x, m_nY + y, nColor, (TEXT_ALIGN_TYPE)nTextAlign);
    }
    void JCGraphics::fillText(float x, float y, int nTextAlign)
    {
        JCGraphicsWordInfo* pWordInfo = m_vWordInfos[m_nWordPos];
        switch (m_nBaseLineType)
        {
        case laya::TBL_TYPE_MIDDLE:
            m_pRenderContext->xDrawWordRes(pWordInfo->getWordRes(m_pRenderContext, m_pFontManager, m_pTextManager), pWordInfo->m_nTextCountWidth, pWordInfo->m_pFont->m_nFontSize, m_nX + x, m_nY + y + pWordInfo->m_pFont->m_nFontSize/2, TBL_TYPE_MIDDLE, (TEXT_ALIGN_TYPE)nTextAlign);
            break;
        default:
            m_pRenderContext->xDrawWordRes(pWordInfo->getWordRes(m_pRenderContext, m_pFontManager, m_pTextManager), pWordInfo->m_nTextCountWidth, pWordInfo->m_pFont->m_nFontSize, m_nX + x, m_nY + y, TBL_TYPE_TOP, (TEXT_ALIGN_TYPE)nTextAlign);
            break;
        }
        m_nWordPos++;
    }
    void JCGraphics::fillBorderText(float x, float y, float nLineWidth, int nTextAlign)
    {
        JCGraphicsWordInfo* pWordInfo = m_vWordInfos[m_nWordPos];
        switch (m_nBaseLineType)
        {
        case laya::TBL_TYPE_MIDDLE:
            m_pRenderContext->xDrawWordRes(pWordInfo->getWordRes(m_pRenderContext, m_pFontManager, m_pTextManager), pWordInfo->m_nTextCountWidth, pWordInfo->m_pFont->m_nFontSize, m_nX + x, m_nY + y + pWordInfo->m_pFont->m_nFontSize/2, TBL_TYPE_MIDDLE, (TEXT_ALIGN_TYPE)nTextAlign);
            break;
        default:
            m_pRenderContext->xDrawWordRes(pWordInfo->getWordRes(m_pRenderContext, m_pFontManager, m_pTextManager), pWordInfo->m_nTextCountWidth, pWordInfo->m_pFont->m_nFontSize, m_nX + x, m_nY + y, TBL_TYPE_TOP, (TEXT_ALIGN_TYPE)nTextAlign);
            break;
        }
        m_nWordPos++;
    }
    void JCGraphics::drawRect(float x, float y, float width, float height, int nFillColor, int nLineColor, float nLineWidth,int nFlag)
    {
        if (nFlag == -3)return;
        if (nFlag != -1)
        {
            m_pRenderContext->setFillStyle(nFillColor);
            m_pRenderContext->fillRect(m_nX + x, m_nY + y, width, height);
        }
        //lineWidth代表是否需要绘制line
        if (nFlag != -2)
        {
            m_pRenderContext->setLineWidth(nLineWidth);
            m_pRenderContext->setStrokeStyle(nLineColor);
            m_pRenderContext->strokeRect(m_nX+x, m_nY+y, width, height);
        }
    }
    void JCGraphics::drawCircle(float x, float y, float radius, int nFillColor, int nLineColor, float nLineWidth,int nFlag)
    {
        if (nFlag == -3)return;
        m_pRenderContext->beginPath();
        m_pRenderContext->arc(m_nX + x, m_nY + y, radius, 0, PI * 2, true);
        m_pRenderContext->closePath();
        if (nFlag != -1)
        {
            m_pRenderContext->setFillStyle(nFillColor);
            m_pRenderContext->fill();
        }
        //lineWidth代表是否需要绘制line
        if (nFlag != -2)
        {
            m_pRenderContext->setLineWidth(nLineWidth);
            m_pRenderContext->setStrokeStyle(nLineColor);
            m_pRenderContext->stroke();
        }
    }
    void JCGraphics::drawPie(float x, float y, float radius, float startAngle, float endAngle, int nFillColor, int nLineColor, float nLineWidth,int nFlag)
    {
        if (nFlag == -3)return;
        m_pRenderContext->beginPath();
        m_pRenderContext->moveTo(m_nX+x, m_nY+y);
        m_pRenderContext->arc(m_nX + x, m_nY + y, radius, startAngle*ARC_TO_RAD, endAngle*ARC_TO_RAD,false );
        m_pRenderContext->closePath();
        if (nFlag != -1)
        {
            m_pRenderContext->setFillStyle(nFillColor);
            m_pRenderContext->fill();
        }
        //lineWidth代表是否需要绘制line
        if (nFlag != -2)
        {
            m_pRenderContext->setLineWidth(nLineWidth);
            m_pRenderContext->setStrokeStyle(nLineColor);
            m_pRenderContext->stroke();
        }
    }
    void JCGraphics::drawImage(int nImageID, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh, float alpha,int nType)
    {
        ContextDataInfo* pCurrentContextData = m_pRenderContext->m_pContextData->m_pCurrentContextData;
        float fSaveAlpha = pCurrentContextData->nAlpha;
        pCurrentContextData->nAlpha *= alpha;
        if (nType==0)
        {
            m_pRenderContext->drawImage(nImageID, sx, sy, sw, sh, dx + m_nX, dy + m_nY, dw, dh);
        }
        else
        {
            m_pRenderContext->drawCanvas(nImageID, sx, sy, sw, sh, dx + m_nX, dy + m_nY, dw, dh);
        }
        pCurrentContextData->nAlpha = fSaveAlpha;
    }

	void JCGraphics::drawImageS(int nImageID, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh, int nType, int len, char *posBuffer)
	{
		int temp = len/4;
		dx += m_nX;
		dy += m_nY;
		float *  posArray = (float*)posBuffer;
        if (nType == 0)
        {
            for (int i = 0; i < temp; i += 2)
            {
                m_pRenderContext->drawImage(nImageID, sx, sy, sw, sh, dx + posArray[i], dy + posArray[i + 1], dw, dh);
            }
        }
        else
        {
            //TODO 2017-5-16
            for (int i = 0; i < temp; i += 2)
            {
                m_pRenderContext->drawCanvas(nImageID, sx, sy, sw, sh, dx + posArray[i], dy + posArray[i + 1], dw, dh);
            }
        }
	}

    void JCGraphics::drawImageM(int nImageID, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh,
        float m11, float m12, float m21, float m22, float m31, float m32, float alpha, int nType)
    {
        m_pRenderContext->save();
        ContextDataInfo* pCurrentContextData = m_pRenderContext->m_pContextData->m_pCurrentContextData;
        pCurrentContextData->nAlpha *= alpha;
        m_pRenderContext->transform(m11, m12, m21, m22, m31 + m_nX, m32 + m_nY);
        if (nType == 0)
        {
            m_pRenderContext->drawImage(nImageID, sx, sy, sw, sh, dx, dy, dw, dh);
        }
        else
        {
            m_pRenderContext->drawCanvas(nImageID, sx, sy, sw, sh, dx, dy, dw, dh);
        }
        m_pRenderContext->restore();
    }
    void JCGraphics::drawPoly(float x, float y, int nFillColor, int nLineColor, float nLineWidth, char* pPointsBuffer, int nPointNumber,int flag)
    {
        if (flag == -3)return;
        m_pRenderContext->beginPath();
        x += m_nX;
        y += m_nY;
        float *points = (float *)pPointsBuffer;
        m_pRenderContext->moveTo(x+points[0], y+points[1]);
        int i = 2;
        while (i<nPointNumber)
        {
            m_pRenderContext->lineTo(x + points[i], y + points[i+1]);
            i += 2;
        }
        m_pRenderContext->closePath();
        if (flag != -1)
        {
            m_pRenderContext->setFillStyle(nFillColor);
            m_pRenderContext->fill();
        }
        if (flag != -2)
        {
            m_pRenderContext->setStrokeStyle(nLineColor);
            m_pRenderContext->setLineWidth(nLineWidth);
            m_pRenderContext->stroke();
        }
    }
    void JCGraphics::drawPath(float x, float y, int nBrushColor, char* pPointsBuffer, int nBufferLength,bool bFlag)
    {
        float x1, y1, x2, y2, r;
        if (nBufferLength <= 0)return;
        x += m_nX;
        y += m_nY;
        m_pRenderContext->beginPath();
        while (nBufferLength>0)
        {
            int type=*(int*)pPointsBuffer;
            pPointsBuffer += 4;
            switch (type)
            {
            case TYPE_MOVETO:
                x1 = *(float *)pPointsBuffer; pPointsBuffer += 4;
                y1 = *(float *)pPointsBuffer; pPointsBuffer += 4;
                m_pRenderContext->moveTo(x+x1, y+y1);
                nBufferLength -= 12;
                break;
            case TYPE_LINETO:
                x1 = *(float *)pPointsBuffer; pPointsBuffer += 4;
                y1 = *(float *)pPointsBuffer; pPointsBuffer += 4;
                m_pRenderContext->lineTo(x+x1, y+y1);
                nBufferLength -= 12;
                break;
            case TYPE_ARCTO:
                x1 = *(float *)pPointsBuffer; pPointsBuffer += 4;
                y1 = *(float *)pPointsBuffer; pPointsBuffer += 4;
                x2= *(float *)pPointsBuffer; pPointsBuffer += 4;
                y2 = *(float *)pPointsBuffer; pPointsBuffer += 4;
                r = *(float *)pPointsBuffer; pPointsBuffer += 4;
                m_pRenderContext->arcTo(x1+x,y1+y,x2+x,y2+y,r);
                nBufferLength -= 24;
                break;
            case TYPE_CLOSEPATH:
                m_pRenderContext->closePath();
                nBufferLength -= 4;
                break;
            default:
                break;
            }
        }
        if (bFlag)
        {
            m_pRenderContext->setFillStyle(nBrushColor);
            m_pRenderContext->fill();
        }
    }
    void JCGraphics::drawPath(float x, float y, int nBrushColor, int nStrokeStyle, int nLineJoin,
        int nLineCap, int nMiterLimit, float nLineWidth, char* pPointsBuffer, int nBufferLength,bool bFlag)
    {
        drawPath(x, y, nBrushColor, pPointsBuffer, nBufferLength, bFlag);
        m_pRenderContext->setStrokeStyle(nStrokeStyle);
        m_pRenderContext->setLineWidth(nLineWidth);
        m_pRenderContext->setLineJoin((LINEJOIN_TYPE)nLineJoin);
        m_pRenderContext->setLineCap((LINECAP_TYPE)nLineCap);
        m_pRenderContext->setMiterLimit(nMiterLimit);
        m_pRenderContext->stroke();
    }
    void JCGraphics::getCmdPoints(std::vector<float>& pResultArrays,std::vector<Matrix32>& vMatrixs,Matrix32& kCurrentMatrix )
    {
        m_pSaveCmd->setReadPos(0);
        while (true)
        {
            char* pData = m_pSaveCmd->getReadPtr();
            if (pData)
            {
                m_pGraphicsDispath->calcBoundingBox(*m_pSaveCmd, vMatrixs, kCurrentMatrix, pResultArrays);
            }
            else
            {
                break;
            }
        }
    }
    void JCGraphics::drawLine(float x1, float y1, float x2, float y2, int lineColor, float lineWidth)
    {
        m_pRenderContext->beginPath();
        m_pRenderContext->setLineWidth(lineWidth);
        m_pRenderContext->setStrokeStyle(lineColor);
        m_pRenderContext->moveTo(x1+m_nX, y1+m_nY);
        m_pRenderContext->lineTo(x2+m_nX, y2+m_nY);
        m_pRenderContext->stroke();
    }
    void JCGraphics::drawLine(float x,float y,int lineColor, float lineWidth, char *pPointsBuffer, int nPointNumber)
    {
        x += m_nX;
        y += m_nY;
        m_pRenderContext->beginPath();
        m_pRenderContext->setLineWidth(lineWidth);
        m_pRenderContext->setStrokeStyle(lineColor);
        int i=2;
        if (nPointNumber % 2 != 0)
        {
            LOGE("JCGraphics::drawLine the point number must be even");
        }
        float * points = (float *)pPointsBuffer;
        m_pRenderContext->moveTo(x+points[0],y+points[1]);
        while (i<nPointNumber)
        {
            m_pRenderContext->lineTo(x + points[i], y + points[i + 1]);
            i += 2;
        }
        m_pRenderContext->stroke();
    }
    void JCGraphics::drawCurves(float x, float y, int lineColor, float lineWidth, char *pPointsBuffer, int nPointNumber)
    {
        x += m_nX;
        y += m_nY;
        m_pRenderContext->beginPath();
        m_pRenderContext->setLineWidth(lineWidth);
        m_pRenderContext->setStrokeStyle(lineColor);
        int i = 2;
        if ((nPointNumber-2) % 4 != 0)
        {
            LOGE("JCGraphics::drawCurves the point number must be multiple of 4 plus 2");
        }
        float * points = (float *)pPointsBuffer;
        m_pRenderContext->moveTo(x + points[0], y + points[1]);
        while (i < nPointNumber)
        {
            m_pRenderContext->quadraticCurveTo (x + points[i], y + points[i + 1],x+points[i+2],y+points[i+3]);
            i += 4;
        }
        m_pRenderContext->stroke();
    }

    void JCGraphics::replaceTextColor(int nColor)
    {
        m_bIsColorReplace = true;
        m_nReplaceColor = nColor;
        for (VectorWordInfos::iterator iter = m_vWordInfos.begin(); iter != m_vWordInfos.end(); iter++)
        {
            JCGraphicsWordInfo* pWordInfo = *iter;
            if (pWordInfo)
            {
                pWordInfo->modifyTextInfo(m_pFontManager,m_pTextManager,nColor);
            }
        }
    }

    void JCGraphics::replaceText(const char* sText)
    {
        for (VectorWordInfos::iterator iter = m_vWordInfos.begin(); iter != m_vWordInfos.end(); iter++)
        {
            JCGraphicsWordInfo* pWordInfo = *iter;
            if (pWordInfo)
            {
                pWordInfo->modifyTextInfo(m_pFontManager, m_pTextManager, sText);
            }
        }
    }

    void JCGraphics::fillImage(int imageId, int repeatType, float* args)
    {
        m_pRenderContext->translate(args[4]+m_nX, args[5]+m_nY);
        m_pRenderContext->setFillStyle(imageId,(FILL_PATTERN_TYPE)repeatType, args[0], args[1], args[2], args[3]);
        m_pRenderContext->fillRect(args[6], args[7], args[8], args[9]);
        m_pRenderContext->translate(-args[4] - m_nX, -args[5] - m_nY);
    }
    void JCGraphics::setSkinMesh(int p_nIbId, int p_nVbId, int p_nVertType, int p_nImgId, int p_nShaderId, int p_nStartIndex, int p_nNumElement, float *mat)
    {
        m_pRenderContext->setSkinMesh(p_nIbId, p_nVbId, p_nVertType, p_nImgId, p_nShaderId, p_nStartIndex, p_nNumElement, mat, m_nX, m_nY);
    }

    void JCGraphics::drawParticle(int p_nParticleTemplatid)
    {
        if (p_nParticleTemplatid == -1)return;
        m_pRenderContext->drawParticle(p_nParticleTemplatid,m_nX,m_nY);
    }

    void JCGraphics::setDrawImageFlag(int nImageID, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh,float alpha,int nType)
    {
        if (m_nCmdNum > 1)return;

        std::vector<JCNode2D*>::iterator it = m_vecNode2D.begin();
        for (; it != m_vecNode2D.end(); it++) {
            JCNode2D* pNode = *it;
            if (pNode != NULL && pNode->m_nRenderType != IMAGE_WITH_CHILD)
            {
                pNode->m_nRenderType |= NRT_IMAGE;
            }
        }
        m_nImageId = nImageID;
        if (m_pDrawImageArgs == NULL)
            m_pDrawImageArgs = new float[10];
        m_pDrawImageArgs[0] = sx;
        m_pDrawImageArgs[1] = sy;
        m_pDrawImageArgs[2] = sw;
        m_pDrawImageArgs[3] = sh;
        m_pDrawImageArgs[4] = dx;
        m_pDrawImageArgs[5] = dy;
        m_pDrawImageArgs[6] = dw;
        m_pDrawImageArgs[7] = dh;
        m_pDrawImageArgs[8] = alpha;
        m_pDrawImageArgs[9] = nType;
    }

    void JCGraphics::removeDrawImageFlag()
    {
        m_nCmdNum++;
        if (m_nImageId == -1)
            return;

        std::vector<JCNode2D*>::iterator it = m_vecNode2D.begin();
        for (; it != m_vecNode2D.end(); it++) {
            JCNode2D* pNode = *it;
            if (pNode != NULL)
            {
                pNode->m_nRenderType &= ~NRT_IMAGE;
            }
        }
        m_nImageId = -1;
    }
    void JCGraphics::setTextInfo(char* sText, const char* sFont, int nColor)
    {
        m_vWordInfos.push_back( new JCGraphicsWordInfo(m_pFontManager, m_pTextManager, sText, sFont, nColor) );
    }
    void JCGraphics::notifyDetachFrom(JCNode2D* pNode)
    {
        std::vector<JCNode2D*>::iterator it = std::find(m_vecNode2D.begin(), m_vecNode2D.end(), pNode);
        if (it != m_vecNode2D.end()) {
            m_vecNode2D.erase(it);
        }
    }
    void JCGraphics::notifyAttachTo(JCNode2D* pNode)
    {
        std::vector<JCNode2D*>::iterator it = std::find(m_vecNode2D.begin(), m_vecNode2D.end(), pNode);
        if (it == m_vecNode2D.end()) {
            m_vecNode2D.push_back(pNode);
        }
    }
    void JCGraphics::repaint()
    {
        std::vector<JCNode2D*>::iterator it = m_vecNode2D.begin();
        for (; it != m_vecNode2D.end(); it++) {
            JCNode2D* pNode = *it;
            if (pNode)
                pNode->repaint();
        }
    }
}

//-----------------------------END FILE--------------------------------