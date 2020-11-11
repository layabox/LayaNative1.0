/**
@file			JSTextCanvas.cpp
@brief			
@author			James
@version		1.0
@date			2016.3.22
*/
#include "JCTextMemoryCanvas.h"
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <math.h>
#include <util/JCCommonMethod.h>
#include <util/Log.h>
#include <util/JCColor.h>
#ifdef ANDROID
#include "../../../conch/JCSystemConfig.h"
#include "../../../conch/CToJavaBridge.h"
#endif
namespace laya
{
    JCTextMemoryCanvas::JCTextMemoryCanvas()
	{
        m_nCurrentPos = 0;
		m_nTextBaseline = 0.0f;
		m_pFontRender = NULL;
        m_pFontManager = NULL;
		m_pCurrentTexUnit = NULL;
		m_sCurrentFillStyle = "#000000";
		m_sCurrentFont = "normal 100 16px Arial";
        m_nMaxTextSize = MAX_FONT_SIZE + TEXT_SIZE_ALLOWANCE;
		m_pBitmapData.m_nBpp = 32;
        m_pBitmapData.m_nWidth = m_nMaxTextSize;
        m_pBitmapData.m_nHeight = m_nMaxTextSize;
        //为了节省内存使用通用的一个
        m_pBitmapData.m_pImageData = &(JCFreeTypeFontRender::m_pWordBuff[0]);
	}
    JCTextMemoryCanvas::~JCTextMemoryCanvas()
	{
        if (m_pCurrentTexUnit)
        {
            delete m_pCurrentTexUnit;
            m_pCurrentTexUnit = NULL;
        }
	}
	void JCTextMemoryCanvas::setFontInfo(const char* sFontInfo)
	{
		if (sFontInfo == NULL)return;
		m_sCurrentFont = sFontInfo;
        if (!m_pCurrentTexUnit)
        {
            m_pCurrentTexUnit = new JCTextUnitInfo();
        }
        m_pCurrentTexUnit->fontInfo = m_pFontManager->getFontInfoFromText(sFontInfo);
	}
	void JCTextMemoryCanvas::setFillStyle(const char* sColor)
	{
		if (sColor == NULL)return;
		m_sCurrentFillStyle = sColor;
        if (!m_pCurrentTexUnit)
        {
            m_pCurrentTexUnit = new JCTextUnitInfo();
        }
        m_pCurrentTexUnit->color = JCColor::getColorUintFromString(sColor);
        m_pCurrentTexUnit->bColor = true;
	}
	const char* JCTextMemoryCanvas::getFontInfo()
	{
		return m_sCurrentFont.c_str();
	}
	const char* JCTextMemoryCanvas::getFillStyle()
	{
		return m_sCurrentFillStyle.c_str();
	}
	void JCTextMemoryCanvas::fillText(const char* sText, int x, int y)
	{
		if (sText == NULL)return;
        if (!m_pCurrentTexUnit)
        {
            m_pCurrentTexUnit = new JCTextUnitInfo();
        }
        m_pCurrentTexUnit->text = sText;
        m_pCurrentTexUnit->x = x;
        m_pCurrentTexUnit->y = y - (int)(m_nTextBaseline*m_pCurrentTexUnit->fontInfo->m_nFontSize);
	}
	void JCTextMemoryCanvas::setTextBaseline(const char* baseLine)
	{
		if (strcmp("top", baseLine) == 0)
			m_nTextBaseline = 0;
		else if (strcmp("middle", baseLine) == 0)
			m_nTextBaseline = 0.5;
		else if (strcmp("bottom", baseLine) == 0)
			m_nTextBaseline = 1;
	}
	const char* JCTextMemoryCanvas::getTextBaseline()
	{
		if (m_nTextBaseline == 1)
			return "bottom";
		else if (m_nTextBaseline == 0.5)
			return "middle";
		else
			return "top";
	}
	void JCTextMemoryCanvas::setTargetTextureID(int nTextureID)
	{
        if (m_pCurrentTexUnit)
        {
            m_pCurrentTexUnit->targetTextureID = nTextureID;
            m_vTextForJS.push_back(m_pCurrentTexUnit);
            m_pCurrentTexUnit = NULL;
        }
        else if( m_vTextForJS.size() > 0 )
        {
            //这种情况是为了处理，一个文字往不同的纹理位置上subImage
            //比如往大图合集中subImage4次，为了处理接缝问题
            //所以增加了引用计数
            JCTextUnitInfo* pLastTextUnitInfo = m_vTextForJS[m_vTextForJS.size() - 1];
            pLastTextUnitInfo->ref++;
            m_vTextForJS.push_back( pLastTextUnitInfo );
        }
        else
        {
            LOGE("JCTextMemoryCanvas::setTargetTextureID error");
        }
	}
    void JCTextMemoryCanvas::reset()
    {
        //两个Render都清空一下
        m_nCurrentPos = 0;
        if (m_vTextForRender.size() > 0)
        {
            for (std::vector<JCTextUnitInfo*>::iterator iter = m_vTextForRender.begin(); iter != m_vTextForRender.end(); iter++)
            {
                JCTextUnitInfo* pTextInfo = *iter;
                if (pTextInfo != NULL)
                {
                    if (pTextInfo->ref > 0)
                    {
                        pTextInfo->ref--;
                    }
                    else
                    {
                        delete pTextInfo;
                    }
                }
            }
            m_vTextForRender.clear();
        }
        if (m_vTextForJS.size() > 0)
        {
            for (std::vector<JCTextUnitInfo*>::iterator iter = m_vTextForJS.begin(); iter != m_vTextForJS.end(); iter++)
            {
                JCTextUnitInfo* pTextInfo = *iter;
                if (pTextInfo != NULL)
                {
                    if (pTextInfo->ref > 0)
                    {
                        pTextInfo->ref--;
                    }
                    else
                    {
                        delete pTextInfo;
                    }
                }
            }
            m_vTextForJS.clear();
        }
    }
	void JCTextMemoryCanvas::swapMemoryCanvasData()
	{
		if (m_vTextForJS.size() > 0)
		{
            std::swap(m_vTextForRender, m_vTextForJS);
		}
	}
	void JCTextMemoryCanvas::clearRenderThreadData()
	{
        m_nCurrentPos = 0;
		if (m_vTextForRender.size() > 0)
		{
            //TODO 等回头需要保存并作资源恢复的时候，保存下来就不删除了
            //或者确定不用C++这边做资源恢复，就直接使用的时候直接删除就OK了
            //日后再订
            for (std::vector<JCTextUnitInfo*>::iterator iter = m_vTextForRender.begin(); iter != m_vTextForRender.end(); iter++)
            {
                JCTextUnitInfo* pTextInfo = *iter;
                if ( pTextInfo != NULL )
                {
                    if (pTextInfo->ref > 0)
                    {
                        pTextInfo->ref--;
                    }
                    else
                    {
                        delete pTextInfo;
                    }
                }
            }
			m_vTextForRender.clear();
		}
	}
    JCTextUnitInfo* JCTextMemoryCanvas::getTextInfoFromRenderThread()
    {
        if (m_nCurrentPos >= (int)m_vTextForRender.size())
        {
            return NULL;
        }
        return m_vTextForRender[m_nCurrentPos++];
    }
	void JCTextMemoryCanvas::setFreeTypeFontRender(JCFreeTypeFontRender* pFreeTypeRender)
	{
		m_pFontRender = pFreeTypeRender;
	}
    void JCTextMemoryCanvas::setFontManager(JCFontManager* pFontManager)
    {
        m_pFontManager = pFontManager;
    }
	bool JCTextMemoryCanvas::drawTextToBitmapData(JCTextUnitInfo* pTextInfo)
	{
		if (pTextInfo == NULL)return false;
		memset(m_pBitmapData.m_pImageData, 0, m_nMaxTextSize * m_nMaxTextSize );
		if (pTextInfo->fontInfo)
		{
			int nLen = pTextInfo->text.length();
			if (nLen < 1) return false;
            m_pBitmapData.m_nWidth = m_nMaxTextSize;
            m_pBitmapData.m_nHeight = m_nMaxTextSize;
			std::vector<std::string> vText = paserUTF8(pTextInfo->text, nLen);
            if (vText.size() > 0)
            {
                m_pFontRender->getTextBitmapData((char*)(vText[0].c_str()), &m_pBitmapData, pTextInfo->color, pTextInfo->fontInfo, 1.0f, 1.0f);
            }
		}
		return true;
	}
}