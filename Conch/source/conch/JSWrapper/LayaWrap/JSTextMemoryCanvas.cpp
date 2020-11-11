/**
@file			JSTextMemoryCanvas.cpp
@brief			
@author			James
@version		1.0
@date			2016.3.22
*/
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <math.h>
#include "JSTextMemoryCanvas.h"
#include "JSMeasureTextInfo.h"
#include "../../JCScrpitRuntime.h"
#include <util/Log.h>

namespace laya
{
	ADDJSCLSINFO(JSTextMemoryCanvas, JSObjNode);
    JSTextMemoryCanvas::JSTextMemoryCanvas()
    {
        AdjustAmountOfExternalAllocatedMemory(20480);
		m_pTextMemoryCanvas = NULL;
        m_nWidth = MAX_FONT_SIZE + TEXT_SIZE_ALLOWANCE;
        m_nHeight = MAX_FONT_SIZE + TEXT_SIZE_ALLOWANCE;
    }
    JSTextMemoryCanvas::~JSTextMemoryCanvas()
    {
    }
    void JSTextMemoryCanvas::setFontInfo(const char* sFontInfo)
    {
		if (m_pTextMemoryCanvas)m_pTextMemoryCanvas->setFontInfo(sFontInfo);
    }
    void JSTextMemoryCanvas::setFillStyle(const char* sColor)
    {
		if (m_pTextMemoryCanvas)m_pTextMemoryCanvas->setFillStyle(sColor);
    }
    const char* JSTextMemoryCanvas::getFontInfo()
    {
		if (m_pTextMemoryCanvas)return m_pTextMemoryCanvas->getFontInfo();
		return "";
    }
    const char* JSTextMemoryCanvas::getFillStyle()
    {
		if (m_pTextMemoryCanvas)return m_pTextMemoryCanvas->getFillStyle();
        return "";
    }
    void JSTextMemoryCanvas::fillText(const char* sText, int x, int y)
    {
		if (m_pTextMemoryCanvas)m_pTextMemoryCanvas->fillText(sText, x, y);
    }
	JsValue JSTextMemoryCanvas::measureText(const char* sText)
    {
        if (m_pTextMemoryCanvas == NULL)return JSP_TO_JS_NULL;
        JSMeasureTextInfo* pMTextInfo = new JSMeasureTextInfo();
        if (m_pTextMemoryCanvas->m_pCurrentTexUnit == NULL || m_pTextMemoryCanvas->m_pCurrentTexUnit->fontInfo == NULL || sText == NULL)
        {
            LOGW("JSMemoryCanvas::measureText 没有设置FontInfo");
            return JSP_TO_JS(JSMeasureTextInfo, pMTextInfo);
        }
        pMTextInfo->measureText(m_pTextMemoryCanvas->m_pCurrentTexUnit->fontInfo, sText);
        return JSP_TO_JS(JSMeasureTextInfo, pMTextInfo);
    }
	void JSTextMemoryCanvas::setTextBaseline(const char* baseLine)
	{
		if (m_pTextMemoryCanvas)m_pTextMemoryCanvas->setTextBaseline(baseLine);
	}
	const char* JSTextMemoryCanvas::getTextBaseline()
	{
		if (m_pTextMemoryCanvas)return m_pTextMemoryCanvas->getTextBaseline();
		return  "bottom";
	}
    void JSTextMemoryCanvas::setTargetTextureID(int nTextureID)
    {
		if (m_pTextMemoryCanvas)m_pTextMemoryCanvas->setTargetTextureID(nTextureID);
    }
    bool JSTextMemoryCanvas::isTextCanvas()
    {
        return true;
    }
    void JSTextMemoryCanvas::setSize( int w,int h )
    {
        m_nWidth = w;
        m_nHeight = h;
    }
    void JSTextMemoryCanvas::setWidth(int w)
    {
        m_nWidth = w;
    }
    int JSTextMemoryCanvas::getWidth()
    {
        return m_nWidth;
    }
    void JSTextMemoryCanvas::setHeight(int h)
    {
        m_nHeight = h;
    }
    int JSTextMemoryCanvas::getHeight()
    {
        return m_nHeight;
    }
    int JSTextMemoryCanvas::getImageID()
    {
        return 1;
    }
	void JSTextMemoryCanvas::exportJS() 
    {
        JSP_GLOBAL_CLASS("ConchTextCanvas", JSTextMemoryCanvas);
        JSP_ADD_PROPERTY(fillStyle, JSTextMemoryCanvas, getFillStyle, setFillStyle);
        JSP_ADD_PROPERTY(font, JSTextMemoryCanvas, getFontInfo, setFontInfo);
        JSP_ADD_PROPERTY(textBaseline, JSTextMemoryCanvas, getTextBaseline, setTextBaseline);
        JSP_ADD_PROPERTY(width, JSTextMemoryCanvas, getWidth, setWidth);
        JSP_ADD_PROPERTY(height, JSTextMemoryCanvas, getHeight, setHeight);
        JSP_ADD_PROPERTY_RO(conchImgId, JSTextMemoryCanvas, getImageID);
        JSP_ADD_METHOD("setFillStyle", JSTextMemoryCanvas::setFillStyle);
        JSP_ADD_METHOD("setFontInfo", JSTextMemoryCanvas::setFontInfo);
        JSP_ADD_METHOD("isTextCanvas", JSTextMemoryCanvas::isTextCanvas);
        JSP_ADD_METHOD("measureText", JSTextMemoryCanvas::measureText);
        JSP_ADD_METHOD("fillText", JSTextMemoryCanvas::fillText);
        JSP_ADD_METHOD("setTargetTextureID", JSTextMemoryCanvas::setTargetTextureID);
        JSP_ADD_METHOD("size", JSTextMemoryCanvas::setSize);
        JSP_INSTALL_GLOBAL_CLASS("ConchTextCanvas", JSTextMemoryCanvas, this);
	}
}