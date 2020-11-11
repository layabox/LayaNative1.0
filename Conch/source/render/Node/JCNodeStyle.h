/**
@file			JCNodeStyle.h
@brief			
@author			James
@version		1.0
@date			2016_6_15
*/

#ifndef __JCNodeStyle_H__
#define __JCNodeStyle_H__

#include <geom/Rectangle.h>
#include "../SaveData/JCContextSaveData.h"

namespace laya
{
    #define PI          3.1415926535897932f
    #define ARC_TO_RAD  0.0174532925199433f
    struct JCTransform
    {
        float rotate;
        float scaleX;
        float scaleY;
        float skewX;
        float skewY;
        float pivotX;
        float pivotY;
        JCTransform()
        {
            rotate = skewX = skewY = pivotX = pivotY = 0;
            scaleX = scaleY = 1;
        }
    };
    struct JCGlowFilter
    {
        int color;
        float blur;
        float offsetX;
        float offsetY;
        JCGlowFilter(int nColor, float fBlur, float fOffsetX, float fOffsetY)
        {
            color = nColor;
            blur = fBlur;
            offsetX = fOffsetX;
            offsetY = fOffsetY;
        }
    };
    /** 
     * @brief 
    */
    class JCNodeStyle
    {
    public:
        enum BGTYPE
        {
            BG_BGCOLOR = 0x01,
            BG_BORDERCOLOR = 0x02,
        };
        

        /** @brief构造函数
        */
        JCNodeStyle();

        /** @brief析构函数
        */
        ~JCNodeStyle();

        void setPoivt(float x,float y);

        void setScale(float x, float y);

        void setRotateAngle( float fAngle);

        void setRotateRadian( float fRadian );
    
        void setSkewAngle(float x, float y);

        void setSkewRadian(float x, float y);

    public:

        static JCTransform*     _TF_EMPTY;

    public:

        JCTransform*            m_pTransform;
        float                   m_fAlpha;
        bool                    m_bVisible;
        Rectangle*              m_pClipRect;
        int                     m_nBlendMode;
        int                     m_nBgColor;
        int                     m_nBorderColor;
        int                     m_nType;
        JCFilter*               m_pFilter;
        JCFilterMatrix*         m_pFilterMatrix;
        JCGlowFilter*           m_pGlowFilter;
        float                   m_fBlurFilterValue;
    };
}

#endif //__JCNodeStyle_H__

//-----------------------------END FILE--------------------------------