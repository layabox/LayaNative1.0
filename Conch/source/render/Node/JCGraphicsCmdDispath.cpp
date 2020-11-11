/**
@file			JCGraphicsCmdDispath.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_14
*/

#include "JCGraphicsCmdDispath.h"
#include <util/Log.h>
#include <resource/JCResManager.h>
#include "../Image/JCImageManager.h"
#include <3DRS/JC2DShaderManager.h>
#include "../Html5Render/JCHtml5RenderManager.h"
#include <resource/text/JCTextManager.h>
#include <resource/text/JCFontManager.h>
#include "../Manager/JCBufferManager.h"
#include "../Manager/JCShaderManager.h"
#include "util/JCCmdStruct.h"
#include "../Node/JCNodeStyle.h"
#include "JCNode2D.h"
#include <cmath>

//------------------------------------------------------------------------------
namespace laya
{
    JCGraphicsCmdDispath::JCGraphicsCmdDispath(int nID,JCResManager* pTextureManager, JCImageManager* pImageManager, JC2DShaderManager* p2DShaderManager,
        JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager,
        JCBufferManager* pBufferManager, JCShaderManager* pShaderManager,TEXT_BASE_LINE_TYPE nBaseLineType)
    {
        m_pGraphics = new JCGraphics(nID,pTextureManager, pImageManager, p2DShaderManager, pHtml5RenderManager, 
                                    pTextManager,pFontManager, pBufferManager, pShaderManager, nBaseLineType);
        m_pGraphics->setGraphicsDispath(this);
    }
    JCGraphicsCmdDispath::~JCGraphicsCmdDispath()
    {
        if (m_pGraphics)
        {
            delete m_pGraphics;
            m_pGraphics = NULL;
        }
    }
    bool JCGraphicsCmdDispath::restartJSThread()
    {
        return true;
    }
    bool JCGraphicsCmdDispath::dispatchRenderCmd(JCMemClass& pRenderCmd, int nFrameCount)
    {
        return saveRenderCmd(pRenderCmd);
    }
    bool JCGraphicsCmdDispath::dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount)
    {
        return dispatchRenderCmd(pRenderCmd, nFrameCount);
    }
    bool JCGraphicsCmdDispath::runRenderCmd(JCMemClass& pRenderCmd)
    {
        static const ProcFunction g_svProcFunctions[] =
        {
            &JCGraphicsCmdDispath::_rendercmd_clear,
            &JCGraphicsCmdDispath::_rendercmd_destory,
            &JCGraphicsCmdDispath::_rendercmd_alpha,
            &JCGraphicsCmdDispath::_rendercmd_rotate,
            &JCGraphicsCmdDispath::_rendercmd_transform,
            &JCGraphicsCmdDispath::_rendercmd_scale,
            &JCGraphicsCmdDispath::_rendercmd_translate,
            &JCGraphicsCmdDispath::_rendercmd_save,
            &JCGraphicsCmdDispath::_rendercmd_restore,
            &JCGraphicsCmdDispath::_rendercmd_clipRect,
            &JCGraphicsCmdDispath::_rendercmd_blendMode,
            &JCGraphicsCmdDispath::_rendercmd_fillText,
            &JCGraphicsCmdDispath::_rendercmd_fillBorderText,
            &JCGraphicsCmdDispath::_rendercmd_drawRect,
            &JCGraphicsCmdDispath::_rendercmd_drawCircle,
            &JCGraphicsCmdDispath::_rendercmd_drawPie,
            &JCGraphicsCmdDispath::_rendercmd_drawImage,
            &JCGraphicsCmdDispath::_rendercmd_drawImageM,
            &JCGraphicsCmdDispath::_rendercmd_drawPoly,
            &JCGraphicsCmdDispath::_rendercmd_drawPath,
            &JCGraphicsCmdDispath::_rendercmd_drawLine,
            &JCGraphicsCmdDispath::_rendercmd_drawLines,
            &JCGraphicsCmdDispath::_renderCmd_drawCurves,
            &JCGraphicsCmdDispath::_renderCmd_replaceTextColor,
            &JCGraphicsCmdDispath::_renderCmd_fillImage,
            &JCGraphicsCmdDispath::_renderCmd_setSkinMesh,
            &JCGraphicsCmdDispath::_rendercmd_drawParticle,
			&JCGraphicsCmdDispath::_rendercmd_drawImageS,
            &JCGraphicsCmdDispath::_rendercmd_replaceText,
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
                LOGE("JCGraphicsCmdDispath::runRenderCmd Wrong command number!,cmd=%d", nFunctionID);
            }
        }
        return true;
    }
    bool JCGraphicsCmdDispath::saveRenderCmd(JCMemClass& pRenderCmd)
    {
        static const ProcFunction g_svSaveProcFunctions[] =
        {
            &JCGraphicsCmdDispath::_savecmd_clear,
            &JCGraphicsCmdDispath::_savecmd_destory,
            &JCGraphicsCmdDispath::_savecmd_alpha,
            &JCGraphicsCmdDispath::_savecmd_rotate,
            &JCGraphicsCmdDispath::_savecmd_transform,
            &JCGraphicsCmdDispath::_savecmd_scale,
            &JCGraphicsCmdDispath::_savecmd_translate,
            &JCGraphicsCmdDispath::_savecmd_save,
            &JCGraphicsCmdDispath::_savecmd_restore,
            &JCGraphicsCmdDispath::_savecmd_clipRect,
            &JCGraphicsCmdDispath::_savecmd_blendMode,
            &JCGraphicsCmdDispath::_savecmd_fillText,
            &JCGraphicsCmdDispath::_savecmd_fillBorderText,
            &JCGraphicsCmdDispath::_savecmd_drawRect,
            &JCGraphicsCmdDispath::_savecmd_drawCircle,
            &JCGraphicsCmdDispath::_savecmd_drawPie,
            &JCGraphicsCmdDispath::_savecmd_drawImage,
            &JCGraphicsCmdDispath::_savecmd_drawImageM,
            &JCGraphicsCmdDispath::_savecmd_drawPoly,
            &JCGraphicsCmdDispath::_savecmd_drawPath,
            &JCGraphicsCmdDispath::_savecmd_drawLine,
            &JCGraphicsCmdDispath::_savecmd_drawLines,
            &JCGraphicsCmdDispath::_savecmd_drawCurves,
            &JCGraphicsCmdDispath::_savecmd_replaceTextColor,
            &JCGraphicsCmdDispath::_savecmd_fillImage,
            &JCGraphicsCmdDispath::_savecmd_setSkinMesh,
            &JCGraphicsCmdDispath::_savecmd_drawParticle,
			&JCGraphicsCmdDispath::_savecmd_drawImageS,
            &JCGraphicsCmdDispath::_savecmd_replaceText,
        };
        static const int nFuncs = sizeof(g_svSaveProcFunctions) / sizeof(g_svSaveProcFunctions[0]);
        char* pData = pRenderCmd.getReadPtr();
        if (pData)
        {
            int nFunctionID = *(int*)pData;
            if (nFunctionID >= 0 && nFunctionID < nFuncs)
            {
                m_pGraphics->removeDrawImageFlag();
                (this->*g_svSaveProcFunctions[nFunctionID])(pRenderCmd);
            }
            else
            {
                LOGE("JCGraphicsCmdDispath::saveRenderCmd Wrong command number!,cmd=%d", nFunctionID);
            }
        }
        return true;
    }
    bool JCGraphicsCmdDispath::calcBoundingBox(JCMemClass& pRenderCmd, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        static const CalcBoundingBoxFunction g_svCalcBoundingBoxProcFunctions[] =
        {
            &JCGraphicsCmdDispath::_calcBoundingBox_clear,
            &JCGraphicsCmdDispath::_calcBoundingBox_destory,
            &JCGraphicsCmdDispath::_calcBoundingBox_alpha,
            &JCGraphicsCmdDispath::_calcBoundingBox_rotate,
            &JCGraphicsCmdDispath::_calcBoundingBox_transform,
            &JCGraphicsCmdDispath::_calcBoundingBox_scale,
            &JCGraphicsCmdDispath::_calcBoundingBox_translate,
            &JCGraphicsCmdDispath::_calcBoundingBox_save,
            &JCGraphicsCmdDispath::_calcBoundingBox_restore,
            &JCGraphicsCmdDispath::_calcBoundingBox_clipRect,
            &JCGraphicsCmdDispath::_calcBoundingBox_blendMode,
            &JCGraphicsCmdDispath::_calcBoundingBox_fillText,
            &JCGraphicsCmdDispath::_calcBoundingBox_fillBorderText,
            &JCGraphicsCmdDispath::_calcBoundingBox_drawRect,
            &JCGraphicsCmdDispath::_calcBoundingBox_drawCircle,
            &JCGraphicsCmdDispath::_calcBoundingBox_drawPie,
            &JCGraphicsCmdDispath::_calcBoundingBox_drawImage,
            &JCGraphicsCmdDispath::_calcBoundingBox_drawImageM,
            &JCGraphicsCmdDispath::_calcBoundingBox_drawPoly,
            &JCGraphicsCmdDispath::_calcBoundingBox_drawPath,
            &JCGraphicsCmdDispath::_calcBoundingBox_drawLine,
            &JCGraphicsCmdDispath::_calcBoundingBox_drawLines,
            &JCGraphicsCmdDispath::_calcBoundingBox_drawCurves,
            &JCGraphicsCmdDispath::_calcBoundingBox_replaceTextColor,
            &JCGraphicsCmdDispath::_calcBoundingBox_fillImage,
            &JCGraphicsCmdDispath::_calcBoundingBox_setSkinMesh,
            &JCGraphicsCmdDispath::_calcBoundingBox_drawParticle,
			&JCGraphicsCmdDispath::_calcBoundingBox_drawImageS,
            &JCGraphicsCmdDispath::_calcBoundingBox_replaceText,
        };
        static const int nFuncs = sizeof(g_svCalcBoundingBoxProcFunctions) / sizeof(g_svCalcBoundingBoxProcFunctions[0]);
        char* pData = pRenderCmd.getReadPtr();
        if (pData)
        {
            int nFunctionID = *(int*)pData;
            if (nFunctionID >= 0 && nFunctionID < nFuncs)
            {
                (this->*g_svCalcBoundingBoxProcFunctions[nFunctionID])(pRenderCmd,vMatrixs,kCurrentMatrix,vResultPointArray);
            }
            else
            {
                LOGE("JCGraphicsCmdDispath::calcBoundingBox Wrong command number!,cmd=%d", nFunctionID);
            }
        }
        return true;
    }
    void JCGraphicsCmdDispath::_rendercmd_clear(JCMemClass& cmdBuffer)
    {
       cmdBuffer.popp<CMDNoParam>();
       m_pGraphics->clear();
    }
    void JCGraphicsCmdDispath::_rendercmd_destory(JCMemClass& cmdBuffer)
    {
        cmdBuffer.popp<CMDNoParam>();
        m_pGraphics->destory();
    }
    void JCGraphicsCmdDispath::_rendercmd_alpha(JCMemClass& cmdBuffer)
    {
        CMDFloat* cmdFloat= cmdBuffer.popp<CMDFloat>();
        m_pGraphics->alpha(cmdFloat->f);
    }
    void JCGraphicsCmdDispath::_rendercmd_rotate(JCMemClass& cmdBuffer)
    {
        CMDFloat3* cmdFloat3 = cmdBuffer.popp<CMDFloat3>();
        m_pGraphics->rotate(cmdFloat3->x, cmdFloat3->y, cmdFloat3->z);
    }
    void JCGraphicsCmdDispath::_rendercmd_transform(JCMemClass& cmdBuffer)
    {
        CMDMatrix* cmdMatrix = cmdBuffer.popp<CMDMatrix>();
        m_pGraphics->transform(cmdMatrix->a, cmdMatrix->b, cmdMatrix->c, cmdMatrix->d, cmdMatrix->tx, cmdMatrix->ty);
    }
    void JCGraphicsCmdDispath::_rendercmd_scale(JCMemClass& cmdBuffer)
    {
        CMDFloatRect *cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
        m_pGraphics->scale(cmdFloatRect->x, cmdFloatRect->y, cmdFloatRect->w, cmdFloatRect->h);
    }
    void JCGraphicsCmdDispath::_rendercmd_translate(JCMemClass& cmdBuffer)
    {
        CMDFloatPoint *cmdFloatPoint = cmdBuffer.popp<CMDFloatPoint>();
        m_pGraphics->translate(cmdFloatPoint->x, cmdFloatPoint->y);
    }
    void JCGraphicsCmdDispath::_rendercmd_save(JCMemClass& cmdBuffer)
    {
        cmdBuffer.popp<CMDNoParam>();
        m_pGraphics->save();
    }
    void JCGraphicsCmdDispath::_rendercmd_restore(JCMemClass& cmdBuffer)
    {
        cmdBuffer.popp<CMDNoParam>();
        m_pGraphics->restore();
    }
    void JCGraphicsCmdDispath::_rendercmd_clipRect(JCMemClass& cmdBuffer)
    {
        CMDFloatRect *cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
        m_pGraphics->clipRect(cmdFloatRect->x, cmdFloatRect->y, cmdFloatRect->w, cmdFloatRect->h);
    }
    void JCGraphicsCmdDispath::_rendercmd_blendMode(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pGraphics->blendMode(cmdInt->i);
    }
    void JCGraphicsCmdDispath::_rendercmd_fillText(JCMemClass& cmdBuffer)
    {
        CMDFillText1 * cmdFillText1 = cmdBuffer.popp<CMDFillText1>();
        /*
        int nfontLen = *(int*)cmdBuffer.readBuffer(4);
        char *sfont = (char *)cmdBuffer.readBuffer(nfontLen);
        int nTextLen = *(int *)cmdBuffer.readBuffer(4);
        char *sText = (char*)cmdBuffer.readBuffer(nTextLen);
        */
        m_pGraphics->fillText(cmdFillText1->x, cmdFillText1->y, cmdFillText1->len);
    }
    void JCGraphicsCmdDispath::_rendercmd_fillBorderText(JCMemClass& cmdBuffer)
    {
        CMDFillBorderText* cmdFillBorderText = cmdBuffer.popp<CMDFillBorderText>();
        /*
        char* sFont = cmdBuffer.readBuffer(cmdFillBorderText->nFontLen);
        int nTextlen = *(int*)cmdBuffer.readBuffer(4);
        char* sText = cmdBuffer.readBuffer(nTextlen);
        */
        m_pGraphics->fillBorderText(cmdFillBorderText->x, cmdFillBorderText->y, cmdFillBorderText->nLineWidth, cmdFillBorderText->nTextAlign);
    }
    void JCGraphicsCmdDispath::_rendercmd_drawRect(JCMemClass& cmdBuffer)
    {
        CMDFloatRect *cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
        CMDFII *cmdFII= cmdBuffer.popp<CMDFII>();
        m_pGraphics->drawRect(cmdFloatRect->x, cmdFloatRect->y, cmdFloatRect->w, cmdFloatRect->h, cmdFII->i1, cmdFII->i2, cmdFII->f,cmdFII->flag);
    }
    void JCGraphicsCmdDispath::_rendercmd_drawCircle(JCMemClass& cmdBuffer)
    {
        CMDFloatRect *cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
        CMDOIntPoint *cmdPoint = cmdBuffer.popp<CMDOIntPoint>();
        int nFlag =*(int *)cmdBuffer.readBuffer(4);
        m_pGraphics->drawCircle(cmdFloatRect->x, cmdFloatRect->y, cmdFloatRect->w, cmdPoint->x, cmdPoint->y, cmdFloatRect->h, nFlag);
    }
    void JCGraphicsCmdDispath::_rendercmd_drawPie(JCMemClass& cmdBuffer)
    {
        CMDMatrix *cmdMatrix = cmdBuffer.popp<CMDMatrix>();
        CMDOIntPoint *cmdPoint = cmdBuffer.popp<CMDOIntPoint>();
        int nFlag = *(int *)cmdBuffer.readBuffer(4);
        m_pGraphics->drawPie(cmdMatrix->a, cmdMatrix->b, cmdMatrix->c, cmdMatrix->d, cmdMatrix->tx, cmdPoint->x, cmdPoint->y, cmdMatrix->ty, nFlag);
    }
    void JCGraphicsCmdDispath::_rendercmd_drawImage(JCMemClass& cmdBuffer)
    {
        CMDDrawImageCanvas9Alpha *cmdDrawImage9Alpha = cmdBuffer.popp<CMDDrawImageCanvas9Alpha>();
        m_pGraphics->drawImage(cmdDrawImage9Alpha->imgID, cmdDrawImage9Alpha->sx, cmdDrawImage9Alpha->sy, cmdDrawImage9Alpha->sw, cmdDrawImage9Alpha->sh, cmdDrawImage9Alpha->dx, cmdDrawImage9Alpha->dy, cmdDrawImage9Alpha->dw, cmdDrawImage9Alpha->dh, cmdDrawImage9Alpha->alpha,cmdDrawImage9Alpha->nType);
    }
    void JCGraphicsCmdDispath::_rendercmd_drawImageM(JCMemClass& cmdBuffer)
    {
        CMDDrawImageCanvasM *cmdDrawImageM = cmdBuffer.popp<CMDDrawImageCanvasM>();
        m_pGraphics->drawImageM(cmdDrawImageM->imgID, cmdDrawImageM->sx, cmdDrawImageM->sy, cmdDrawImageM->sw, cmdDrawImageM->sh, cmdDrawImageM->dx, cmdDrawImageM->dy, cmdDrawImageM->dw, cmdDrawImageM->dh, cmdDrawImageM->a,
            cmdDrawImageM->b, cmdDrawImageM->c, cmdDrawImageM->d, cmdDrawImageM->tx, cmdDrawImageM->ty,cmdDrawImageM->alpha, cmdDrawImageM->nType);
    }
    void JCGraphicsCmdDispath::_rendercmd_drawPoly(JCMemClass& cmdBuffer)
    {
        CMDDrawImage3 *cmdDrawImage3 = cmdBuffer.popp<CMDDrawImage3>();
        CMDFII* cmdFII= cmdBuffer.popp<CMDFII>();
        char* sPoints= (char*)cmdBuffer.readBuffer(cmdDrawImage3->imgID*4);
        m_pGraphics->drawPoly(cmdDrawImage3->x, cmdDrawImage3->y, cmdFII->i1, cmdFII->i2, cmdFII->f, sPoints, cmdDrawImage3->imgID,cmdFII->flag);
    }
    void JCGraphicsCmdDispath::_rendercmd_drawPath(JCMemClass& cmdBuffer)
    {
        CMDDrawPath *cmdDrawPath = cmdBuffer.popp<CMDDrawPath>();
        int  nPointLen = cmdDrawPath->pointsLen;
        int  nBrushLen = cmdDrawPath->brushLen;
        int  nPenLen = cmdDrawPath->penLen;
        int  nFillColor = -1;
        char * sPointBuffer = (char *)cmdBuffer.readBuffer(nPointLen);
        bool bFlag = true;
        if (nBrushLen > 0)
        {
            nFillColor = *(int*)cmdBuffer.readBuffer(4);
        }
        else
        {
            bFlag = false;
        }
        if (nPenLen<=0)
        {
            if (nBrushLen < 0)return;
            m_pGraphics->drawPath(cmdDrawPath->x, cmdDrawPath->y,nFillColor,sPointBuffer, nPointLen,true);
        }
        else if (nPenLen>0)
        {
            CMDPen *cmdPen = cmdBuffer.popp<CMDPen>();
            m_pGraphics->drawPath(cmdDrawPath->x, cmdDrawPath->y, nFillColor, cmdPen->strokeColor, cmdPen->lineJoin, cmdPen->lineCap, cmdPen->miterLimit, cmdPen->lineWidth, sPointBuffer, nPointLen, bFlag);
        }

    }
    void JCGraphicsCmdDispath::_rendercmd_drawLine(JCMemClass& cmdBuffer)
    {
        CMDDrawLine *cmdDrawLine = cmdBuffer.popp<CMDDrawLine>();
        m_pGraphics->drawLine(cmdDrawLine->x, cmdDrawLine->y, cmdDrawLine->w, cmdDrawLine->h, cmdDrawLine->lc, cmdDrawLine->lineWidth);
    }
    void JCGraphicsCmdDispath::_rendercmd_drawLines(JCMemClass& cmdBuffer)
    {
        CMDDrawLines *cmdDrawLines = cmdBuffer.popp<CMDDrawLines>();
        char * sBuffer = cmdBuffer.readBuffer(cmdDrawLines->pl * 4);
        m_pGraphics->drawLine(cmdDrawLines->x, cmdDrawLines->y, cmdDrawLines->lc, cmdDrawLines->z, sBuffer, cmdDrawLines->pl);
    }
    void JCGraphicsCmdDispath::_renderCmd_drawCurves(JCMemClass& cmdBuffer)
    {
        CMDDrawLines *cmdDrawLines = cmdBuffer.popp<CMDDrawLines>();
        char * sBuffer = cmdBuffer.readBuffer(cmdDrawLines->pl * 4);
        m_pGraphics->drawCurves(cmdDrawLines->x, cmdDrawLines->y, cmdDrawLines->lc, cmdDrawLines->z, sBuffer, cmdDrawLines->pl);
    }

    void JCGraphicsCmdDispath::_renderCmd_replaceTextColor(JCMemClass& cmdBuffer)
    {
        
    }

    void JCGraphicsCmdDispath::_renderCmd_fillImage(JCMemClass& cmdBuffer)
    {
        CMDFillImage *cmdFillImage = cmdBuffer.popp<CMDFillImage>();
        m_pGraphics->fillImage(cmdFillImage->imgID, cmdFillImage->repetition, cmdFillImage->args);
    }

    void JCGraphicsCmdDispath::_renderCmd_setSkinMesh(JCMemClass& cmdBuffer)
    {
        CMDSkinMesh *cmdSkinMesh = cmdBuffer.popp<CMDSkinMesh>();
        m_pGraphics->setSkinMesh(cmdSkinMesh->ibId, cmdSkinMesh->vbId, cmdSkinMesh->vertType, cmdSkinMesh->imgId, cmdSkinMesh->shaderId, cmdSkinMesh->startIndex, cmdSkinMesh->numElement, cmdSkinMesh->mat);
    }

    void JCGraphicsCmdDispath::_rendercmd_drawParticle(JCMemClass& cmdBuffer)
    {
        CMDInt *cmdDrawParticle = cmdBuffer.popp<CMDInt>();
        m_pGraphics->drawParticle(cmdDrawParticle->i);
        //m_pGraphics->m_pSaveCmd->append(cmdDrawParticle);
    }

	void JCGraphicsCmdDispath::_rendercmd_drawImageS(JCMemClass& cmdBuffer)
	{
		CMDDrawImageCanvas9 * cmdDrawImage9 = cmdBuffer.popp<CMDDrawImageCanvas9>();
		int len= *(int*)cmdBuffer.readBuffer(4);
		char * sBuffer = cmdBuffer.readBuffer(len);
		m_pGraphics->drawImageS(cmdDrawImage9->imgID, cmdDrawImage9->sx, cmdDrawImage9->sy, cmdDrawImage9->sw, cmdDrawImage9->sh, cmdDrawImage9->dx, cmdDrawImage9->dy, cmdDrawImage9->dw, cmdDrawImage9->dh, cmdDrawImage9->nType, len, sBuffer);
	}
    void JCGraphicsCmdDispath::_rendercmd_replaceText(JCMemClass& cmdBuffer)
    {

    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void JCGraphicsCmdDispath::_savecmd_clear(JCMemClass& cmdBuffer)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        m_pGraphics->clear();
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_destory(JCMemClass& cmdBuffer)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        //m_pGraphics->destory();
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_alpha(JCMemClass& cmdBuffer)
    {
        CMDFloat* cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pGraphics->m_pSaveCmd->append(*cmdFloat);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_rotate(JCMemClass& cmdBuffer)
    {
        CMDFloat3* cmdFloat3 = cmdBuffer.popp<CMDFloat3>();
        m_pGraphics->m_pSaveCmd->append(*cmdFloat3);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_transform(JCMemClass& cmdBuffer)
    {
        CMDMatrix* cmdMatrix = cmdBuffer.popp<CMDMatrix>();
        m_pGraphics->m_pSaveCmd->append(*cmdMatrix);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_scale(JCMemClass& cmdBuffer)
    {
        CMDFloatRect *cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
        m_pGraphics->m_pSaveCmd->append(*cmdFloatRect);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_translate(JCMemClass& cmdBuffer)
    {
        CMDFloatPoint *cmdFloatPoint = cmdBuffer.popp<CMDFloatPoint>();
        m_pGraphics->m_pSaveCmd->append(*cmdFloatPoint);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_save(JCMemClass& cmdBuffer)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        m_pGraphics->m_pSaveCmd->append(*cmdnoParam);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_restore(JCMemClass& cmdBuffer)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        m_pGraphics->m_pSaveCmd->append(*cmdnoParam);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_clipRect(JCMemClass& cmdBuffer)
    {
        CMDFloatRect *cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
        m_pGraphics->m_pSaveCmd->append(*cmdFloatRect);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_blendMode(JCMemClass& cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pGraphics->m_pSaveCmd->append(*cmdInt);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_fillText(JCMemClass& cmdBuffer)
    {
        CMDFillText1 * cmdFillText1 = cmdBuffer.popp<CMDFillText1>();
        int nfontLen = *(int*)cmdBuffer.readBuffer(4);
        char *sfont = (char *)cmdBuffer.readBuffer(nfontLen);
        int nTextLen = *(int *)cmdBuffer.readBuffer(4);
        char *sText = (char*)cmdBuffer.readBuffer(nTextLen);

        m_pGraphics->m_pSaveCmd->append(*cmdFillText1);
        /*
        m_pGraphics->m_pSaveCmd->append(nfontLen);
        m_pGraphics->m_pSaveCmd->append(sfont, nfontLen);
        m_pGraphics->m_pSaveCmd->append(nTextLen);
        m_pGraphics->m_pSaveCmd->append(sText, nTextLen);
        */
        //保存文字信息
        {
            int nColor = (m_pGraphics->m_bIsColorReplace) ? m_pGraphics->m_nReplaceColor : cmdFillText1->maxWidth;
            //maxWidth 是color
            m_pGraphics->setTextInfo(sText, sfont, nColor);
        }
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_fillBorderText(JCMemClass& cmdBuffer)
    {
        CMDFillBorderText* cmdFillBorderText = cmdBuffer.popp<CMDFillBorderText>();
        char* sFont = cmdBuffer.readBuffer(cmdFillBorderText->nFontLen);
        int nTextlen = *(int*)cmdBuffer.readBuffer(4);
        char* sText = cmdBuffer.readBuffer(nTextlen);

        m_pGraphics->m_pSaveCmd->append(*cmdFillBorderText);
        /*
        m_pGraphics->m_pSaveCmd->append(sFont, cmdFillBorderText->nFontLen);
        m_pGraphics->m_pSaveCmd->append(nTextlen);
        m_pGraphics->m_pSaveCmd->append(sText, nTextlen);
        */
        //保存文字信息
        {
            int nColor = (m_pGraphics->m_bIsColorReplace) ? m_pGraphics->m_nReplaceColor : cmdFillBorderText->nFillColor;
            //maxWidth 是color
            m_pGraphics->setTextInfo(sText, sFont, nColor);
        }
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_drawRect(JCMemClass& cmdBuffer)
    {
        char * sBuffer = (char*)cmdBuffer.readBuffer(36);
        m_pGraphics->m_pSaveCmd->append(sBuffer, 36);
       /* CMDFloatRect *cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
        CMDFII *cmdFII = cmdBuffer.popp<CMDFII>();
        m_pGraphics->m_pSaveCmd->append(*cmdFloatRect);
        m_pGraphics->m_pSaveCmd->append(*cmdFII);*/
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_drawCircle(JCMemClass& cmdBuffer)
    {
        char * sBuffer = (char*)cmdBuffer.readBuffer(32);
        m_pGraphics->m_pSaveCmd->append(sBuffer, 32);
       /* CMDFloatRect *cmdFloatRect= cmdBuffer.popp<CMDFloatRect>();
        CMDOIntPoint *cmdPoint = cmdBuffer.popp<CMDOIntPoint>();
        m_pGraphics->m_pSaveCmd->append(*cmdFloatRect);
        m_pGraphics->m_pSaveCmd->append(*cmdPoint);*/
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_drawPie(JCMemClass& cmdBuffer)
    {
        char * sBuffer = (char*)cmdBuffer.readBuffer(40);
        m_pGraphics->m_pSaveCmd->append(sBuffer, 40);
        /*CMDMatrix *cmdMatrix = cmdBuffer.popp<CMDMatrix>();
        CMDOIntPoint *cmdPoint = cmdBuffer.popp<CMDOIntPoint>();
        m_pGraphics->m_pSaveCmd->append(*cmdMatrix);
        m_pGraphics->m_pSaveCmd->append(*cmdPoint);*/
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_drawImage(JCMemClass& cmdBuffer)
    {
        CMDDrawImageCanvas9Alpha *cmdDrawImage9 = cmdBuffer.popp<CMDDrawImageCanvas9Alpha>();
        m_pGraphics->m_pSaveCmd->append(*cmdDrawImage9);
        m_pGraphics->setDrawImageFlag(cmdDrawImage9->imgID, cmdDrawImage9->sx, cmdDrawImage9->sy, cmdDrawImage9->sw, cmdDrawImage9->sh, cmdDrawImage9->dx, cmdDrawImage9->dy, cmdDrawImage9->dw, cmdDrawImage9->dh,cmdDrawImage9->alpha,cmdDrawImage9->nType);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_drawImageM(JCMemClass& cmdBuffer)
    {
        CMDDrawImageCanvasM *cmdDrawImageM = cmdBuffer.popp<CMDDrawImageCanvasM>();
        m_pGraphics->m_pSaveCmd->append(*cmdDrawImageM);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_drawPoly(JCMemClass& cmdBuffer)
    {
        CMDDrawImage3 *cmdDrawImage3 = cmdBuffer.popp<CMDDrawImage3>();
        int nLen = cmdDrawImage3->imgID * 4 + 16;
        char * sBuffer = (char*)cmdBuffer.readBuffer(nLen);
        m_pGraphics->m_pSaveCmd->append(*cmdDrawImage3);
        m_pGraphics->m_pSaveCmd->append(sBuffer, nLen);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_drawPath(JCMemClass& cmdBuffer)
    {
        CMDDrawPath *cmdDrawPath = cmdBuffer.popp<CMDDrawPath>();
        m_pGraphics->m_pSaveCmd->append(*cmdDrawPath);
        int nLen = (cmdDrawPath->brushLen + cmdDrawPath->penLen ) * 4+cmdDrawPath->pointsLen;
        if (nLen > 0)
        {
            char * sBuffer = (char *)cmdBuffer.readBuffer(nLen);
            m_pGraphics->m_pSaveCmd->append(sBuffer,nLen);
        }
        //m_pGraphics->m_pSaveCmd->append();
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_drawLine(JCMemClass& cmdBuffer)
    {
        CMDDrawLine *cmdDrawLine = cmdBuffer.popp<CMDDrawLine>();
        m_pGraphics->m_pSaveCmd->append(*cmdDrawLine);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_drawLines(JCMemClass& cmdBuffer)
    {
        CMDDrawLines *cmdDrawLines = cmdBuffer.popp<CMDDrawLines>();
        char * sBuffer = cmdBuffer.readBuffer(cmdDrawLines->pl * 4);
        m_pGraphics->m_pSaveCmd->append(*cmdDrawLines);
        m_pGraphics->m_pSaveCmd->append(sBuffer, cmdDrawLines->pl * 4);
        m_pGraphics->repaint();
    }
    void JCGraphicsCmdDispath::_savecmd_drawCurves(JCMemClass& cmdBuffer)
    {
        CMDDrawLines *cmdDrawLines = cmdBuffer.popp<CMDDrawLines>();
        char * sBuffer = cmdBuffer.readBuffer(cmdDrawLines->pl * 4);
        m_pGraphics->m_pSaveCmd->append(*cmdDrawLines);
        m_pGraphics->m_pSaveCmd->append(sBuffer, cmdDrawLines->pl * 4);
        m_pGraphics->repaint();
    }

    void JCGraphicsCmdDispath::_savecmd_replaceTextColor(JCMemClass& cmdBuffer)
    {
        CMDInt *cmdInt = cmdBuffer.popp<CMDInt>();
        m_pGraphics->replaceTextColor(cmdInt->i);
        m_pGraphics->repaint();
    }

    void JCGraphicsCmdDispath::_savecmd_fillImage(JCMemClass& cmdBuffer)
    {
        CMDFillImage* cmdFillImage=cmdBuffer.popp<CMDFillImage>();
        m_pGraphics->m_pSaveCmd->append(*cmdFillImage);
        m_pGraphics->repaint();
    }

    void JCGraphicsCmdDispath::_savecmd_setSkinMesh(JCMemClass& cmdBuffer)
    {
        CMDSkinMesh * cmdSkinMesh = cmdBuffer.popp<CMDSkinMesh>();
        m_pGraphics->m_pSaveCmd->append(*cmdSkinMesh);
        m_pGraphics->repaint();
    }

    void JCGraphicsCmdDispath::_savecmd_drawParticle(JCMemClass& cmdBuffer)
    {
        CMDInt *cmdDrawParticle = cmdBuffer.popp<CMDInt>();
        m_pGraphics->m_pSaveCmd->append(*cmdDrawParticle); 
        m_pGraphics->repaint();
    }

	void JCGraphicsCmdDispath::_savecmd_drawImageS(JCMemClass& cmdBuffer)
	{
		CMDDrawImageCanvas9 * cmdDrawImage9 = cmdBuffer.popp<CMDDrawImageCanvas9>();
		m_pGraphics->m_pSaveCmd->append(*cmdDrawImage9);
		int len = *(int*)cmdBuffer.readBuffer(4);
		m_pGraphics->m_pSaveCmd->append(len);
		char * sBuffer = cmdBuffer.readBuffer(len);
		m_pGraphics->m_pSaveCmd->append(sBuffer,len);
        m_pGraphics->repaint();
	}
    void JCGraphicsCmdDispath::_savecmd_replaceText(JCMemClass& cmdBuffer)
    {
        CMDInt *cmdInt = cmdBuffer.popp<CMDInt>();
        char* sText = (char*)cmdBuffer.readBuffer(cmdInt->i);
        m_pGraphics->replaceText(sText);
        m_pGraphics->repaint();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void JCGraphicsCmdDispath::_calcBoundingBox_clear(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_destory(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_alpha(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDFloat* cmdFloat = cmdBuffer.popp<CMDFloat>();
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_rotate(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDFloat3* cmdFloat3 = cmdBuffer.popp<CMDFloat3>();
        Matrix32 kMatrix;
        kMatrix.translate(-cmdFloat3->y, -cmdFloat3->z);
        kMatrix.rotate(cmdFloat3->x);
        kMatrix.translate(cmdFloat3->y, cmdFloat3->z);
        kCurrentMatrix.concat( &kMatrix );
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_transform(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDMatrix* cmdMatrix = cmdBuffer.popp<CMDMatrix>();
        //JS原版翻译
        /*
        Matrix32 kTempMat;
        kTempMat.setTransform(cmdMatrix->a, cmdMatrix->b, cmdMatrix->c, cmdMatrix->d, 0, 0);
        Matrix32 kMatrix;
        kMatrix.translate( -cmdMatrix->tx, -cmdMatrix->ty );
        kMatrix.concat(&kTempMat);
        kMatrix.translate(cmdMatrix->tx, cmdMatrix->ty);
        kCurrentMatrix.concat(&kMatrix);
        */
        Matrix32 kMatrix;
        kMatrix.setTransform(cmdMatrix->a, cmdMatrix->b, cmdMatrix->c, cmdMatrix->d, cmdMatrix->tx, cmdMatrix->ty);
        kCurrentMatrix.concat(&kMatrix);
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_scale(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDFloatRect* cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
        Matrix32 kMatrix;
        kMatrix.translate(-cmdFloatRect->w, -cmdFloatRect->h);
        kMatrix.scale(cmdFloatRect->x,cmdFloatRect->y);
        kMatrix.translate(cmdFloatRect->w, cmdFloatRect->h);
        kCurrentMatrix.concat(&kMatrix);
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_translate(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDFloatPoint* cmdFloatPoint = cmdBuffer.popp<CMDFloatPoint>();
        kCurrentMatrix.translate(cmdFloatPoint->x, cmdFloatPoint->y);
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_save(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        vMatrixs.push_back(kCurrentMatrix);
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_restore(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        kCurrentMatrix = vMatrixs[vMatrixs.size() - 1];
        vMatrixs.pop_back();
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_clipRect(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDFloatRect *cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_blendMode(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_fillText(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDFillText1 * cmdFillText1 = cmdBuffer.popp<CMDFillText1>();
        /*
        int nfontLen = *(int*)cmdBuffer.readBuffer(4);
        char *sfont = (char *)cmdBuffer.readBuffer(nfontLen);
        int nTextLen = *(int *)cmdBuffer.readBuffer(4);
        char *sText = (char*)cmdBuffer.readBuffer(nTextLen);
        */
        //没有处理，因为LayaAir就没有处理
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_fillBorderText(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDFillBorderText* cmdFillBorderText = cmdBuffer.popp<CMDFillBorderText>();
        /*
        char* sFont = cmdBuffer.readBuffer(cmdFillBorderText->nFontLen);
        int nTextlen = *(int*)cmdBuffer.readBuffer(4);
        char* sText = cmdBuffer.readBuffer(nTextlen);
        */
        //没有处理，因为LayaAir就没有处理
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_drawRect(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDFloatRect* cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
        CMDFII* cmdFII = cmdBuffer.popp<CMDFII>();
        addPointResult(vResultPointArray, cmdFloatRect->x, cmdFloatRect->y, cmdFloatRect->w, cmdFloatRect->h, &kCurrentMatrix);
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_drawCircle(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDFloatRect* cmdFloatRect = cmdBuffer.popp<CMDFloatRect>();
        CMDOIntPoint* cmdPoint = cmdBuffer.popp<CMDOIntPoint>();
        int nFlag = *(int *)cmdBuffer.readBuffer(4);
        addPointResult(vResultPointArray, cmdFloatRect->x-cmdFloatRect->w, cmdFloatRect->y-cmdFloatRect->w, cmdFloatRect->w*2, cmdFloatRect->w*2, &kCurrentMatrix);
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_drawPie(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDMatrix *cmdMatrix = cmdBuffer.popp<CMDMatrix>();
        CMDOIntPoint *cmdPoint = cmdBuffer.popp<CMDOIntPoint>();
        int nFlag = *(int*)cmdBuffer.readBuffer(4);
        addPointResult(vResultPointArray, cmdMatrix->a, cmdMatrix->b, cmdMatrix->c, cmdMatrix->d, cmdMatrix->tx,&kCurrentMatrix );
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_drawImage(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDDrawImageCanvas9Alpha *cmdDrawImage9 = cmdBuffer.popp<CMDDrawImageCanvas9Alpha>();
        addPointResult(vResultPointArray, cmdDrawImage9->dx, cmdDrawImage9->dy, cmdDrawImage9->dw, cmdDrawImage9->dh, &kCurrentMatrix);
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_drawImageM(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDDrawImageCanvasM* cmdDrawImageM = cmdBuffer.popp<CMDDrawImageCanvasM>();
        Matrix32 tempMatrix;
        tempMatrix.setTransform(cmdDrawImageM->a, cmdDrawImageM->b, cmdDrawImageM->c, cmdDrawImageM->d, cmdDrawImageM->tx, cmdDrawImageM->ty );
        Matrix32 kMatrix = kCurrentMatrix;
        kMatrix.concat(&tempMatrix);
        addPointResult(vResultPointArray, cmdDrawImageM->dx, cmdDrawImageM->dy, cmdDrawImageM->dw, cmdDrawImageM->dh, &kMatrix);
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_drawPoly(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDDrawImage3 *cmdDrawImage3 = cmdBuffer.popp<CMDDrawImage3>();
        CMDFII* cmdFII = cmdBuffer.popp<CMDFII>();
        float* sPoints = (float*)cmdBuffer.readBuffer(cmdDrawImage3->imgID * 4);
        int nPointNum = cmdDrawImage3->imgID;
        if (nPointNum >= 2 && nPointNum % 2 == 0)
        {
            int x = cmdDrawImage3->x;
            int y = cmdDrawImage3->y;
            float fX = 0, fY = 0;
            for (int i = 0; i < nPointNum; i += 2)
            {
                kCurrentMatrix.transVert(x + sPoints[i], y + sPoints[i + 1], fX, fY);
                vResultPointArray.push_back(fX);
                vResultPointArray.push_back(fY);
            }
        }
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_drawPath(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDDrawPath *cmdDrawPath = cmdBuffer.popp<CMDDrawPath>();
        int dx = cmdDrawPath->x;
        int dy = cmdDrawPath->y;
        int  nBufferLength = cmdDrawPath->pointsLen;
        int  nBrushLen = cmdDrawPath->brushLen;
        int  nPenLen = cmdDrawPath->penLen;
        int  nFillColor = -1;
        char * sPointBuffer = (char *)cmdBuffer.readBuffer(nBufferLength);
        int x1, y1, x2, y2, r;
        while (nBufferLength > 0)
        {
            int type = *(int*)sPointBuffer;
            sPointBuffer += 4;
            switch (type)
            {
            case JCGraphics::TYPE_MOVETO:
            case JCGraphics::TYPE_LINETO:
                x1 = *(float *)sPointBuffer; sPointBuffer += 4;
                y1 = *(float *)sPointBuffer; sPointBuffer += 4;
                addPointResult(vResultPointArray, x1 + dx, y1 + dy, &kCurrentMatrix);
                nBufferLength -= 12;
                break;
            case JCGraphics::TYPE_ARCTO:
                x1 = *(float *)sPointBuffer; sPointBuffer += 4;
                y1 = *(float *)sPointBuffer; sPointBuffer += 4;
                x2 = *(float *)sPointBuffer; sPointBuffer += 4;
                y2 = *(float *)sPointBuffer; sPointBuffer += 4;
                r = *(float *)sPointBuffer; sPointBuffer += 4;
                addPointResult(vResultPointArray, x2 + dx, y2 + dy, &kCurrentMatrix);
                nBufferLength -= 24;
                break;
            case JCGraphics::TYPE_CLOSEPATH:
                nBufferLength -= 4;
                break;
            default:
                break;
            }
        }

        if (nBrushLen > 0)
        {
            nFillColor = *(int*)cmdBuffer.readBuffer(4);
        }
        if (nPenLen > 0)
        {
            cmdBuffer.popp<CMDPen>();
        }
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_drawLine(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDDrawLine *cmdDrawLine = cmdBuffer.popp<CMDDrawLine>();
        addPointResult(vResultPointArray, cmdDrawLine->x, cmdDrawLine->y,&kCurrentMatrix);
        addPointResult(vResultPointArray, cmdDrawLine->w, cmdDrawLine->h, &kCurrentMatrix);
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_drawLines(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDDrawLines *cmdDrawLines = cmdBuffer.popp<CMDDrawLines>();
        int nPointNum = cmdDrawLines->pl;
        float* sPoints = (float*)cmdBuffer.readBuffer(nPointNum * 4);
        if (nPointNum >= 2 && nPointNum % 2 == 0)
        {
            float fX = 0, fY = 0;
            float dx = cmdDrawLines->x;
            float dy = cmdDrawLines->y;
            for (int i = 0; i < nPointNum; i += 2)
            {
                kCurrentMatrix.transVert(sPoints[i] + dx, sPoints[i + 1] + dy, fX, fY);
                vResultPointArray.push_back(fX);
                vResultPointArray.push_back(fY);
            }
        }
    }
    void JCGraphicsCmdDispath::_calcBoundingBox_drawCurves(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDDrawLines *cmdDrawLines = cmdBuffer.popp<CMDDrawLines>();
        int nPointNum = cmdDrawLines->pl;
        float* sPoints = (float*)cmdBuffer.readBuffer(nPointNum * 4);
        if (nPointNum >= 2 && nPointNum % 2 == 0)
        {
            float fX = 0, fY = 0;
            float dx = cmdDrawLines->x;
            float dy = cmdDrawLines->y;
            for (int i = 0; i < nPointNum; i += 2)
            {
                kCurrentMatrix.transVert(sPoints[i] + dx, sPoints[i + 1] + dy, fX, fY);
                vResultPointArray.push_back(fX);
                vResultPointArray.push_back(fY);
            }
        }
    }


    void JCGraphicsCmdDispath::_calcBoundingBox_replaceTextColor(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        
    }

    void JCGraphicsCmdDispath::_calcBoundingBox_fillImage(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDFillImage *cmdFillImage = cmdBuffer.popp<CMDFillImage>();
        addPointResult(vResultPointArray, cmdFillImage->args[4]+cmdFillImage->args[6], cmdFillImage->args[5] + cmdFillImage->args[7], cmdFillImage->args[8],cmdFillImage->args[9], &kCurrentMatrix);
    }

    void JCGraphicsCmdDispath::_calcBoundingBox_setSkinMesh(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
        CMDSkinMesh* cmdSkinMesh = cmdBuffer.popp<CMDSkinMesh>();
    }

    void JCGraphicsCmdDispath::_calcBoundingBox_drawParticle(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {
         CMDInt *cmdDrawParticle = cmdBuffer.popp<CMDInt>();
    }

	void JCGraphicsCmdDispath::_calcBoundingBox_drawImageS(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
	{
		CMDDrawImageCanvas9 * cmdDrawImage9 = cmdBuffer.popp<CMDDrawImageCanvas9>();
		int len = *(int*)cmdBuffer.readBuffer(4);
		char * sBuffer = cmdBuffer.readBuffer(len);
	}

    void JCGraphicsCmdDispath::_calcBoundingBox_replaceText(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray)
    {

    }

    void JCGraphicsCmdDispath::addPointResult(std::vector<float>& pResultArrays, float x, float y, float radius, float startAngle, float endAngle, Matrix32* pMatrix)
    {
        float dP = PI / 8;
        float i = 0;
        float fX = 0, fY = 0;
        for (i = startAngle; i < endAngle; i += dP)
        {
            pMatrix->transVert(x + radius * cos(i), y + radius * sin(i), fX, fY);
            pResultArrays.push_back(fX);
            pResultArrays.push_back(fY);
        }
        if (endAngle != i)
        {
            pMatrix->transVert(x + radius * cos(endAngle), y + radius * sin(endAngle), fX, fY);
            pResultArrays.push_back(fX);
            pResultArrays.push_back(fY);
        }
    }
    void JCGraphicsCmdDispath::addPointResult(std::vector<float>& pResultArrays, float x, float y, Matrix32* pMatrix)
    {
        float fX = 0, fY = 0;
        pMatrix->transVert(x, y, fX, fY);
        pResultArrays.push_back(fX);
        pResultArrays.push_back(fY);
    }
    void JCGraphicsCmdDispath::addPointResult(std::vector<float>& pResultArrays, float x, float y, float w, float h, Matrix32* pMatrix)
    {
        if (w <= 0 || h <= 0) return;
        float fX = 0, fY = 0;
        pMatrix->transVert(x, y, fX, fY);
        pResultArrays.push_back(fX);
        pResultArrays.push_back(fY);

        pMatrix->transVert(x+w, y, fX, fY);
        pResultArrays.push_back(fX);
        pResultArrays.push_back(fY);

        pMatrix->transVert(x, y+h, fX, fY);
        pResultArrays.push_back(fX);
        pResultArrays.push_back(fY);

        pMatrix->transVert(x + w, y + h, fX, fY);
        pResultArrays.push_back(fX);
        pResultArrays.push_back(fY);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
