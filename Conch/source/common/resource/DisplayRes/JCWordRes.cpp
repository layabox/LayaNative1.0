/**
@file			JCWordRes.cpp
@brief			
@author			James
@version		1.0
@date			2016_6_2
*/

#include "JCWordRes.h"
#include "../../util/JCCommonMethod.h"
#include "../../FontRender/JCFreeTypeRender.h"
#include "../Atlas/JCAtlasManager.h"
#include "../../util/Log.h"
#ifdef ANDROID
#include "../../../conch/JCSystemConfig.h"
#include "../../../conch/CToJavaBridge.h"
#endif
#include <chrono>
//------------------------------------------------------------------------------
namespace laya
{
    JCWordRes::JCWordRes(JCAtlasManager* pAtlasManager,JCFreeTypeFontRender* pFreeTypeRender,JCFontInfo* pFont, int nColor, const char* sWord, float scaleX, float scaleY)
        : JCInAtlasRes()
    {
        m_fScaleX = scaleX;
        m_fScaleY = scaleY;
        m_pAtlasManager = pAtlasManager;
        m_pFreeTypeRender = pFreeTypeRender;
        m_pFont = pFont;
        m_nColor = nColor;
        m_sWord = sWord;
        setRestoreFunction(std::bind(&JCWordRes::gpuRestoreRes, this, this));
    }
    JCWordRes::~JCWordRes()
    {
        if (m_pAtlas)
        {
            m_pAtlas->removeInAtlasRes(this);
        }
    }
 
    bool JCWordRes::gpuRestoreRes(JCDisplayRes* pDisplayRes)
    {
        char* pBuff = &(JCFreeTypeFontRender::m_pWordBuff[0]);
        BitmapData kBmp;
        kBmp.m_nBpp = 32;
        kBmp.m_nWidth = MAX_FONT_SIZE + TEXT_SIZE_ALLOWANCE;
        kBmp.m_nHeight = MAX_FONT_SIZE + TEXT_SIZE_ALLOWANCE;
        kBmp.m_pImageData = pBuff;

        if (m_pFreeTypeRender->getTextBitmapData((char*)m_sWord.c_str(), &kBmp, m_nColor, m_pFont, 1.0f, 1.0f))
        {
            m_kRectNoScaled.width = kBmp.m_nWidth;
            m_kRectNoScaled.height = kBmp.m_nHeight;
            kBmp.m_nWidth = MAX_FONT_SIZE + TEXT_SIZE_ALLOWANCE;
            kBmp.m_nHeight = MAX_FONT_SIZE + TEXT_SIZE_ALLOWANCE;
            if (m_pFreeTypeRender->getTextBitmapData((char*)m_sWord.c_str(), &kBmp, m_nColor, m_pFont, m_fScaleX, m_fScaleY))
            {
                m_kRect.width = kBmp.m_nWidth;
                m_kRect.height = kBmp.m_nHeight;
                return m_pAtlasManager->pushData(this, &kBmp);
            }
        }    
        return false;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
