/**
@file			JCNode2DRenderer.h
@brief			
@author			James
@version		1.0
@date			2016_6_13
*/

#ifndef __JCNode2DRenderer_H__
#define __JCNode2DRenderer_H__

#include <vector>
#include <functional>
#include <vector>

namespace laya
{
    class JCNode2D;
    class JCHtml5Context;
    enum NODE2D_RENDERER_TYPE
    {
        NRT_NO = 0x0,
        NRT_IMAGE = 0x01,
        NRT_ALPHA = 0x02,
        NRT_TRANSFORM = 0x04,
		NRT_BLEND=0x08,
        NRT_CANVAS = 0x10,
        NRT_FILTERS = 0x20,
        NRT_MASK = 0x40,
        NRT_CLIP = 0x80,
        NRT_STYLE = 0x100,
        NRT_GRAPHICS = 0x200,
        NRT_CUSTOM = 0x400,
        NRT_CHILDS = 0x800,
        NRT_INIT = 0x11111,
    };
    /** 
     * @brief 
    */
    class JCNode2DRenderer
    {
    public:

        typedef void (JCNode2DRenderer::*RenderFunction)(JCNode2D* pNode, JCHtml5Context* pRenderContet, float x, float y);
        typedef std::vector<JCNode2DRenderer*>  VectorRenderers;

    public:

        static void initAll();

        static void initSame(int* pArray, int nArrayLen, JCNode2DRenderer* pRenderer);

        static JCNode2DRenderer* getTypeRenderer(int type);

        static void clearAll();

    public:

        JCNode2DRenderer( int nType,JCNode2DRenderer* pNext );
        ~JCNode2DRenderer();

        void runRender(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);

        void initRenderFunction(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        void style(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        void no(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        void custom(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        void clip(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        void blend(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
		void mask(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        void colorFilter(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        void graphics(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        //这两个函数是layaAir中优化用的，我们先不做，因为下一步要是做模型缓存就统一再做
        void image(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        void image2(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        void alpha(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        void transform(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        void childs(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);
        void canvas(JCNode2D* pNode, JCHtml5Context* pRenderContext, float x, float y);

    public:

        static VectorRenderers                                          m_vVectorRenderers;
        static JCNode2DRenderer*                                        NORENDER;
        static JCNode2DRenderer*                                        INITRENDER;

    public:

        JCNode2DRenderer*                                               m_pNext;
        RenderFunction                                                  m_pFunction;

    };
}

#endif //__JCNode2DRenderer_H__

//-----------------------------END FILE--------------------------------