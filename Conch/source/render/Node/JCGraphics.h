/**
@file			JCGraphics.h
@brief			
@author			James
@version		1.0
@date			2016_6_13
*/

#ifndef __JCGraphics_H__
#define __JCGraphics_H__

#include <buffer/JCBuffer.h>
#include <vector>
#include <geom/Point.h>
#include <geom/Matrix32.h>
#include "JCGraphicsWordInfo.h"
#include "../SaveData/JCContextSaveData.h"

namespace laya
{
    /** 
     * @brief 
    */
    class JCNode2D;
    class JCGraphicsCmdDispath;
    class JCResManager;
    class JCImageManager;
    class JC2DShaderManager;
    class JCHtml5RenderManager;
    class JCTextManager;
    class JCFontManager;
    class JCBufferManager;
    class JCShaderManager;
    class JCHtml5Context;
    class JCGraphics
    {
    public:

        typedef std::vector<JCGraphicsWordInfo*> VectorWordInfos;

    public:
        enum DrawPathTYPE
        {
            TYPE_MOVETO=0,
            TYPE_LINETO=1,
            TYPE_ARCTO=2,
            TYPE_CLOSEPATH=3
        };

        JCGraphics(int nID,JCResManager* pTextureManager, JCImageManager* pImageManager, JC2DShaderManager* p2DShaderManager,
            JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager,
            JCBufferManager* pBufferManager, JCShaderManager* pShaderManager, TEXT_BASE_LINE_TYPE nBaseLineType);

        ~JCGraphics();

        void renderGraphics( JCHtml5Context* pContext,float x,float y);

        void setGraphicsDispath(JCGraphicsCmdDispath* pGraphicsDispath);

        void clear();
        
        void destory();

        void alpha( float alpha );

        void rotate(float r, float pivotX, float pivotY);

        void transform(float m11, float m12, float m21, float m22, float m31, float m32);

        void scale(float x, float y, float pivotX, float pivotY);

        void translate(float x, float y);

        void save();

        void restore();

        void clipRect(float x, float y, float w, float h);

        void blendMode(int nBlendMode);

        //这两个函数不建议用，效率太低了,现在都用下面的两个函数
        void fillText(const char* sText, float x,float y, const char* sFont, int nColor, int nTextAlign );
        void fillBorderText(const char* sText, float x, float y, const char* sFont, int nColor, int nBorderColor, float nLineWidth, int nTextAlign);

        void fillText(float x, float y, int nTextAlign);

        void fillBorderText(float x, float y, float nLineWidth, int nTextAlign);

        void drawRect(float x, float y, float width, float height, int nFillColor, int nLineColor, float nLineWidth,int nFlag);

        void drawCircle(float x, float y, float radius, int nFillColor, int nLineColor, float nLineWidth,int nFlag);

        void drawPie(float x, float y, float radius, float startAngle, float endAngle, int nFillColor, int nLineColor, float nLineWidth,int nFlag);

        void drawImage(int nImageID, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh,float alpha,int nType);

		void drawImageS(int nImageID, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh,int nType,int len,char *posBuffer);

        void drawImageM(int nImageID, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh,
            float m11, float m12, float m21, float m22, float m31, float m32,float alpha,int nType);
        
        /** @brief 大部分Graphics都只有一张图，这样其实可以快速的处理，这个函数是保存drawImage 参数信息用的
         *  @param[in] imageID
         *  @param[in] image的args参数
        */
        void setDrawImageFlag(int nImageID, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh,float alpha,int nType);

        /** @brief 其他函数都会调用这个，复杂指令，就会去掉DrawImage 的imageID
        */
        void removeDrawImageFlag();

        void drawPoly(float x, float y, int nFillColor, int nLineColor, float nLineWidth, char* pPointsBuffer, int nPointNumber,int flag);

        void drawPath(float x, float y, int nBrushColor, char* pPointsBuffer, int nBufferLength,bool bFlag);

        void drawPath(float x, float y, int nBrushColor, int nStrokeStyle, int nLineJoin,
            int nLineCap, int nMiterLimit, float nLineWidth, char* pPointsBuffer, int nBufferLength,bool bFlag);

        void drawLine(float x1, float y1, float x2, float y2, int lineColor, float lineWidth);

        void drawLine(float x,float y,int lineColor, float lineWidth, char *pPointsBuffer, int nPointNumber);

        void drawCurves(float x, float y, int lineColor, float lineWidth, char *pPointsBuffer, int nPointNumber);

        void replaceTextColor(int nColor);

        void replaceText(const char* sText);

        void fillImage(int imageId, int repeatType,float* args);

        void setSkinMesh(int p_nIbId, int p_nVbId, int p_nVertType, int p_nImgId, int p_nShaderId, int p_nStartIndex, int p_nNumElement, float *mat);

        void drawParticle(int p_nParticleTemplatid);

    public:

        void getCmdPoints(std::vector<float>& pResultArrays,std::vector<Matrix32>& vMatrixs, Matrix32& kCurrentMatrix);

        void setTextInfo(char* sText, const char* sFont, int nColor);

        void notifyDetachFrom(JCNode2D* pNode);

        void notifyAttachTo(JCNode2D* pNode);

        void repaint();
    public:

        JCHtml5Context*         m_pRenderContext;
        int                     m_nID;                  ///<ID
       
        JCMemClass*             m_pSaveCmd;             ///<缓存的指令
        JCGraphicsCmdDispath*   m_pGraphicsDispath;     ///<存储GraphicsDispatch的指针
        float                   m_nX;
        float                   m_nY;
        int                     m_nReplaceColor;
        bool                    m_bIsColorReplace;
        int                     m_nImageId;
        int                     m_nCmdNum;
        float*                  m_pDrawImageArgs;
        JCFontManager*          m_pFontManager;
        JCTextManager*          m_pTextManager;
        TEXT_BASE_LINE_TYPE     m_nBaseLineType;

    private:

        VectorWordInfos         m_vWordInfos;
        int                     m_nWordPos;
        std::vector<JCNode2D*>  m_vecNode2D;
    };
}

#endif //__JCGraphics_H__

//-----------------------------END FILE--------------------------------