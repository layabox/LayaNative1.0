/**
@file			JCNode.h
@brief			
@author			James
@version		1.0
@date			2016_10_13
*/

#ifndef __JCNode_H__
#define __JCNode_H__

#include <vector>
#include <geom/Rectangle.h>
#include <geom/Matrix32.h>
#include "JCGraphics.h"
#include "JCNodeStyle.h"
#include <buffer/JCBuffer.h>

namespace laya
{
    enum NODE_TYPE
    {
        NODE_TYPE_2D = 0,
        NODE_TYPE_3D,
        NODE_TYPE_SCENE,
        NODE_TYPE_CAMERA
    };
    /** 
     * @brief 
    */
    class JCNode
    {
    public:

        JCNode(int nID, NODE_TYPE nType);

        virtual ~JCNode();

        virtual void render(JCHtml5Context* pContext, float x=0, float y=0){}

        virtual void addChildAt( JCNode* pNode,int nInsertNum = -1 );

        virtual void removeChild( JCNode* pNode );

        virtual void alpha(float fAlpha){}

        virtual void visible(bool bVisible){}

        virtual void blendMode(int nBlendMode){}

        virtual void repaint();

    protected:

        virtual void parentRepaint();
        virtual void removeAllChildren();
        virtual void removeThisFromParent();

    public:
        static int                  s_nNodeCount;
        int                         m_nID;
        NODE_TYPE                   m_nType;
        int                         m_nRenderType;
        int                         m_nRepaint;
        std::vector<JCNode*>        m_vChildren;
        JCNode*                     m_pParent;
    };
}
//------------------------------------------------------------------------------


#endif //__JCNode_H__

//-----------------------------END FILE--------------------------------