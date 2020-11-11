/**
@file			JCHtml5Render.h
@brief			
@author			James
@version		1.0
@date			2016_5_27
*/

#ifndef __JCHtml5Render_H__
#define __JCHtml5Render_H__

#include <stdlib.h>
#include <buffer/JCBuffer.h>
#include <util/JCCommonMethod.h>
#include <util/Log.h>
#include "JCHtml5Context.h"
#include "../Text/JCTextMemoryCanvas.h"
#include <buffer/JCBuffer.h>
#include <JCICmdDispatch.h>
#include "../Image/JCImageManager.h"
#include "../Manager/JCBufferManager.h"
#include "../Manager/JCShaderManager.h"
#include <3DRS/JCMaterial.h>
#include <3DRS/JCMaterialManager.h>
#include <geom/RectGeometry.h>
#include <JCIGLRender.h>
#include <3DRS/JC2DShaderManager.h>


namespace laya
{
    #define MEM_RENDER_COMMAND_SIZE 256000
 
    class JCHtml5RenderManager;
    /** 
     * @brief Html5Render ¼Ì³ÐDispath
    */
    class JCHtml5Render : public JCICmdDispatch , public JCIGLRender
    {
    public:
        JCHtml5Render(int nID,JCResManager* pTextureManager,JCImageManager* pImageManager,JC2DShaderManager* p2DShaderManager,JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager,JCBufferManager* pBufferManager,JCShaderManager* pShaderManager, std::vector<JCICmdDispatch*>* vVectorCmdDispat);
        ~JCHtml5Render();
        typedef bool (JCHtml5Render::*ProcFunction)(JCMemClass& cmdBuffer);

        void onGLReady();
        void renderFrame(long nCurrentFrame, bool bStopRender);
        void invalidGLRes();
        bool dispatchRenderCmd(JCMemClass& pRenderCmd,int nFrameCount);
        bool dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount);
        bool restartJSThread();
        void setCanvasType( CANVAS_TYPE nType );
        void setCanvasSize( int w,int h);
        void setMemoryCanvasClip( bool bClip );

        bool runRenderCmd(JCMemClass& pRenderCmd);
        bool saveRenderCmd(JCMemClass& pRenderCmd);

    private:

        bool _rendercmd_clearRect(JCMemClass&  cmdBuffer);
        bool _rendercmd_fillRect(JCMemClass&  cmdBuffer);
        bool _rendercmd_translate(JCMemClass&  cmdBuffer);//50
        bool _rendercmd_save(JCMemClass&  cmdBuffer);
        bool _rendercmd_restore(JCMemClass&  cmdBuffer);
        bool _rendercmd_beginPath(JCMemClass&  cmdBuffer);
        bool _rendercmd_clip(JCMemClass&  cmdBuffer);
        bool _rendercmd_rect(JCMemClass&  cmdBuffer);
        bool _rendercmd_strokeRect(JCMemClass&  cmdBuffer);
        bool _rendercmd_fill(JCMemClass&  cmdBuffer);
        bool _rendercmd_stroke(JCMemClass&  cmdBuffer);
        bool _rendercmd_moveTo(JCMemClass&  cmdBuffer);
        bool _rendercmd_closePath(JCMemClass&  cmdBuffer);//60
        bool _rendercmd_lineTo(JCMemClass&  cmdBuffer);
        bool _rendercmd_quadraticCurveTo(JCMemClass&  cmdBuffer);
        bool _rendercmd_bezierCurveTo(JCMemClass&  cmdBuffer);
        bool _rendercmd_arc(JCMemClass&  cmdBuffer);
        bool _rendercmd_arcTo(JCMemClass&  cmdBuffer);
        bool _rendercmd_scale(JCMemClass&  cmdBuffer);
        bool _rendercmd_rotate(JCMemClass&  cmdBuffer);
        bool _rendercmd_transform(JCMemClass&  cmdBuffer);
        bool _rendercmd_setTransform(JCMemClass&  cmdBuffer);
        bool _rendercmd_fillText(JCMemClass&  cmdBuffer);//70
        bool _rendercmd_fillText1(JCMemClass&  cmdBuffer);
        bool _rendercmd_strokeText(JCMemClass&  cmdBuffer);
        bool _rendercmd_strokeText2(JCMemClass&  cmdBuffer);
        bool _rendercmd_drawImage3(JCMemClass&  cmdBuffer);
        bool _rendercmd_drawImage5(JCMemClass&  cmdBuffer);
        bool _rendercmd_drawImage9(JCMemClass&  cmdBuffer);
        bool _rendercmd_setFilter(JCMemClass&  cmdBuffer);
        bool _rendercmd_isPointInPath(JCMemClass&  cmdBuffer);
        bool _rendercmd_measureText(JCMemClass&  cmdBuffer);
        bool _rendercmd_getImageData(JCMemClass&  cmdBuffer);//80
        bool _rendercmd_disableClip(JCMemClass&  cmdBuffer);
        bool _rendercmd_setCanvas(JCMemClass&  cmdBuffer);
        bool _rendercmd_createPattern(JCMemClass&  cmdBuffer);
        bool _rendercmd_createLinearGradient(JCMemClass&  cmdBuffer);
        bool _rendercmd_createRadialGradient(JCMemClass&  cmdBuffer);
        bool _rendercmd_globalAlpha(JCMemClass&  cmdBuffer);
        bool _rendercmd_shadowColor(JCMemClass&  cmdBuffer);
        bool _rendercmd_shadowBlur(JCMemClass&  cmdBuffer);
        bool _rendercmd_shadowOffsetX(JCMemClass&  cmdBuffer);
        bool _rendercmd_shadowOffsetY(JCMemClass&  cmdBuffer);//90
        bool _rendercmd_lineCap(JCMemClass&  cmdBuffer);
        bool _rendercmd_lineJoin(JCMemClass&  cmdBuffer);
        bool _rendercmd_lineWidth(JCMemClass&  cmdBuffer);
        bool _rendercmd_miterLimit(JCMemClass&  cmdBuffer);
        bool _rendercmd_font(JCMemClass&  cmdBuffer);
        bool _rendercmd_textAlign(JCMemClass&  cmdBuffer);
        bool _rendercmd_textBaseline(JCMemClass&  cmdBuffer);
        bool _rendercmd_globalCompositeOperation(JCMemClass&  cmdBuffer);
        bool _rendercmd_fillStyle(JCMemClass&  cmdBuffer);
        bool _rendercmd_strokeStyle(JCMemClass&  cmdBuffer);//100
        bool _rendercmd_drawCanvas3(JCMemClass&  cmdBuffer);
        bool _rendercmd_drawCanvas5(JCMemClass&  cmdBuffer);
        bool _rendercmd_drawCanvas9(JCMemClass&  cmdBuffer);
        bool _rendercmd_fillStyle1(JCMemClass& cmdBuffer);
        bool _rendercmd_fillBorderText(JCMemClass&cmdBuffer);
        bool _rendercmd_clipRect(JCMemClass&cmdBuffer);
        bool _rendercmd_setIBVB(JCMemClass&cmdBuffer);
        bool _rendercmd_setFilterMatrix(JCMemClass&cmdBuffer);
        bool _rendercmd_setSkinMesh(JCMemClass&cmdBuffer);
        bool _rendercmd_drawParticle2D(JCMemClass&cmdBuffer);
        bool _rendercmd_getPixels(JCMemClass&cmdBuffer);
        bool _rendercmd_toBase64(JCMemClass&cmdBuffer);

