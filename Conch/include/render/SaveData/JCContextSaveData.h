/**
@file			JCContextSaveData.h
@brief			
@author			James
@version		1.0
@date			2016_5_25
*/

#ifndef __JCContextSaveData_H__
#define __JCContextSaveData_H__

#include <geom/Rectangle.h>
#include <geom/Matrix32.h>
#include <3DRS/JCMaterial.h>

namespace laya
{
    struct JCFilter
    {
        float r, g, b, gray;
        JCFilter()
        {
            r = 1;
            g = 1;
            b = 1;
            gray = 0;
        }
        bool isIdentity()
        {
            return (r == 1 && g == 1 && b == 1 && gray == 0);
        }
    };
    struct JCFilterMatrix
    {
        JCFilterMatrix()
        {
            memset(matrix, 0, sizeof(float) * 16);
            memset(alpha, 0, sizeof(float) * 4);
        }
        float matrix[16];
        float alpha[4];
    };
    enum FILE_STYLE_TYPE
    {
        FILL_TYPE_COLOR,
        FILL_TYPE_PATTERN,
    };
    enum STROKE_STYLE_TYPE
    {
        STROKE_TYPE_COLOR,
    };
    enum TEXT_BASE_LINE_TYPE
    {
        TBL_TYPE_ALPHABETIC = 0,
        TBL_TYPE_TOP,
        TBL_TYPE_HANGING,
        TBL_TYPE_MIDDLE,
        TBL_TYPE_IDEOGRAPHIC,
        TBL_TYPE_BOTTOM,
        TBL_TYPE_COUNT,
    };
    enum TEXT_ALIGN_TYPE
    {
        TEXT_ALIGN_START = 0,
        TEXT_ALIGN_LEFT,
        TEXT_ALIGN_CENTER,
        TEXT_ALIGN_END,
        TEXT_ALIGN_RIGHT,
    };
    enum  LINECAP_TYPE
    {
        LINECAP_TYPE_BUTT=0,
        LINECAP_TYPE_ROUND=1,
        LINECAP_TYPE_SQUARE=2,
    };

    enum LINEJOIN_TYPE
    {
        LINEJOIN_TYPE_MITER=0,
        LINEJOIN_TYPE_ROUND=1,
        LINEJOIN_TYPE_BEVEL=2,
    };

    enum FILL_PATTERN_TYPE
    {
        FILL_PATTERN_TYPE_REPEAT=0,
        FILL_PATTERN_TYPE_REPEAT_X=1,
        FILL_PATTERN_TYPE_REPEAT_Y=2,
        FILL_PATTERN_TYPE_NO_REPEAT=3,
    };
    enum FILTER_TYPE
    {
        FILTER_TYPE_NO,
        FILTER_TYPE_COLOR,
        FILTER_TYPE_MATRIX,
    };

    enum MASK_TYPE
    {
        MASK_TYPE_NO,
        MASK_TYPE_STENCIL,
        MASK_TYPE_SCISSOR,
    };

    struct ContextDataInfo
    {
        ContextDataInfo()
        {
            nFillType = FILL_TYPE_COLOR;
            nFillStyle = 0xffffffff;
            nStrokeType = STROKE_TYPE_COLOR;
            nStorkeStyle = 0xffffffff;
            nLineWidth = 1;
            nTextBaseLineType = TBL_TYPE_TOP;
            nTextAlignType = TEXT_ALIGN_START;
            nBlendType = BLEND_TYPE_SOURCE_OVER;
            nShadowColor = 0;
            nShadowBlur = 0;
            nShadowOffsetX = 0;
            nShadowOffsetY = 0;
            nLineCap = 0;
            nLineJoin = 0;
            nMiterLimit = 0;
            nFilterType = FILTER_TYPE_NO;
            nPatternType = FILL_PATTERN_TYPE_REPEAT;
            nAlpha = 1;
            nClip = 0;
            kMatrix.identity();
            memset(sFont, 0, sizeof(char) * 64);
            nClipType = MASK_TYPE_NO;
        }
        unsigned short nFillType;
        unsigned short nStrokeType;
        unsigned short nTextBaseLineType;
        unsigned short nTextAlignType;
        unsigned short nBlendType;
        unsigned short nPatternType;
        unsigned short nFilterType;//是否有filter
        unsigned short nClip;
        unsigned short nClipType;
        int nFillStyle;
        int nStorkeStyle;
        int nLineWidth;
        int nShadowColor;
        int nShadowBlur;
        int nShadowOffsetX;
        int nShadowOffsetY;
        int nLineCap;
        int nLineJoin;
        int nMiterLimit;
        float nAlpha;
        Rectangle kRect;
        Rectangle kClipRect;
        Rectangle kPattern;
        JCFilter kFilter;
        Matrix32 kMatrix;
        JCFilterMatrix kFilterMatrix;
        char sFont[64];
    };

    /** 
     * @brief 
    */
    class JCContextSaveData
    {
    public:

        /** @brief构造函数
        */
        JCContextSaveData();

        /** @brief析构函数
        */
        ~JCContextSaveData();


        /** @brief reset函数
         *  @param[in] canvas的宽
         *  @param[in] canvas的高
         *  @return 
        */
        bool reset( int w,int h);

        /** @brief save保存状态
         *  @return 返回成功失败
        */
        bool save();


        /** @brief 恢复函数 
         *  @return 返回成功失败
        */
        bool restore();

    public:

        ContextDataInfo*    m_pCurrentContextData;    ///<当前的ContextData

    private:

        char*			    m_pBuffer;                ///<保存的buffer
        int                 m_nCurrentBufferPos;      ///<当前的bufferPos
        int                 m_nSaveDataSize;          ///<存储的dataSize

    };
}

#endif //__JCContextSaveData_H__

//-----------------------------END FILE--------------------------------
