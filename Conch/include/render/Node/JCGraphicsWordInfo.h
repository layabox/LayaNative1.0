/**
@file			JCGraphicsWordInfo.h
@brief			
@author			James
@version		1.0
@date			2016_8_2
*/

#ifndef __JCGraphicsWordInfo_H__
#define __JCGraphicsWordInfo_H__

#include <string>
#include <resource/text/JCTextManager.h>
#include <resource/text/JCFontManager.h>
#include <vector>

namespace laya
{
    class JCHtml5Context;
    /** 
     * @brief 
    */
    class JCGraphicsWordInfo
    {
    public:

        /** @brief构造函数
        */
        JCGraphicsWordInfo(JCFontManager* pFontManager, JCTextManager* pTextManager, char* sText, const char* sFont, int nColor );

        /** @brief析构函数
        */
        ~JCGraphicsWordInfo();

        void setTextInfo( JCFontManager* pFontManager,JCTextManager* pTextManager, const char* sText, const char* sFont,int nColor );

        void modifyTextInfo(JCFontManager* pFontManager, JCTextManager* pTextManager, int nColor );

        void modifyTextInfo(JCFontManager* pFontManager, JCTextManager* pTextManager, const char* sText);

        std::vector<JCWordRes*>& getWordRes(JCHtml5Context* pRenderContext, JCFontManager* pFontManager, JCTextManager* pTextManager);

        void update(JCTextManager* pTextManager, float scaleX, float scaleY);

    private:

        void setTextInfo(JCFontManager* pFontManager, JCTextManager* pTextManager, const char* sText, JCFontInfo* pFont, int nColor);

    public:

        std::string                 m_sText;
        JCFontInfo*                 m_pFont;
        int                         m_nColor;
        int                         m_nTextCountWidth;

    private:

        std::vector<JCWordRes*>     m_vWordRes;
        bool                        m_bDirty;
        short                       m_lastScaledFontSizeX = 0;
        short                       m_lastScaledFontSizeY = 0;
    };
}

#endif //__JCGraphicsWordInfo_H__

//-----------------------------END FILE--------------------------------