        bool _save_rendercmd_clearRect(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_fillRect(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_translate(JCMemClass&  cmdBuffer);//50
        bool _save_rendercmd_save(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_restore(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_beginPath(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_clip(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_rect(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_strokeRect(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_fill(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_stroke(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_moveTo(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_closePath(JCMemClass&  cmdBuffer);//60
        bool _save_rendercmd_lineTo(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_quadraticCurveTo(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_bezierCurveTo(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_arc(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_arcTo(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_scale(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_rotate(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_transform(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_setTransform(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_fillText(JCMemClass&  cmdBuffer);//70
        bool _save_rendercmd_fillText1(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_strokeText(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_strokeText2(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_drawImage3(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_drawImage5(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_drawImage9(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_setFilter(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_isPointInPath(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_measureText(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_getImageData(JCMemClass&  cmdBuffer);//80
        bool _save_rendercmd_disableClip(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_setCanvas(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_createPattern(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_createLinearGradient(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_createRadialGradient(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_globalAlpha(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_shadowColor(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_shadowBlur(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_shadowOffsetX(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_shadowOffsetY(JCMemClass&  cmdBuffer);//90
        bool _save_rendercmd_lineCap(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_lineJoin(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_lineWidth(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_miterLimit(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_font(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_textAlign(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_textBaseline(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_globalCompositeOperation(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_fillStyle(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_strokeStyle(JCMemClass&  cmdBuffer);//100
        bool _save_rendercmd_drawCanvas3(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_drawCanvas5(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_drawCanvas9(JCMemClass&  cmdBuffer);
        bool _save_rendercmd_fillStyle1(JCMemClass& cmdBuffer);
        bool _save_rendercmd_fillBorderText(JCMemClass&cmdBuffer);
        bool _save_rendercmd_clipRect(JCMemClass&cmdBuffer);
        bool _save_rendercmd_setIBVB(JCMemClass&cmdBuffer);
        bool _save_rendercmd_setFilterMatrix(JCMemClass&cmdBuffer);
        bool _save_rendercmd_setSkinMesh(JCMemClass&cmdBuffer);
        bool _save_rendercmd_drawParticle2D(JCMemClass&cmdBuffer);
        bool _save_rendercmd_getPixels(JCMemClass&cmdBuffer);
        bool _save_rendercmd_toBase64(JCMemClass&cmdBuffer);
    private:
        void getPixels(int x, int y, int w, int h);
        void toBase64(const char* type, float encoderOptions);
    public:
        int                     m_nID;              ///<CmdDispathManager·ÖÅäµÄID
        JCHtml5Context*         m_pContext;         ///<context
        int                     m_nNoteFrame;       //¼ÇÂ¼frame  
    };
}
//------------------------------------------------------------------------------


#endif //__JCHtml5Render_H__

//-----------------------------END FILE--------------------------------