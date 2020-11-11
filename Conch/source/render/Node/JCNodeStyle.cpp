/**
@file			JCNodeStyle.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_15
*/

#include "JCNodeStyle.h"
#include <3DRS/JCMaterial.h>

//------------------------------------------------------------------------------
namespace laya
{
    JCTransform* JCNodeStyle::_TF_EMPTY = new JCTransform();
    JCNodeStyle::JCNodeStyle()
    {
        m_pTransform = _TF_EMPTY;
        m_fAlpha = 1.0;
        m_bVisible = true;
        m_nBlendMode = BLEND_TYPE_NONE;
        m_nBgColor = 0;
        m_nBorderColor = 0;
        m_pClipRect = NULL;;
        m_pFilter = NULL;
        m_pFilterMatrix = NULL;
        m_pGlowFilter = NULL;
        m_fBlurFilterValue = 0;
        m_nType = 0;
    }
    JCNodeStyle::~JCNodeStyle()
    {
        if (m_pTransform != _TF_EMPTY)
        {
            delete m_pTransform;
            m_pTransform = NULL;
        }
        if (m_pClipRect)
        {
            delete m_pClipRect;
            m_pClipRect = NULL;
        }
        if (m_pFilter)
        {
            delete m_pFilter;
            m_pFilter = NULL;
        }
        if (m_pFilterMatrix)
        {
            delete m_pFilterMatrix;
            m_pFilterMatrix = NULL;
        }
        if (m_pGlowFilter)
        {
            delete m_pGlowFilter;
            m_pGlowFilter = NULL;
        }
    }
    void JCNodeStyle::setPoivt(float x, float y)
    {
        m_pTransform = (m_pTransform != _TF_EMPTY) ? m_pTransform : new JCTransform();
        m_pTransform->pivotX = x;
        m_pTransform->pivotY = y;
    }
    void JCNodeStyle::setScale(float x, float y)
    {
        m_pTransform = (m_pTransform != _TF_EMPTY) ? m_pTransform : new JCTransform();
        m_pTransform->scaleX = x;
        m_pTransform->scaleY = y;
    }
    void JCNodeStyle::setRotateAngle(float fAngle)
    {
        m_pTransform = (m_pTransform != _TF_EMPTY) ? m_pTransform : new JCTransform();
        m_pTransform->rotate = fAngle * ARC_TO_RAD;
    }
    void JCNodeStyle::setRotateRadian(float fRadian)
    {
        m_pTransform = (m_pTransform != _TF_EMPTY) ? m_pTransform : new JCTransform();
        m_pTransform->rotate = fRadian;
    }
    void JCNodeStyle::setSkewAngle(float x, float y)
    {
        m_pTransform = (m_pTransform != _TF_EMPTY) ? m_pTransform : new JCTransform();
        m_pTransform->skewX = x * ARC_TO_RAD;
        m_pTransform->skewY = y * ARC_TO_RAD;
    }
    void JCNodeStyle::setSkewRadian(float x, float y)
    {
        m_pTransform = (m_pTransform != _TF_EMPTY) ? m_pTransform : new JCTransform();
        m_pTransform->skewX = x;
        m_pTransform->skewY = y;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------