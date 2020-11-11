/**
@file			JCTextMemoryCanvas.h
@brief
@author			James
@version		1.0
@date			2016_5_17
*/

#ifndef __JCTextMemoryCanvas_H__
#define __JCTextMemoryCanvas_H__

#include <vector>
#include <resource/text/JCFontInfo.h>
#include <FontRender/JCFreeTypeRender.h>
#include <imageLib/JCImageRW.h>
#include <resource/text/JCFontManager.h>

namespace laya
{
    struct JCTextUnitInfo
    {
        JCTextUnitInfo()
        {
            ref = 0;
            targetTextureID = 0;
            bColor = false;
            fontInfo = NULL;
            color = 0;
            x = y = 0;
        }
        ~JCTextUnitInfo()
        {
            fontInfo = NULL;
        }
        JCFontInfo*     fontInfo;           //文字的信息
        std::string     text;               //文字内容
        int             color;              //颜色
        int             x;                  //x
        int             y;                  //y
        bool            bColor;             //颜色
        int             targetTextureID;    //目标纹理ID
        int             ref;                //引用计数
    };
    class JCTextMemoryCanvas
    {
    public:

        JCTextMemoryCanvas();

        ~JCTextMemoryCanvas();

    public:

        void swapMemoryCanvasData();

        void setFontInfo(const char* sFontInfo);

        const char* getFontInfo();

        void setFillStyle(const char* sColor);

        const char* getFillStyle();

        void fillText(const char* sText, int x, int y);

        void setTargetTextureID(int nTextureID);

        void clearRenderThreadData();

        void  setTextBaseline(const char* baseline);

        const char *getTextBaseline();

    public:

        void setFreeTypeFontRender(JCFreeTypeFontRender* pFreeTypeRender);

        void setFontManager(JCFontManager* pFontManager);

        bool drawTextToBitmapData(JCTextUnitInfo* pTextInfo);

        JCTextUnitInfo* getTextInfoFromRenderThread();

        void reset();

    public:

        BitmapData                      m_pBitmapData;
        float                           m_nTextBaseline;
        JCTextUnitInfo*                 m_pCurrentTexUnit;      //文字单元的info

    protected:

        std::vector<JCTextUnitInfo*>    m_vTextForRender;       //文字的info给渲染线程的
        std::vector<JCTextUnitInfo*>    m_vTextForJS;           //文字的info给JS线程的
        std::string                     m_sCurrentFont;
        std::string                     m_sCurrentFillStyle;

    private:
        int                             m_nMaxTextSize;
        int                             m_nCurrentPos;
        JCFontManager*                  m_pFontManager;
        JCFreeTypeFontRender*           m_pFontRender;
    };
}
//------------------------------------------------------------------------------


#endif //__JCTextMemoryCanvas_H__

//-----------------------------END FILE--------------------------------