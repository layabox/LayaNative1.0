/**
@file			JCNode2DCmdDispath.h
@brief			
@author			James
@version		1.0
@date			2016_6_14
*/

#ifndef __JCNode2DCmdDispath_H__
#define __JCNode2DCmdDispath_H__

#include "JCNode2D.h"
#include <JCICmdDispatch.h>

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
    class JCNode2DCmdDispath : public JCICmdDispatch
    {
    public:

        typedef void (JCNode2DCmdDispath::*ProcFunction)(JCMemClass& cmdBuffer);

    public:

        /** @brief构造函数
        */
        JCNode2DCmdDispath(int nID,JCResManager* pTextureManager, JCImageManager* pImageManager, JC2DShaderManager* p2DShaderManager, 
                    JCHtml5RenderManager* pHtml5RenderManager, JCTextManager* pTextManager, JCFontManager* pFontManager,
                    JCBufferManager* pBufferManager, JCShaderManager* pShaderManager,std::vector<JCICmdDispatch*>* vVectorCmdDispath);

        /** @brief析构函数
        */
        ~JCNode2DCmdDispath();

        bool restartJSThread();

        bool dispatchRenderCmd(JCMemClass& pRenderCmd, int nFrameCount);

        bool dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount);

    public:

        void _rendercmd_addChildAt(JCMemClass& cmdBuffer);

        void _rendercmd_removeChild(JCMemClass& cmdBuffer);

        void _rendercmd_size(JCMemClass& cmdBuffer);

        void _rendercmd_pos(JCMemClass& cmdBuffer);

        void _rendercmd_pivot(JCMemClass& cmdBuffer);

        void _rendercmd_scale(JCMemClass& cmdBuffer);

        void _rendercmd_skew(JCMemClass& cmdBuffer);

        void _rendercmd_rotate(JCMemClass& cmdBuffer);

        void _rendercmd_bgColor(JCMemClass& cmdBuffer);

        void _rendercmd_font(JCMemClass& cmdBuffer);

        void _rendercmd_text(JCMemClass& cmdBuffer);

        void _rendercmd_transform(JCMemClass& cmdBuffer);

        void _rendercmd_setTransform(JCMemClass& cmdBuffer);

        void _rendercmd_alpha(JCMemClass& cmdBuffer);

        void _rendercmd_filter(JCMemClass& cmdBuffer);

        void _rendercmd_visible(JCMemClass& cmdBuffer);

        void _rendercmd_blendMode(JCMemClass& cmdBuffer);

        void _rendercmd_scrollRect(JCMemClass& cmdBuffer);

        void _rendercmd_mask(JCMemClass& cmdBuffer);

        void _rendercmd_graphics(JCMemClass& cmdBuffer);

        void _rendercmd_custom(JCMemClass& cmdBuffer);

        void _rendercmd_removeType(JCMemClass& cmdBuffer);

        void _rendercmd_cacheAs(JCMemClass& cmdBuffer);

        void _rendercmd_border(JCMemClass& cmdBuffer);

        void _rendercmd_optimizeScrollRect(JCMemClass& cmdBuffer);

        void _rendercmd_blurFilter(JCMemClass& cmdBuffer);

        void _rendercmd_glowFilter(JCMemClass& cmdBuffer);

        void _rendercmd_filterMatrix(JCMemClass& cmdBuffer);

        void _rendercmd_repaint(JCMemClass& cmdBuffer);

		void _rendercmd_setZOrder(JCMemClass& cmdBuffer);

		void _rendercmd_updateZOrder(JCMemClass& cmdBuffer);

        void _rendercmd_drawToCanvas(JCMemClass& cmdBuffer);

    public:

        std::vector<JCICmdDispatch*>*       m_vVectorCmdDispath;
        JCNode2D*                           m_pNode2D;

    };
}

#endif //__JCNode2DCmdDispath_H__

//-----------------------------END FILE--------------------------------
