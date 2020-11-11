/**
@file			JCGraphicsWordInfo.cpp
@brief			
@author			James
@version		1.0
@date			2016_8_2
*/

#include "JCGraphicsWordInfo.h"
#include "../Html5Render/JCHtml5Context.h"
#include <util/JCCommonMethod.h>
#include <cmath>
#include <algorithm> 
//------------------------------------------------------------------------------
namespace laya
{
    JCGraphicsWordInfo::JCGraphicsWordInfo(JCFontManager* pFontManager, JCTextManager* pTextManager, char* sText, const char* sFont, int nColor)
    {
        m_pFont = NULL;
        m_nColor = 0;
        m_nTextCountWidth = 0;
        setTextInfo(pFontManager, pTextManager, sText, sFont, nColor);
    }
    JCGraphicsWordInfo::~JCGraphicsWordInfo()
    {
        m_pFont = NULL;
        m_vWordRes.clear();
    }
    void JCGraphicsWordInfo::setTextInfo(JCFontManager* pFontManager, JCTextManager* pTextManager, const char* sText, const char* sFont, int nColor)
    {
        JCFontInfo* pFont = pFontManager->getFontInfoFromText(sFont);
        setTextInfo( pFontManager,pTextManager,sText,pFont,nColor );
    }
    void JCGraphicsWordInfo::setTextInfo(JCFontManager* pFontManager, JCTextManager* pTextManager, const char* sText, JCFontInfo* pFont, int nColor)
    {
        m_nColor = nColor;
        m_pFont = pFont;
        m_sText = sText;
        m_bDirty = true;
    }
    void JCGraphicsWordInfo::update(JCTextManager* pTextManager, float scaleX, float scaleY)
    {
        m_vWordRes.clear();

        int nLen = m_sText.length();
        if (nLen < 1)return;
        std::vector<std::string> vText = paserUTF8(m_sText, nLen);
        int nLastPos = 0;
        m_nTextCountWidth = 0;
        //这段代码效率不高，但是也没有好的办法，总比measureText 效率高。
        for (int i = 0, n = vText.size(); i < n; i++)
        {
            if (vText[i] == " ")
            {
                //处理空格
                int nWidth = (int)floorf((m_pFont->m_nFontSize + 1.0f) / 4);
                m_nTextCountWidth += nWidth;
                m_vWordRes.push_back(NULL);
            }
            else
            {
                //todo
                JCWordRes* pWordRes = pTextManager->getWordRes(m_pFont, m_nColor, vText[i].c_str(), scaleX, scaleY);
                if (pWordRes != NULL)
                {
                    if (pWordRes->enable() == ENALBE_TYPE_FAIL)continue;
                    m_nTextCountWidth += pWordRes->m_kRectNoScaled.width;
                    m_vWordRes.push_back(pWordRes);
                }
            }
        }
    }
    void JCGraphicsWordInfo::modifyTextInfo(JCFontManager* pFontManager, JCTextManager* pTextManager, int nColor)
    {
        m_vWordRes.clear();
        setTextInfo( pFontManager,pTextManager,m_sText.c_str(),m_pFont,nColor );
    }
    void JCGraphicsWordInfo::modifyTextInfo(JCFontManager* pFontManager, JCTextManager* pTextManager, const char* sText)
    {
        m_vWordRes.clear();
        setTextInfo(pFontManager, pTextManager, sText, m_pFont, m_nColor);
    }
    std::vector<JCWordRes*>& JCGraphicsWordInfo::getWordRes(JCHtml5Context* pRenderContext, JCFontManager* pFontManager, JCTextManager* pTextManager)
    { 
        float sx = pRenderContext->m_pContextData->m_pCurrentContextData->kMatrix.getScaleX();
        float sy = pRenderContext->m_pContextData->m_pCurrentContextData->kMatrix.getScaleY();
        short scaledFontSizeX = sx * m_pFont->m_nFontSize;
        short scaledFontSizeY = sy * m_pFont->m_nFontSize;
        bool fontScaledChanged = (m_lastScaledFontSizeX != scaledFontSizeX) || (m_lastScaledFontSizeY != scaledFontSizeY);
        m_lastScaledFontSizeX = scaledFontSizeX;
        m_lastScaledFontSizeY = scaledFontSizeY;
        if (m_bDirty || fontScaledChanged)
        {
            update(pTextManager, sx, sy);
            m_bDirty = false;
        }
        return m_vWordRes; 
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------

