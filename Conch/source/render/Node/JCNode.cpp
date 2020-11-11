/**
@file			JCNode.cpp
@brief			
@author			James
@version		1.0
@date			2016_10_13
*/

#include "JCNode.h"
#include <resource/JCResManager.h>
#include "../Image/JCImageManager.h"
#include <3DRS/JC2DShaderManager.h>
#include "../Html5Render/JCHtml5RenderManager.h"
#include <resource/text/JCTextManager.h>
#include <resource/text/JCFontManager.h>
#include "../Manager/JCBufferManager.h"
#include "../Manager/JCShaderManager.h"
#include "JCNode2DRenderer.h"
#include <util/Log.h>
#include "util/Log.h"
#include "../Html5Render/JCHtml5Render.h"
#include "JCGraphicsCmdDispath.h"

//------------------------------------------------------------------------------
namespace laya
{
    int JCNode::s_nNodeCount = 0;
    JCNode::JCNode(int nID,NODE_TYPE nType)
    {
        m_nID = nID;
        m_nType = nType;
        m_pParent = NULL;
        m_nRepaint = 1;
    }
    JCNode::~JCNode()
    {
        removeAllChildren();
        removeThisFromParent();
    }
    void JCNode::removeAllChildren()
    {
        for (int i = 0, sz = m_vChildren.size(); i < sz; i++)
        {
            JCNode* pNode = m_vChildren[i];
            if (pNode)
            {
                pNode->m_pParent = NULL;
                pNode->removeAllChildren();
            }
        }
        m_vChildren.clear();
    }
    void JCNode::removeThisFromParent()
    {
        if (m_pParent)
        {
            for (std::vector<JCNode*>::iterator iter = m_pParent->m_vChildren.begin(); iter != m_pParent->m_vChildren.end(); )
            {
                if ((*iter) == this)
                {
                    iter = m_pParent->m_vChildren.erase(iter);
                    break;
                }
                else
                {
                    iter++;
                }
            }
        }
        m_pParent = NULL;
    }
    void JCNode::addChildAt( JCNode* pNode, int nInsertNum)
    {
        int nSize = m_vChildren.size();
        if (nSize == nInsertNum || nInsertNum == -1 )
        {
            m_vChildren.push_back(pNode);
        }
        else if (nInsertNum < nSize)
        {
            m_vChildren.insert(m_vChildren.begin() + nInsertNum, pNode);
           /* if (m_vChildren[nInsertNum])
            {
                //只是把当前存在的节点和parent断开了关系，children还是他的
                m_vChildren[nInsertNum]->m_pParent = NULL;
            }
            m_vChildren[nInsertNum] = pNode;*/
        }
        else
        {
            m_vChildren.resize(nInsertNum + 1);
            m_vChildren[nInsertNum] = pNode;
        }
        pNode->m_pParent = this;
        m_nRenderType |= NRT_CHILDS;
    }
    void JCNode::removeChild(JCNode* pNode)
    {
        for (std::vector<JCNode*>::iterator iter = m_vChildren.begin(); iter != m_vChildren.end(); )
        {
            JCNode* pChild = *iter;
            if (pChild == pNode)
            {
                iter = m_vChildren.erase(iter);
                pNode->m_pParent = NULL;
                break;
            }
            else
            {
                iter++;
            }
        }
    }
    void JCNode::parentRepaint()
    {
        if (m_pParent && m_pParent->m_nRepaint == 0)
        {
            m_pParent->m_nRepaint = 1;
            m_pParent->parentRepaint();
        }
    }
    void JCNode::repaint()
    {
        if (m_nRepaint == 0)
        {
            m_nRepaint = 1;
            parentRepaint();
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------