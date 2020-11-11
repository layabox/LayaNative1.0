/**
@file			JCNode2D.h
@brief			
@author			James
@version		1.0
@date			2016_6_13
*/

#ifndef __JCNode2D_H__
#define __JCNode2D_H__

#include <vector>
#include "../Html5Render/JCHtml5Context.h"
#include <geom/Rectangle.h>
#include <geom/Matrix32.h>
#include "JCGraphics.h"
#include "JCNodeStyle.h"
#include <buffer/JCBuffer.h>
#include "JCNode.h"

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
    class JCHtml5Context;
    class JCParticleTemplate2DManager;

    enum CACHE_CANVAS_REASON
    {
        CCR_ORIGINAL = 1,
        CCR_FILTER = 2,
        CCR_MASK = 4,
    };
    /** 
     * @brief 
    */
    class JCNode2D : public JCNode
    {
    public:

        JCNode2D(int nID,JCResManager* pTextureManager, JCImageManager* pImageManager, JC2DShaderManager* p2DShaderManager,
            JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager,
            JCBufferManager* pBufferManager, JCShaderManager* pShaderManager, std::vector<JCICmdDispatch*>* vVectorCmdDispath);

        ~JCNode2D();

        void render( JCHtml5Context* pContext,float x = 0,float y = 0 );

        void size(float w, float h);

        void pos(float x, float y);

        void pivot(float x,float y);

        void scale(float x,float y);

        void skew(float x, float y);

        void rotate(float r);

        void bgColor( int nColor );

        void border(int nColor);

        void font(const char* sFont);

        void text();

        void transform(float m11, float m12, float m21, float m22, float m31, float m32);

        void setTransform(float m11, float m12, float m21, float m22, float m31, float m32);

        void alpha(float fAlpha);

        void filterMatrix(float* pColorMatrix,float* pAlpha);

        void filter(float r,float g,float b,float gray);

        void blurFilter(float value);

        void glowFilter(int color, float blur,float offsetX, float offsetY);

        void visible(bool bVisible);

        void blendMode(int nBlendMode);

        void scrollRect(float x,float y,float w,float h);

        void optimizeScrollRect(bool b);

        void mask( JCNode2D* pNodeID );

        void graphics(JCGraphics* pGraphics, bool bNotifyDetach = true);

        void removeType(int type);

        void custom( JCMemClass* pCustomData );

        void cacheAs(CANVAS_TYPE nCanvasType);

        Matrix32* getTransform();

        void getSelfBound( Rectangle& kResultRect );

        void getParentBound( Rectangle& kResultRect );

        void getSelfAllPoint(std::vector<float>& pResultArrays, const Matrix32& kMatrix,bool bFirstNode);

		void updateOrder();

        void removeCacheCanvas();

        void drawToCanvas(JCHtml5Render* pHtml5Render,float offsetX,float offsetY );

    private:

        void applyTransform();
        int getCreateCacheCanvasID();
        void removeCacheCanvasFromType(CACHE_CANVAS_REASON nType);
        void _cacheAs(CANVAS_TYPE nCanvasType);

    public:
        static int                  m_nCreateCacheCanvasID;
        JCNode2D*                   m_pMask;
    public:
        JCGraphics*                 m_pGraphics;
        JCMemClass*                 m_pCustomData;
        JCNodeStyle                 m_kNodeStyle;
		float						m_nZOrder;
        float                       m_nX;
        float                       m_nY;
        float                       m_nWidth;
        float                       m_nHeight;
        bool                        m_bTransformChange;
        bool                        m_bOptimizeScrollRect;
        JCHtml5Render*              m_pCacheCanvas;
        Rectangle                   m_kBoundingBox;
        Rectangle                   m_kParentBoundingBox;
        Matrix32*                   m_pMatrix;
    private:
        JCResManager*               m_pTextureManager;
        JCImageManager*             m_pImageManager;
        JC2DShaderManager*          m_p2DShaderManager;
        JCHtml5RenderManager*       m_pHtml5RenderManager;
        JCTextManager*              m_pTextManager;
        JCFontManager*              m_pFontManager;
        JCBufferManager*            m_pBufferManager;
        JCShaderManager*            m_pShaderManager;
		std::vector<JCICmdDispatch*>*       m_vVectorCmdDispath;
        int                         m_nCacheCanvasReason;
    };
}

#endif //__JCNode2D_H__

//-----------------------------END FILE--------------------------------