/**
@file			JCGraphicsCmdDispath.h
@brief			
@author			James
@version		1.0
@date			2016_6_14
*/

#ifndef __JCGraphicsCmdDispath_H__
#define __JCGraphicsCmdDispath_H__

#include <JCICmdDispatch.h>
#include "JCGraphics.h"
#include <buffer/JCBuffer.h>

namespace laya
{
    class JCResManager;
    class JCImageManager;
    class JC2DShaderManager;
    class JCHtml5RenderManager;
    class JCTextManager;
    class JCFontManager;
    class JCBufferManager;
    class JCShaderManager;
    /** 
     * @brief 
    */
    class JCGraphicsCmdDispath : public JCICmdDispatch
    {
    public:

        typedef void (JCGraphicsCmdDispath::*ProcFunction)(JCMemClass& cmdBuffer);

        typedef void (JCGraphicsCmdDispath::*CalcBoundingBoxFunction)(JCMemClass& cmdBuffer,std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);

    public:

        JCGraphicsCmdDispath(int nID,JCResManager* pTextureManager, JCImageManager* pImageManager, JC2DShaderManager* p2DShaderManager,
            JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager,
            JCBufferManager* pBufferManager, JCShaderManager* pShaderManager, TEXT_BASE_LINE_TYPE nBaseLineType);

        ~JCGraphicsCmdDispath();

        bool restartJSThread();

        bool dispatchRenderCmd(JCMemClass& pRenderCmd, int nFrameCount);

        bool dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount);

        bool runRenderCmd(JCMemClass& pRenderCmd);

        bool saveRenderCmd(JCMemClass& pRenderCmd);

        bool calcBoundingBox(JCMemClass& pRenderCmd,std::vector<Matrix32>& vMatrixs,Matrix32& kCurrentMatrix,std::vector<float>& vResultPointArray );

        static void addPointResult(std::vector<float>& pResultArrays, float x, float y, Matrix32* pMatrix);

        static void addPointResult(std::vector<float>& pResultArrays, float x, float y, float w, float h, Matrix32* pMatrix);

        static void addPointResult(std::vector<float>& pResultArrays, float x, float y, float radius, float startAngle, float endAngle, Matrix32* pMatrix);

    public:

        void _rendercmd_clear(JCMemClass& cmdBuffer);
        void _rendercmd_destory(JCMemClass& cmdBuffer);
        void _rendercmd_alpha(JCMemClass& cmdBuffer);
        void _rendercmd_rotate(JCMemClass& cmdBuffer);
        void _rendercmd_transform(JCMemClass& cmdBuffer);
        void _rendercmd_scale(JCMemClass& cmdBuffer);
        void _rendercmd_translate(JCMemClass& cmdBuffer);
        void _rendercmd_save(JCMemClass& cmdBuffer);
        void _rendercmd_restore(JCMemClass& cmdBuffer);
        void _rendercmd_clipRect(JCMemClass& cmdBuffer);
        void _rendercmd_blendMode(JCMemClass& cmdBuffer);
        void _rendercmd_fillText(JCMemClass& cmdBuffer);
        void _rendercmd_fillBorderText(JCMemClass& cmdBuffer);
        void _rendercmd_drawRect(JCMemClass& cmdBuffer);
        void _rendercmd_drawCircle(JCMemClass& cmdBuffer);
        void _rendercmd_drawPie(JCMemClass& cmdBuffer);
        void _rendercmd_drawImage(JCMemClass& cmdBuffer);
        void _rendercmd_drawImageM(JCMemClass& cmdBuffer);
        void _rendercmd_drawPoly(JCMemClass& cmdBuffer);
        void _rendercmd_drawPath(JCMemClass& cmdBuffer);
        void _rendercmd_drawLine(JCMemClass& cmdBuffer);
        void _rendercmd_drawLines(JCMemClass& cmdBuffer);
        void _renderCmd_drawCurves(JCMemClass& cmdBuffer);
        void _renderCmd_replaceTextColor(JCMemClass& cmdBuffer);
        void _renderCmd_fillImage(JCMemClass& cmdBuffer);
        void _renderCmd_setSkinMesh(JCMemClass& cmdBuffer);
        void _rendercmd_drawParticle(JCMemClass& cmdBuffer);
		void _rendercmd_drawImageS(JCMemClass& cmdBuffer);
        void _rendercmd_replaceText(JCMemClass& cmdBuffer);

        //------------------------------------------------------------------------------
        void _savecmd_clear(JCMemClass& cmdBuffer);
        void _savecmd_destory(JCMemClass& cmdBuffer);
        void _savecmd_alpha(JCMemClass& cmdBuffer);
        void _savecmd_rotate(JCMemClass& cmdBuffer);
        void _savecmd_transform(JCMemClass& cmdBuffer);
        void _savecmd_scale(JCMemClass& cmdBuffer);
        void _savecmd_translate(JCMemClass& cmdBuffer);
        void _savecmd_save(JCMemClass& cmdBuffer);
        void _savecmd_restore(JCMemClass& cmdBuffer);
        void _savecmd_clipRect(JCMemClass& cmdBuffer);
        void _savecmd_blendMode(JCMemClass& cmdBuffer);
        void _savecmd_fillText(JCMemClass& cmdBuffer);
        void _savecmd_fillBorderText(JCMemClass& cmdBuffer);
        void _savecmd_drawRect(JCMemClass& cmdBuffer);
        void _savecmd_drawCircle(JCMemClass& cmdBuffer);
        void _savecmd_drawPie(JCMemClass& cmdBuffer);
        void _savecmd_drawImage(JCMemClass& cmdBuffer);
        void _savecmd_drawImageM(JCMemClass& cmdBuffer);
        void _savecmd_drawPoly(JCMemClass& cmdBuffer);
        void _savecmd_drawPath(JCMemClass& cmdBuffer);
        void _savecmd_drawLine(JCMemClass& cmdBuffer);
        void _savecmd_drawLines(JCMemClass& cmdBuffer);
        void _savecmd_drawCurves(JCMemClass& cmdBuffer);
        void _savecmd_replaceTextColor(JCMemClass& cmdBuffer);
        void _savecmd_fillImage(JCMemClass& cmdBuffer);
        void _savecmd_setSkinMesh(JCMemClass& cmdBuffer);
        void _savecmd_drawParticle(JCMemClass& cmdBuffer);
		void _savecmd_drawImageS(JCMemClass& cmdBuffer);
        void _savecmd_replaceText(JCMemClass& cmdBuffer);

        //------------------------------------------------------------------------------
        void _calcBoundingBox_clear(JCMemClass& cmdBuffer,std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_destory(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_alpha(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_rotate(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_transform(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_scale(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_translate(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_save(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_restore(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_clipRect(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_blendMode(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_fillText(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_fillBorderText(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_drawRect(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_drawCircle(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_drawPie(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_drawImage(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_drawImageM(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_drawPoly(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_drawPath(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_drawLine(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_drawLines(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_drawCurves(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_replaceTextColor(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_fillImage(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_setSkinMesh(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_drawParticle(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
		void _calcBoundingBox_drawImageS(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
        void _calcBoundingBox_replaceText(JCMemClass& cmdBuffer, std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix, std::vector<float>& vResultPointArray);
    public:

        JCGraphics*             m_pGraphics;
    };
}

#endif //__JCGraphicsCmdDispath_H__

//-----------------------------END FILE--------------------------------
