/**
@file			JCHtml5Render.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_27
*/

#include "JCHtml5Render.h"
#include "JCHtml5RenderManager.h"
#include <util/JCCmdStruct.h>
#include <Bridge/JCConchBridge.h>

namespace laya
{
    JCHtml5Render::JCHtml5Render(int nID,JCResManager* pTextureManager, JCImageManager* pImageManager, JC2DShaderManager* p2DShaderManager,JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager, JCBufferManager* pBufferManager, JCShaderManager* pShaderManager, std::vector<JCICmdDispatch*>* vVectorCmdDispat)
    {
        m_nID = nID;
        m_pContext = new JCHtml5Context(this,pTextureManager,pImageManager, p2DShaderManager,pHtml5RenderManager,pTextManager,pFontManager,pBufferManager,pShaderManager, vVectorCmdDispat);
        m_nNoteFrame = 0;
    }
    JCHtml5Render::~JCHtml5Render()
    {
        if (m_pContext)
        {
            delete m_pContext;
            m_pContext = NULL;
        }
    }
    void JCHtml5Render::onGLReady()
    {

    }
    void JCHtml5Render::renderFrame(long nCurrentFrame, bool bStopRender)
    {
    }
    void JCHtml5Render::invalidGLRes()
    {
        m_pContext->invalidGLRes();
    }
    void JCHtml5Render::setCanvasType(CANVAS_TYPE nType)
    {
        m_pContext->setCanvasType(nType);
    }
    void JCHtml5Render::setMemoryCanvasClip(bool bClip)
    {
        m_pContext->setMemoryCanvasClip(bClip);
    }
    void JCHtml5Render::setCanvasSize(int w, int h)
    {
        m_pContext->setCanvasSize(w, h);
    }
    bool JCHtml5Render::JCHtml5Render::dispatchRenderCmd(JCMemClass& pRenderCmd,int nFrameCount)
    {
        if (m_pContext->m_nCanvasType == CANVAS_TYPE_MAIN)
        {
            return runRenderCmd(pRenderCmd);
        }
        else
        {
            //这个判断是为了避免由于用户使用不当，忘记清空canvas数据
            if (m_nNoteFrame != 0 && m_nNoteFrame != nFrameCount)
            {
                m_pContext->clearSaveCmd();
            }
            m_nNoteFrame = nFrameCount;
            return saveRenderCmd(pRenderCmd);
        }
        return true;
    }
    bool JCHtml5Render::dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount)
    {
        return dispatchRenderCmd(pRenderCmd, nFrameCount);
    }
    bool JCHtml5Render::runRenderCmd(JCMemClass& pRenderCmd)
    {
        static const ProcFunction g_svProcFunctions[] =
        {
            &JCHtml5Render::_rendercmd_save,//0
            &JCHtml5Render::_rendercmd_restore,
            &JCHtml5Render::_rendercmd_beginPath,
            &JCHtml5Render::_rendercmd_drawImage3,
            &JCHtml5Render::_rendercmd_drawImage5,
            &JCHtml5Render::_rendercmd_drawImage9,//5
            &JCHtml5Render::_rendercmd_rect,
            &JCHtml5Render::_rendercmd_clip,
            &JCHtml5Render::_rendercmd_scale,
            &JCHtml5Render::_rendercmd_rotate,
            &JCHtml5Render::_rendercmd_translate,//10
            &JCHtml5Render::_rendercmd_transform,
            &JCHtml5Render::_rendercmd_setTransform,
            &JCHtml5Render::_rendercmd_globalAlpha,
            &JCHtml5Render::_rendercmd_clearRect,
            &JCHtml5Render::_rendercmd_closePath,//15
            &JCHtml5Render::_rendercmd_fillStyle,
            &JCHtml5Render::_rendercmd_fillRect,
            &JCHtml5Render::_rendercmd_strokeStyle,
            &JCHtml5Render::_rendercmd_stroke,
            &JCHtml5Render::_rendercmd_strokeRect,//20
            &JCHtml5Render::_rendercmd_strokeText,
            &JCHtml5Render::_rendercmd_strokeText2,
            &JCHtml5Render::_rendercmd_fillText,
            &JCHtml5Render::_rendercmd_fillText1,
            &JCHtml5Render::_rendercmd_fill,//25
            &JCHtml5Render::_rendercmd_lineCap,
            &JCHtml5Render::_rendercmd_lineWidth,
            &JCHtml5Render::_rendercmd_lineJoin,
            &JCHtml5Render::_rendercmd_moveTo,
            &JCHtml5Render::_rendercmd_lineTo,//30
            &JCHtml5Render::_rendercmd_arc,
            &JCHtml5Render::_rendercmd_arcTo,
            &JCHtml5Render::_rendercmd_bezierCurveTo,
            &JCHtml5Render::_rendercmd_quadraticCurveTo,
            &JCHtml5Render::_rendercmd_drawCanvas3,//drawCanvas3 35
            &JCHtml5Render::_rendercmd_drawCanvas5,//drawCanvas5
            &JCHtml5Render::_rendercmd_drawCanvas9,//drawCanvas9
            &JCHtml5Render::_rendercmd_font,//38
            &JCHtml5Render::_rendercmd_globalCompositeOperation,//_rendercmd_globalCompositeOperation
            &JCHtml5Render::_rendercmd_textBaseline,//textbaseLine 40
            &JCHtml5Render::_rendercmd_textAlign,
            &JCHtml5Render::_rendercmd_miterLimit,
            &JCHtml5Render::_rendercmd_shadowOffsetX,
            &JCHtml5Render::_rendercmd_shadowOffsetY,
            &JCHtml5Render::_rendercmd_shadowColor,
            &JCHtml5Render::_rendercmd_shadowBlur,
            &JCHtml5Render::_rendercmd_fillStyle1,
            &JCHtml5Render::_rendercmd_setFilter,
            &JCHtml5Render::_rendercmd_fillBorderText,
            &JCHtml5Render::_rendercmd_clipRect,
            &JCHtml5Render::_rendercmd_setIBVB,
            &JCHtml5Render::_rendercmd_setFilterMatrix,
            &JCHtml5Render::_rendercmd_setSkinMesh,
            &JCHtml5Render::_rendercmd_drawParticle2D,
            &JCHtml5Render::_rendercmd_getPixels,
            &JCHtml5Render::_rendercmd_toBase64,
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
                LOGE("JCHtml5Render::runRenderCmd Wrong command number!,cmd=%d", nFunctionID);
            } 
        }
        return true;
    }
    bool JCHtml5Render::saveRenderCmd(JCMemClass& pRenderCmd)
    {
        static const ProcFunction g_svSaveProcFunctions[] =
        {
            &JCHtml5Render::_save_rendercmd_save,//0
            &JCHtml5Render::_save_rendercmd_restore,
            &JCHtml5Render::_save_rendercmd_beginPath,
            &JCHtml5Render::_save_rendercmd_drawImage3,
            &JCHtml5Render::_save_rendercmd_drawImage5,
            &JCHtml5Render::_save_rendercmd_drawImage9,//5
            &JCHtml5Render::_save_rendercmd_rect,
            &JCHtml5Render::_save_rendercmd_clip,
            &JCHtml5Render::_save_rendercmd_scale,
            &JCHtml5Render::_save_rendercmd_rotate,
            &JCHtml5Render::_save_rendercmd_translate,//10
            &JCHtml5Render::_save_rendercmd_transform,
            &JCHtml5Render::_save_rendercmd_setTransform,
            &JCHtml5Render::_save_rendercmd_globalAlpha,
            &JCHtml5Render::_save_rendercmd_clearRect,
            &JCHtml5Render::_save_rendercmd_closePath,//15
            &JCHtml5Render::_save_rendercmd_fillStyle,
            &JCHtml5Render::_save_rendercmd_fillRect,
            &JCHtml5Render::_save_rendercmd_strokeStyle,
            &JCHtml5Render::_save_rendercmd_stroke,
            &JCHtml5Render::_save_rendercmd_strokeRect,//20
            &JCHtml5Render::_save_rendercmd_strokeText,
            &JCHtml5Render::_save_rendercmd_strokeText2,
            &JCHtml5Render::_save_rendercmd_fillText,
            &JCHtml5Render::_save_rendercmd_fillText1,
            &JCHtml5Render::_save_rendercmd_fill,//25
            &JCHtml5Render::_save_rendercmd_lineCap,
            &JCHtml5Render::_save_rendercmd_lineWidth,
            &JCHtml5Render::_save_rendercmd_lineJoin,
            &JCHtml5Render::_save_rendercmd_moveTo,
            &JCHtml5Render::_save_rendercmd_lineTo,//30
            &JCHtml5Render::_save_rendercmd_arc,
            &JCHtml5Render::_save_rendercmd_arcTo,
            &JCHtml5Render::_save_rendercmd_bezierCurveTo,
            &JCHtml5Render::_save_rendercmd_quadraticCurveTo,
            &JCHtml5Render::_save_rendercmd_drawCanvas3,
            &JCHtml5Render::_save_rendercmd_drawCanvas5,
            &JCHtml5Render::_save_rendercmd_drawCanvas9,
            &JCHtml5Render::_save_rendercmd_font,//38
            &JCHtml5Render::_save_rendercmd_globalCompositeOperation,
            &JCHtml5Render::_save_rendercmd_textBaseline,
            &JCHtml5Render::_save_rendercmd_textAlign,
            &JCHtml5Render::_save_rendercmd_miterLimit,
            &JCHtml5Render::_save_rendercmd_shadowOffsetX,
            &JCHtml5Render::_save_rendercmd_shadowOffsetY,
            &JCHtml5Render::_save_rendercmd_shadowColor,
            &JCHtml5Render::_save_rendercmd_shadowBlur,
            &JCHtml5Render::_save_rendercmd_fillStyle1,
            &JCHtml5Render::_save_rendercmd_setFilter,
            &JCHtml5Render::_save_rendercmd_fillBorderText,
            &JCHtml5Render::_save_rendercmd_clipRect,
            &JCHtml5Render::_save_rendercmd_setIBVB,
            &JCHtml5Render::_save_rendercmd_setFilterMatrix,
            &JCHtml5Render::_save_rendercmd_setSkinMesh,
            &JCHtml5Render::_save_rendercmd_drawParticle2D,
            &JCHtml5Render::_save_rendercmd_getPixels,
            &JCHtml5Render::_save_rendercmd_toBase64,
        };
        static const int nFuncs = sizeof(g_svSaveProcFunctions) / sizeof(g_svSaveProcFunctions[0]);
        char* pData = pRenderCmd.getReadPtr();
        if (pData)
        {
            int nFunctionID = *(int*)pData;
            if (nFunctionID >= 0 && nFunctionID < nFuncs)
            {
                (this->*g_svSaveProcFunctions[nFunctionID])(pRenderCmd);
            }
            else
            {
                LOGE("JCHtml5Render::saveRenderCmd Wrong command number!,cmd=%d", nFunctionID);
            }
        }
        return true;
    }
    bool JCHtml5Render::restartJSThread()
    {
        return true;
    }
    bool  JCHtml5Render::_rendercmd_save(JCMemClass&  cmdBuffer)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        m_pContext->save();
        return true;
    }
    bool  JCHtml5Render::_rendercmd_restore(JCMemClass&  cmdBuffer)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        m_pContext->restore();
        return true;
    }
    bool  JCHtml5Render::_rendercmd_beginPath(JCMemClass&  cmdBuffer)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        m_pContext->beginPath();
        return true;
    }
    bool  JCHtml5Render::_rendercmd_drawImage3(JCMemClass&  cmdBuffer)
    {
        CMDDrawImage3 * cmdDrawImage3 = cmdBuffer.popp<CMDDrawImage3>();
        m_pContext->drawImage(cmdDrawImage3->imgID, cmdDrawImage3->x, cmdDrawImage3->y);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_drawImage5(JCMemClass&  cmdBuffer)
    {
        CMDDrawImage5* cmdDrawImage5 = cmdBuffer.popp<CMDDrawImage5>();
        m_pContext->drawImage(cmdDrawImage5->imgID, cmdDrawImage5->x, cmdDrawImage5->y, cmdDrawImage5->w, cmdDrawImage5->h);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_drawImage9(JCMemClass&  cmdBuffer)
    {
        CMDDrawImage9* cmdDrawImage9 = cmdBuffer.popp<CMDDrawImage9>();
        m_pContext->drawImage(cmdDrawImage9->imgID, cmdDrawImage9->sx, cmdDrawImage9->sy, cmdDrawImage9->sw, cmdDrawImage9->sh, cmdDrawImage9->dx, cmdDrawImage9->dy, cmdDrawImage9->dw, cmdDrawImage9->dh);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_rect(JCMemClass&  cmdBuffer)
    {
        CMDFloatRect * rect = cmdBuffer.popp<CMDFloatRect>();
        m_pContext->rect(rect->x, rect->y, rect->w, rect->h);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_clip(JCMemClass&  cmdBuffer)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        m_pContext->clip();
        return true;
    }
    bool  JCHtml5Render::_rendercmd_scale(JCMemClass&  cmdBuffer)
    {
        CMDFloatPoint * scale = cmdBuffer.popp<CMDFloatPoint>();
        m_pContext->scale(scale->x, scale->y);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_rotate(JCMemClass&  cmdBuffer)
    {
        CMDFloat * cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pContext->rotate(cmdFloat->f);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_translate(JCMemClass&  cmdBuffer)
    {
        CMDFloatPoint * pos = cmdBuffer.popp<CMDFloatPoint>();
        m_pContext->translate(pos->x, pos->y);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_transform(JCMemClass&  cmdBuffer)
    {
        CMDMatrix *matrix= cmdBuffer.popp<CMDMatrix>();
        m_pContext->transform(matrix->a, matrix->b, matrix->c, matrix->d,matrix->tx,matrix->ty);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_setTransform(JCMemClass&  cmdBuffer)
    {
        CMDMatrix *matrix = cmdBuffer.popp<CMDMatrix>();
        m_pContext->setTransform(matrix->a, matrix->b, matrix->c, matrix->d, matrix->tx, matrix->ty);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_globalAlpha(JCMemClass&  cmdBuffer)
    {
        CMDFloat * cf= cmdBuffer.popp<CMDFloat>();
        m_pContext->setGlobalAlpha(cf->f);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_clearRect(JCMemClass&  cmdBuffer)
    {
        CMDFloatRect * rect = cmdBuffer.popp<CMDFloatRect>();
        m_pContext->clearRect(rect->x, rect->y, rect->w, rect->h);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_closePath(JCMemClass&  cmdBuffer)
    {
        cmdBuffer.popp<CMDNoParam>();
        m_pContext->closePath();
        return true;
    }
    bool  JCHtml5Render::_rendercmd_fillStyle(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->setFillStyle(cmdInt->i);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_fillRect(JCMemClass&  cmdBuffer)
    {
        CMDFloatRect * rect = cmdBuffer.popp<CMDFloatRect>();
        m_pContext->fillRect(rect->x, rect->y, rect->w, rect->h);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_strokeStyle(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->setStrokeStyle(cmdInt->i);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_stroke(JCMemClass&  cmdBuffer)
    {
        cmdBuffer.popp<CMDNoParam>();
        m_pContext->stroke();
        return true;
    }
    bool  JCHtml5Render::_rendercmd_strokeRect(JCMemClass&  cmdBuffer)
    {
        CMDFloatRect * rect = cmdBuffer.popp<CMDFloatRect>();
        m_pContext->strokeRect(rect->x, rect->y, rect->w, rect->h);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_strokeText(JCMemClass&  cmdBuffer)
    {
        CMDFillText * cmdStrokeText = cmdBuffer.popp<CMDFillText>();
        char * text = cmdBuffer.readBuffer(cmdStrokeText->len);
        m_pContext->strokeText(text, cmdStrokeText->x, cmdStrokeText->y);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_strokeText2(JCMemClass&  cmdBuffer)
    {
        CMDFillText1 * cmdStrokeText = cmdBuffer.popp<CMDFillText1>();
        char * text = (char*)cmdBuffer.readBuffer((int)cmdStrokeText->len);
        m_pContext->strokeText4(text, cmdStrokeText->x, cmdStrokeText->y, cmdStrokeText->maxWidth);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_fillText(JCMemClass&  cmdBuffer)
    {
        CMDFillText * cmdFillText= cmdBuffer.popp<CMDFillText>();
        char * text = cmdBuffer.readBuffer(cmdFillText->len);
        m_pContext->fillText(text, cmdFillText->x, cmdFillText->y);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_fillText1(JCMemClass&  cmdBuffer)
    {
        CMDFillText1 * cmdFillText = cmdBuffer.popp<CMDFillText1>();
        char * text = (char*)cmdBuffer.readBuffer((int)cmdFillText->len);
        m_pContext->fillText4(text, cmdFillText->x, cmdFillText->y,cmdFillText->maxWidth);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_fill(JCMemClass&  cmdBuffer)
    {
        cmdBuffer.popp<CMDNoParam>();
        m_pContext->fill();
        return true;
    }
    bool  JCHtml5Render::_rendercmd_lineCap(JCMemClass&  cmdBuffer)
    {
        CMDInt * cmdInt= cmdBuffer.popp<CMDInt>();
        m_pContext->setLineCap((LINECAP_TYPE)cmdInt->i);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_lineWidth(JCMemClass&  cmdBuffer)
    {
        CMDFloat *cmdFloat= cmdBuffer.popp<CMDFloat>();
        m_pContext->setLineWidth(cmdFloat->f);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_lineJoin(JCMemClass&  cmdBuffer)
    {
        CMDInt * cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->setLineJoin((LINEJOIN_TYPE)cmdInt->i);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_moveTo(JCMemClass&  cmdBuffer)
    {
        CMDFloatPoint * mt = cmdBuffer.popp<CMDFloatPoint>();
        m_pContext->moveTo(mt->x, mt->y);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_lineTo(JCMemClass&  cmdBuffer)
    {
        CMDFloatPoint * lt = cmdBuffer.popp<CMDFloatPoint>();
        m_pContext->lineTo(lt->x, lt->y);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_arc(JCMemClass&  cmdBuffer)
    {
        CMDArc *cmdArc=cmdBuffer.popp<CMDArc>();
        m_pContext->arc(cmdArc->x, cmdArc->y, cmdArc->r, cmdArc->sAngle, cmdArc->eAngle, cmdArc->bCounterclockwise > 0);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_arcTo(JCMemClass&  cmdBuffer)
    {
        CMDArcTo *cmdArcTo = cmdBuffer.popp<CMDArcTo>();
        m_pContext->arcTo(cmdArcTo->x1, cmdArcTo->y1, cmdArcTo->x2, cmdArcTo->y2, cmdArcTo->radium);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_bezierCurveTo(JCMemClass&  cmdBuffer)
    {
        CMDMatrix *martrix= cmdBuffer.popp<CMDMatrix>();
        m_pContext->bezierCurveTo(martrix->a, martrix->b, martrix->c, martrix->d, martrix->tx, martrix->ty);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_quadraticCurveTo(JCMemClass&  cmdBuffer)
    {
        CMDFloatRect *rect= cmdBuffer.popp<CMDFloatRect>();
        m_pContext->quadraticCurveTo(rect->x, rect->y, rect->w, rect->h);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_drawCanvas3(JCMemClass&  cmdBuffer)
    {
        CMDDrawImage3 * cmdDrawImage3 = cmdBuffer.popp<CMDDrawImage3>();
        m_pContext->drawCanvas(cmdDrawImage3->imgID, cmdDrawImage3->x, cmdDrawImage3->y);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_drawCanvas5(JCMemClass&  cmdBuffer)
    {
        CMDDrawImage5 * cmdDrawImage5 = cmdBuffer.popp<CMDDrawImage5>();
        m_pContext->drawCanvas(cmdDrawImage5->imgID, cmdDrawImage5->x, cmdDrawImage5->y,cmdDrawImage5->w,cmdDrawImage5->h);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_drawCanvas9(JCMemClass&  cmdBuffer)
    {
        CMDDrawImage9 * cmdDrawImage9 = cmdBuffer.popp<CMDDrawImage9>();
        m_pContext->drawCanvas(cmdDrawImage9->imgID, cmdDrawImage9->sx, cmdDrawImage9->sy, cmdDrawImage9->sw, cmdDrawImage9->sh,cmdDrawImage9->dx,cmdDrawImage9->dy,cmdDrawImage9->dw,cmdDrawImage9->dh);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_font(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        char* font = (char*)cmdBuffer.readBuffer(cmdInt->i);
        m_pContext->setFont(font);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_globalCompositeOperation(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->setGlobalCompositeOperation((BLEND_TYPE)cmdInt->i);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_textBaseline(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->setTextBaseline((TEXT_BASE_LINE_TYPE)cmdInt->i);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_textAlign(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->setTextAlign((TEXT_ALIGN_TYPE)cmdInt->i);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_miterLimit(JCMemClass&  cmdBuffer)
    {
        CMDFloat* cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pContext->setMiterLimit(cmdFloat->f);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_shadowOffsetX(JCMemClass&  cmdBuffer)
    {
        CMDFloat* cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pContext->setShadowOffsetX(cmdFloat->f);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_shadowOffsetY(JCMemClass&  cmdBuffer)
    {
        CMDFloat* cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pContext->setShadowOffsetY(cmdFloat->f);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_shadowColor(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->setShadowColor(cmdInt->i);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_shadowBlur(JCMemClass&  cmdBuffer)
    {
        CMDFloat* cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pContext->setShadowBlur(cmdFloat->f);
        return true;
    }
    
    bool JCHtml5Render::_rendercmd_fillStyle1(JCMemClass& cmdBuffer)
    {
        CMDPattern* cmdPattern = cmdBuffer.popp<CMDPattern>();
        m_pContext->setFillStyle(cmdPattern->imgid, (FILL_PATTERN_TYPE)cmdPattern->repetition, cmdPattern->x, cmdPattern->y, cmdPattern->w, cmdPattern->h);
        return true;
    }

    bool  JCHtml5Render::_rendercmd_setFilter(JCMemClass&  cmdBuffer)
    {
        CMDFloatRect* cmdRgbg= cmdBuffer.popp<CMDFloatRect>();
        m_pContext->setFilter(cmdRgbg->x, cmdRgbg->y, cmdRgbg->w, cmdRgbg->h);
        return true;
    }

    bool JCHtml5Render::_rendercmd_fillBorderText(JCMemClass& cmdBuffer)
    {
        CMDFillBorderText* cmdFillBorderText= cmdBuffer.popp<CMDFillBorderText>();
        char* sFont = cmdBuffer.readBuffer(cmdFillBorderText->nFontLen);
        int nTextlen = *(int*)cmdBuffer.readBuffer(4);
        char* sText = cmdBuffer.readBuffer(nTextlen);
        m_pContext->fillText(sText, sFont, cmdFillBorderText->x, cmdFillBorderText->y, cmdFillBorderText->nFillColor, (TEXT_ALIGN_TYPE)cmdFillBorderText->nTextAlign);
        return true;
    }

    bool JCHtml5Render::_rendercmd_clipRect(JCMemClass& cmdBuffer)
    {
        CMDFloatRect* cmdRect= cmdBuffer.popp<CMDFloatRect>();
        m_pContext->clipRect(cmdRect->x, cmdRect->y, cmdRect->w, cmdRect->y);
        return true;
    }

    bool JCHtml5Render::_rendercmd_setIBVB(JCMemClass& cmdBuffer)
    {
        CMDIBVB* cmdIBVB = cmdBuffer.popp<CMDIBVB>();
        m_pContext->setIBVB(cmdIBVB->ibId, cmdIBVB->vbId, cmdIBVB->vertType, cmdIBVB->shaderId, cmdIBVB->imgId, cmdIBVB->startIndex, 
            cmdIBVB->offset, cmdIBVB->numElement, cmdIBVB->x, cmdIBVB->y,cmdIBVB->mat);
        return true;
    }
    bool JCHtml5Render::_rendercmd_setFilterMatrix(JCMemClass& cmdBuffer)
    {
        CMDFilterMatrix* cmdFilter = cmdBuffer.popp<CMDFilterMatrix>();
        m_pContext->setFilterMatrix( cmdFilter->matrix,cmdFilter->alpha );
        return true;
    }

    bool JCHtml5Render::_rendercmd_setSkinMesh(JCMemClass& cmdBuffer)
    {
        CMDSkinMesh * cmdSkinMesh = cmdBuffer.popp<CMDSkinMesh>();
        m_pContext->setSkinMesh(cmdSkinMesh->ibId, cmdSkinMesh->vbId, cmdSkinMesh->vertType, cmdSkinMesh->imgId,cmdSkinMesh->shaderId,cmdSkinMesh->startIndex,cmdSkinMesh->numElement,cmdSkinMesh->mat, cmdSkinMesh->x, cmdSkinMesh->y);
        return true;
    }

    bool JCHtml5Render::_rendercmd_drawParticle2D(JCMemClass& cmdBuffer)
    {
        CMDDrawImage3 *cmdDrawParticle= cmdBuffer.popp<CMDDrawImage3>();
        m_pContext->drawParticle(cmdDrawParticle->imgID, cmdDrawParticle->x, cmdDrawParticle->y);
        //CMDSkinMesh *cmdSkinMesh= cmdBuffer.popp<CMDSkinMesh>();
        //m_pContext->drawParticle(cmdSkinMesh->ibId, cmdSkinMesh->vbId, cmdSkinMesh->vertType, cmdSkinMesh->imgId, cmdSkinMesh->shaderId, cmdSkinMesh->startIndex, cmdSkinMesh->numElement, cmdSkinMesh->mat, cmdSkinMesh->x, cmdSkinMesh->y);
        return true;
    }

    bool JCHtml5Render::_rendercmd_getPixels(JCMemClass& cmdBuffer)
    {
        //主canvas  最简单的方式  直接就可以获取像素
        CMDFloat4 *cmdGetPixels = cmdBuffer.popp<CMDFloat4>();
        getPixels(cmdGetPixels->x, cmdGetPixels->y, cmdGetPixels->z, cmdGetPixels->w);
        return true;
    }
    bool JCHtml5Render::_rendercmd_toBase64(JCMemClass& cmdBuffer)
    {
        CMDToBase64* cmdToBase64 = cmdBuffer.popp<CMDToBase64>();
        char* type = (char*)cmdBuffer.readBuffer(cmdToBase64->length);
        toBase64(type, cmdToBase64->encoderOptions);
        return true;
    }
    bool  JCHtml5Render::_rendercmd_isPointInPath(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_rendercmd_measureText(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_rendercmd_getImageData(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_rendercmd_disableClip(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_rendercmd_setCanvas(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_rendercmd_createPattern(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_rendercmd_createLinearGradient(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_rendercmd_createRadialGradient(JCMemClass&  cmdBuffer)
    {
        return true;
    }



    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------

    bool  JCHtml5Render::_save_rendercmd_save(JCMemClass&  cmdBuffer)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        m_pContext->m_pSaveCmd->append(*cmdnoParam);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_restore(JCMemClass&  cmdBuffer)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        m_pContext->m_pSaveCmd->append(*cmdnoParam);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_beginPath(JCMemClass&  cmdBuffer)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        m_pContext->m_pSaveCmd->append(*cmdnoParam);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_drawImage3(JCMemClass&  cmdBuffer)
    {
        CMDDrawImage3 * cmdDrawImage3 = cmdBuffer.popp<CMDDrawImage3>();
        m_pContext->m_pSaveCmd->append(*cmdDrawImage3);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_drawImage5(JCMemClass&  cmdBuffer)
    {
        CMDDrawImage5* cmdDrawImage5 = cmdBuffer.popp<CMDDrawImage5>();
        m_pContext->m_pSaveCmd->append(*cmdDrawImage5);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_drawImage9(JCMemClass&  cmdBuffer)
    {
        CMDDrawImage9* cmdDrawImage9 = cmdBuffer.popp<CMDDrawImage9>();
        m_pContext->m_pSaveCmd->append(*cmdDrawImage9);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_rect(JCMemClass&  cmdBuffer)
    {
        CMDFloatRect * rect = cmdBuffer.popp<CMDFloatRect>();
        m_pContext->m_pSaveCmd->append(*rect);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_clip(JCMemClass&  cmdBuffer)
    {
        CMDNoParam* cmdnoParam = cmdBuffer.popp<CMDNoParam>();
        m_pContext->m_pSaveCmd->append(*cmdnoParam);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_scale(JCMemClass&  cmdBuffer)
    {
        CMDFloatPoint * scale = cmdBuffer.popp<CMDFloatPoint>();
        m_pContext->m_pSaveCmd->append(*scale);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_rotate(JCMemClass&  cmdBuffer)
    {
        CMDFloat * cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pContext->m_pSaveCmd->append(*cmdFloat);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_translate(JCMemClass&  cmdBuffer)
    {
        CMDFloatPoint * pos = cmdBuffer.popp<CMDFloatPoint>();
        m_pContext->m_pSaveCmd->append(*pos);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_transform(JCMemClass&  cmdBuffer)
    {
        CMDMatrix *matrix = cmdBuffer.popp<CMDMatrix>();
        m_pContext->m_pSaveCmd->append(*matrix);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_setTransform(JCMemClass&  cmdBuffer)
    {
        CMDMatrix *matrix = cmdBuffer.popp<CMDMatrix>();
        m_pContext->m_pSaveCmd->append(*matrix);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_globalAlpha(JCMemClass&  cmdBuffer)
    {
        CMDFloat * cf = cmdBuffer.popp<CMDFloat>();
        m_pContext->m_pSaveCmd->append(*cf);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_clearRect(JCMemClass&  cmdBuffer)
    {
        CMDFloatRect * rect = cmdBuffer.popp<CMDFloatRect>();
        m_pContext->m_pSaveCmd->append(*rect);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_closePath(JCMemClass&  cmdBuffer)
    {
        CMDNoParam* noparam = cmdBuffer.popp<CMDNoParam>();
        m_pContext->m_pSaveCmd->append(*noparam);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_fillStyle(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->m_pSaveCmd->append(*cmdInt);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_fillRect(JCMemClass&  cmdBuffer)
    {
        CMDFloatRect * rect = cmdBuffer.popp<CMDFloatRect>();
        m_pContext->m_pSaveCmd->append(*rect);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_strokeStyle(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->m_pSaveCmd->append(*cmdInt);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_stroke(JCMemClass&  cmdBuffer)
    {
        CMDNoParam* pParam = cmdBuffer.popp<CMDNoParam>();
        m_pContext->m_pSaveCmd->append(*pParam);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_strokeRect(JCMemClass&  cmdBuffer)
    {
        CMDFloatRect * rect = cmdBuffer.popp<CMDFloatRect>();
        m_pContext->m_pSaveCmd->append(*rect);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_strokeText(JCMemClass&  cmdBuffer)
    {
        CMDFillText * cmdStrokeText = cmdBuffer.popp<CMDFillText>();
        char * text = cmdBuffer.readBuffer(cmdStrokeText->len);
        //TODO 测试
        m_pContext->m_pSaveCmd->append(*cmdStrokeText);
        m_pContext->m_pSaveCmd->append(text,cmdStrokeText->len);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_strokeText2(JCMemClass&  cmdBuffer)
    {
        CMDFillText1 * cmdStrokeText = cmdBuffer.popp<CMDFillText1>();
        char * text = (char*)cmdBuffer.readBuffer((int)cmdStrokeText->len);
        //TODO 测试
        m_pContext->m_pSaveCmd->append(*cmdStrokeText);
        m_pContext->m_pSaveCmd->append(text, cmdStrokeText->len);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_fillText(JCMemClass&  cmdBuffer)
    {
        CMDFillText * cmdFillText = cmdBuffer.popp<CMDFillText>();
        char * text = cmdBuffer.readBuffer(cmdFillText->len);
        //TODO 测试
        m_pContext->m_pSaveCmd->append(*cmdFillText);
        m_pContext->m_pSaveCmd->append(text, cmdFillText->len);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_fillText1(JCMemClass&  cmdBuffer)
    {
        CMDFillText1 * cmdFillText = cmdBuffer.popp<CMDFillText1>();
        char * text = (char*)cmdBuffer.readBuffer((int)cmdFillText->len);
        //TODO 测试
        m_pContext->m_pSaveCmd->append(*cmdFillText);
        m_pContext->m_pSaveCmd->append(text, cmdFillText->len);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_fill(JCMemClass&  cmdBuffer)
    {
        CMDNoParam* pParam = cmdBuffer.popp<CMDNoParam>();
        m_pContext->m_pSaveCmd->append(*pParam);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_lineCap(JCMemClass&  cmdBuffer)
    {
        CMDInt * cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->m_pSaveCmd->append(*cmdInt);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_lineWidth(JCMemClass&  cmdBuffer)
    {
        CMDFloat *cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pContext->m_pSaveCmd->append(*cmdFloat);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_lineJoin(JCMemClass&  cmdBuffer)
    {
        CMDInt * cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->m_pSaveCmd->append(*cmdInt);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_moveTo(JCMemClass&  cmdBuffer)
    {
        CMDFloatPoint * mt = cmdBuffer.popp<CMDFloatPoint>();
        m_pContext->m_pSaveCmd->append(*mt);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_lineTo(JCMemClass&  cmdBuffer)
    {
        CMDFloatPoint * lt = cmdBuffer.popp<CMDFloatPoint>();
        m_pContext->m_pSaveCmd->append(*lt);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_arc(JCMemClass&  cmdBuffer)
    {
        CMDArc *cmdArc = cmdBuffer.popp<CMDArc>();
        m_pContext->m_pSaveCmd->append(*cmdArc);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_arcTo(JCMemClass&  cmdBuffer)
    {
        CMDArcTo *cmdArcTo = cmdBuffer.popp<CMDArcTo>();
        m_pContext->m_pSaveCmd->append(*cmdArcTo);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_bezierCurveTo(JCMemClass&  cmdBuffer)
    {
        CMDMatrix *martrix = cmdBuffer.popp<CMDMatrix>();
        m_pContext->m_pSaveCmd->append(*martrix);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_quadraticCurveTo(JCMemClass&  cmdBuffer)
    {
        CMDFloatRect *rect = cmdBuffer.popp<CMDFloatRect>();
        m_pContext->m_pSaveCmd->append(*rect);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_drawCanvas3(JCMemClass&  cmdBuffer)
    {
        CMDDrawImage3 * cmdDrawImage3 = cmdBuffer.popp<CMDDrawImage3>();
        m_pContext->m_pSaveCmd->append(*cmdDrawImage3);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_drawCanvas5(JCMemClass&  cmdBuffer)
    {
        CMDDrawImage5 * cmdDrawImage5 = cmdBuffer.popp<CMDDrawImage5>();
        m_pContext->m_pSaveCmd->append(*cmdDrawImage5);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_drawCanvas9(JCMemClass&  cmdBuffer)
    {
        CMDDrawImage9 * cmdDrawImage9 = cmdBuffer.popp<CMDDrawImage9>();
        m_pContext->m_pSaveCmd->append(*cmdDrawImage9);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_font(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        char* font = (char*)cmdBuffer.readBuffer(cmdInt->i);
        m_pContext->m_pSaveCmd->append(*cmdInt);
        m_pContext->m_pSaveCmd->append(font,cmdInt->i);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_globalCompositeOperation(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->m_pSaveCmd->append(*cmdInt);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_textBaseline(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->m_pSaveCmd->append(*cmdInt);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_textAlign(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->m_pSaveCmd->append(*cmdInt);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_miterLimit(JCMemClass&  cmdBuffer)
    {
        CMDFloat* cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pContext->m_pSaveCmd->append(*cmdFloat);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_shadowOffsetX(JCMemClass&  cmdBuffer)
    {
        CMDFloat* cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pContext->m_pSaveCmd->append(*cmdFloat);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_shadowOffsetY(JCMemClass&  cmdBuffer)
    {
        CMDFloat* cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pContext->m_pSaveCmd->append(*cmdFloat);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_shadowColor(JCMemClass&  cmdBuffer)
    {
        CMDInt* cmdInt = cmdBuffer.popp<CMDInt>();
        m_pContext->m_pSaveCmd->append(*cmdInt);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_shadowBlur(JCMemClass&  cmdBuffer)
    {
        CMDFloat* cmdFloat = cmdBuffer.popp<CMDFloat>();
        m_pContext->m_pSaveCmd->append(*cmdFloat);
        return true;
    }
    bool JCHtml5Render::_save_rendercmd_fillStyle1(JCMemClass& cmdBuffer)
    {
        CMDPattern* cmdPattern = cmdBuffer.popp<CMDPattern>();
        m_pContext->m_pSaveCmd->append(*cmdPattern);
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_setFilter(JCMemClass&  cmdBuffer)
    {
        CMDFloatRect* cmdRgbg = cmdBuffer.popp<CMDFloatRect>();
        m_pContext->m_pSaveCmd->append(*cmdRgbg);
        return true;
    }

    bool JCHtml5Render::_save_rendercmd_fillBorderText(JCMemClass& cmdBuffer)
    {
        CMDFillBorderText* cmdFillBorderText = cmdBuffer.popp<CMDFillBorderText>();
        m_pContext->m_pSaveCmd->append(*cmdFillBorderText);
        char* sFont = cmdBuffer.readBuffer(cmdFillBorderText->nFontLen);
        m_pContext->m_pSaveCmd->append(sFont, cmdFillBorderText->nFontLen);
        int nTextlen = *(int*)cmdBuffer.readBuffer(4);
        m_pContext->m_pSaveCmd->append(nTextlen);
        char* sText = cmdBuffer.readBuffer(nTextlen);
        m_pContext->m_pSaveCmd->append(sText, nTextlen);
        return true;
    }

    bool JCHtml5Render::_save_rendercmd_clipRect(JCMemClass& cmdBuffer)
    {
        CMDFloatRect* cmdRect = cmdBuffer.popp<CMDFloatRect>();
        m_pContext->m_pSaveCmd->append(*cmdRect);
        return true;
    }

    bool JCHtml5Render::_save_rendercmd_setIBVB(JCMemClass& cmdBuffer)
    {
        CMDIBVB* cmdIBVB = cmdBuffer.popp<CMDIBVB>();
        m_pContext->m_pSaveCmd->append(*cmdIBVB);
        return true;
    }

    bool JCHtml5Render::_save_rendercmd_setFilterMatrix(JCMemClass& cmdBuffer)
    {
        CMDFilterMatrix* cmdFilter = cmdBuffer.popp<CMDFilterMatrix>();
        m_pContext->m_pSaveCmd->append(*cmdFilter);
        return true;
    }

    bool JCHtml5Render::_save_rendercmd_setSkinMesh(JCMemClass& cmdBuffer)
    {
        CMDSkinMesh *cmdSkinMesh = cmdBuffer.popp<CMDSkinMesh>();
        m_pContext->m_pSaveCmd->append(*cmdSkinMesh);
        return true;
    }

    bool JCHtml5Render::_save_rendercmd_drawParticle2D(JCMemClass& cmdBuffer)
    {
        //CMDSkinMesh *cmdSkinMesh = cmdBuffer.popp<CMDSkinMesh>();
        CMDDrawImage3 *cmdDrawParticle = cmdBuffer.popp<CMDDrawImage3>();
       // m_pContext->m_pSaveCmd->append(*cmdSkinMesh);
        m_pContext->m_pSaveCmd->append(cmdDrawParticle);
        return true;
    }

    bool JCHtml5Render::_save_rendercmd_getPixels(JCMemClass& cmdBuffer)
    {
        CMDFloat4*  cmdGetPixels = cmdBuffer.popp<CMDFloat4>();
        //子canvs  
        getPixels(cmdGetPixels->x, cmdGetPixels->y, cmdGetPixels->z, cmdGetPixels->w);
        m_pContext->flushToRenderTarget();
        return true;
    }
    bool JCHtml5Render::_save_rendercmd_toBase64(JCMemClass& cmdBuffer)
    {
        CMDToBase64* cmdToBase64 = cmdBuffer.popp<CMDToBase64>();
        char* type = (char*)cmdBuffer.readBuffer(cmdToBase64->length);
        //子canvs  
        toBase64(type, cmdToBase64->encoderOptions);
        m_pContext->flushToRenderTarget();
        return true;
    }
    void JCHtml5Render::getPixels(int x, int y, int w, int h)
    {
        if (x < 0 || y < 0 || w <= 0 || h <= 0)
            return;
        m_pContext->getPixels(x, y, w, h, [this](unsigned char *pPixels, int size, int w, int h)
        { 
            JCConchBridge::getPixelsRenderToJS(pPixels, size, m_nID, w, h);
        });
    }
    void JCHtml5Render::toBase64(const char* type, float encoderOptions)
    {
        m_pContext->toBase64(type, encoderOptions, [this](const char* base64)
        {
            JCConchBridge::toBaseRenderToJS(base64, m_nID);
        });
    }
    bool  JCHtml5Render::_save_rendercmd_isPointInPath(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_measureText(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_getImageData(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_disableClip(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_setCanvas(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_createPattern(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_createLinearGradient(JCMemClass&  cmdBuffer)
    {
        return true;
    }
    bool  JCHtml5Render::_save_rendercmd_createRadialGradient(JCMemClass&  cmdBuffer)
    {
        return true;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
