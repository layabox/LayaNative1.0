/**
@file			JCFreeTypeRender.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_11
*/

#include "JCFreeTypeRender.h"
#include "../util/Log.h"
#include "../fileSystem/JCFileSystem.h"
#include "../util/JCLayaUrl.h"
#include <thread>
#include "../util/JCCommonMethod.h"

namespace laya
{
    unsigned short JCFreeTypeFontRender::m_pWordBmpBorder[4 * (MAX_FONT_SIZE + TEXT_SIZE_ALLOWANCE) * (MAX_FONT_SIZE + TEXT_SIZE_ALLOWANCE)];
    char JCFreeTypeFontRender::m_pWordBuff[4 * (MAX_FONT_SIZE + TEXT_SIZE_ALLOWANCE) * (MAX_FONT_SIZE + TEXT_SIZE_ALLOWANCE)];
    JCFreeTypeFontRender::JCFreeTypeFontRender()
    {
        m_pFTlibrary = NULL;
        m_nFontSizeW = 0;
        m_nFontSizeH = 0;
        m_nDevDPIX = 72;
        m_nDevDPIY = 72;
        m_fItalicsValue = 0;
        m_pCurrentFT = NULL;
        m_pIOSFTInterface = NULL;
        FT_Init_FreeType(&m_pFTlibrary);
    }
    JCFreeTypeFontRender::~JCFreeTypeFontRender()
    {
        clearCustomFont();
        clearDefaultFont();
        FT_Done_FreeType(m_pFTlibrary);
        m_pFTlibrary = NULL;
    }
    void JCFreeTypeFontRender::clearCustomFont()
    {
        for (MapFTFace::iterator iter = m_vCustomMadeFT.begin(); iter != m_vCustomMadeFT.end(); iter++)
        {
            delete iter->second;
        }
        m_vCustomMadeFT.clear();
    }
    void JCFreeTypeFontRender::clearDefaultFont()
    {
        for (int i = 0, n = m_vDefaultFT.size(); i < n; i++)
        {
            delete m_vDefaultFT[i];
        }
        m_vDefaultFT.clear();
    }
    bool JCFreeTypeFontRender::initDefaultFont(const char* sDefaultTTFs)
    {
        if (m_vDefaultFT.size() > 0)return true;
        
        if( strlen(sDefaultTTFs) > 0 )
        {
            std::vector<char*> vVector;
            splitString(vVector, (char*)sDefaultTTFs,'|');
            for (int i = 0, n = vVector.size(); i < n; i++)
            {
                FT_Face pFTFace = getFTFaceFromFile(vVector[i]);
                if ( pFTFace == NULL)
                {
                    return false;
                }
                else
                {
                    m_vDefaultFT.push_back(new FTFaceRecord(pFTFace));
                }
            }
        }
        else
        {
        #ifdef __APPLE__
            std::string sPath = getIOSFontTTFPath();
            FT_Face pFTFace = getFTFaceFromFile(sPath.c_str());
            if ( pFTFace == NULL )
            {
                sPath = writeIOSFontTTF();
                pFTFace = getFTFaceFromFile(sPath.c_str());
                if ( pFTFace == NULL )
                {
                    return false;
                }
                else
                {
                    m_vDefaultFT.push_back(new FTFaceRecord(pFTFace));
                }
            }
            else
            {
                m_vDefaultFT.push_back(new FTFaceRecord(pFTFace));
            }
        #endif
        }
        return true;
    }
    bool JCFreeTypeFontRender::initDefaultFont(char* pBuffer, int nBuferLen)
    {
        clearDefaultFont();
        //arrayBuffer will be GC , so copy
        char* copyBuffer = new char[nBuferLen];
        memcpy(copyBuffer, pBuffer, nBuferLen);
        FT_Face pFTFace = getFTFaceFromBuffer(copyBuffer, nBuferLen);
        if (pFTFace == NULL)
        {
            delete[] copyBuffer;
            return false;
        }
        else
        {
            m_vDefaultFT.push_back(new FTFaceRecord(pFTFace, copyBuffer));
        }
        return true;
    }
    FT_Face JCFreeTypeFontRender::getFTFaceFromBuffer(char* pBuffer, int nBuferLen)
    {
        FT_Error nError = 0;
        FT_Face pFTFace = NULL;
        nError = FT_New_Memory_Face(m_pFTlibrary, (const FT_Byte*)pBuffer, nBuferLen, 0, &pFTFace);
        if (nError == FT_Err_Unknown_File_Format) 
        {
            return NULL;
        }
        else if (nError)
        {
            return NULL;
        }
        FT_Select_Charmap(pFTFace, FT_ENCODING_UNICODE);
        return pFTFace;
    }
    FT_Face JCFreeTypeFontRender::getFTFaceFromFile(const char* sFileName)
    {
        FT_Face pFTFace = NULL;
        int nError = FT_New_Face(m_pFTlibrary, sFileName, 0, &pFTFace);
        if (nError)
        {
            pFTFace = NULL;
        }
        FT_Select_Charmap(pFTFace, FT_ENCODING_UNICODE);
        return pFTFace;
    }
    bool JCFreeTypeFontRender::setFont(const char* sFontName) 
    {
        m_pCurrentFT = NULL;
        MapFTFace::iterator iter = m_vCustomMadeFT.find(sFontName);
        if (iter != m_vCustomMadeFT.end())
        {
            m_pCurrentFT = iter->second->face;
            return true;
        }
        return false;
    }
	void JCFreeTypeFontRender::setItalics(float fValue)
    {
        if (m_fItalicsValue == fValue)return;
		m_fItalicsValue = fValue;
        FT_Matrix kMatrix;
        kMatrix.xx = 0x10000L;
        kMatrix.xy = (FT_Fixed)(m_fItalicsValue * 0x10000L);
        kMatrix.yx = 0;
        kMatrix.yy = 0x10000L;
        if (m_pCurrentFT)
        {
            FT_Set_Transform(m_pCurrentFT, &kMatrix, 0);
        }
        for (int i = 0, n = m_vDefaultFT.size(); i < n; i++)
        {
            FT_Set_Transform(m_vDefaultFT[i]->face, &kMatrix, 0);
        }
	}
    void JCFreeTypeFontRender::setFontSize(int nWidth, int nHeight)
    {
        int nW = nWidth << 6;
        int nH = nHeight << 6;
        if (m_nFontSizeW == nW && m_nFontSizeH == nH)
        {
            //避免两种字体，第一种设置了size  切换到第二种字体的时候，没有设置就返回了。
            if (m_pCurrentFT)
            {
                FT_Set_Char_Size(m_pCurrentFT, m_nFontSizeW, m_nFontSizeH, m_nDevDPIX, m_nDevDPIY);
            }
            return;
        }
        m_nFontSizeW = nW;
        m_nFontSizeH = nH;
        if (m_pCurrentFT)
        {
            FT_Set_Char_Size(m_pCurrentFT, m_nFontSizeW, m_nFontSizeH, m_nDevDPIX, m_nDevDPIY);
        }
        for (int i = 0, n = m_vDefaultFT.size(); i < n; i++)
        {
            FT_Set_Char_Size(m_vDefaultFT[i]->face, m_nFontSizeW, m_nFontSizeH, m_nDevDPIX, m_nDevDPIY);
        }
    }
	void JCFreeTypeFontRender::getMetric( int nUnicode, int& nWidth, int& nHeight )
    {
        FT_UInt nFoundIndex = 0;
        FT_Face pFTFace = m_pCurrentFT;
        //先去看看用户设置的字体中是否存在
        if (pFTFace)
        {
            nFoundIndex = FT_Get_Char_Index(pFTFace, nUnicode);
        }
		if(nFoundIndex==0)
        {
			for( int i=0,n= m_vDefaultFT.size(); i<n; i++)
            {
                nFoundIndex = FT_Get_Char_Index(m_vDefaultFT[i]->face, nUnicode);
				if(nFoundIndex>0)
                {
                    pFTFace = m_vDefaultFT[i]->face;
					break;
				}
			}
		}
		if(nFoundIndex ==0)
        {
			LOGW("JCFreeTypeFontRender::getMetric Cannot find the specified character in all fonts:%x", nUnicode);
            nWidth = nHeight = 0;
			return;
		}
		FT_Error kFTError = FT_Load_Glyph(pFTFace, nFoundIndex, FT_LOAD_RENDER|FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP);
		if(kFTError ==36)
        {
			LOGE("JCFreeTypeFontRender::getMetric Not set font size yet");
            nWidth = nHeight = 0;
		}
		FT_GlyphSlot pGlyph = pFTFace->glyph;
		FT_Bitmap& kBitmap = pGlyph->bitmap;
        nWidth = pGlyph->bitmap_left + kBitmap.width;
        nHeight = (pFTFace->size->metrics.ascender- pFTFace->size->metrics.descender)>>6;
	}
	void JCFreeTypeFontRender::getBitmapData(int nUnicode, unsigned char*& pBitmap, int& nWidth, int& nHeight, int& nDataW, int& nDataH, int& nDataLeft, int& nDataTop, int& nUnderlineTop, int& nUnderlineHeight)
    {
        FT_UInt nFoundIndex = 0;
        FT_Face pFTFace = m_pCurrentFT;
        if (pFTFace)
        {
            nFoundIndex = FT_Get_Char_Index(pFTFace, nUnicode);
        }
        if (nFoundIndex == 0)
        {
            for (int i = 0, n = m_vDefaultFT.size(); i < n; i++)
            {
                nFoundIndex = FT_Get_Char_Index(m_vDefaultFT[i]->face, nUnicode);
                if (nFoundIndex > 0)
                {
                    pFTFace = m_vDefaultFT[i]->face;
                    break;
                }
            }
        }
		if(nFoundIndex ==0)
        {
			LOGW("JCFreeTypeFontRender::getBitmapData Cannot find the specified character in all fonts:%x",nUnicode);
            pBitmap =NULL;
			nWidth=nHeight=nDataW=nDataH=nDataLeft=nDataTop=0;
			return;
		}

        FT_Error kFTError = FT_Load_Glyph(pFTFace, nFoundIndex, FT_LOAD_RENDER | FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP);
        if (kFTError == 36)
        {
            LOGE("JCFreeTypeFontRender::getBitmapData Not set font size yet");
            pBitmap = NULL;
            nWidth = nHeight = nDataW = nDataH = nDataLeft = nDataTop = 0;
        }
        FT_GlyphSlot pGlyph = pFTFace->glyph;
		if( pGlyph->format != FT_GLYPH_FORMAT_BITMAP )
        {
            FT_Render_Glyph(pGlyph, FT_RENDER_MODE_NORMAL);	
		}
		FT_Bitmap& kBitmap = pGlyph->bitmap;
		nDataLeft= pGlyph->bitmap_left;
		nDataTop = (pFTFace->size->metrics.ascender>>6) - pGlyph->bitmap_top;
		nWidth = pGlyph->bitmap_left+ kBitmap.width;
		nHeight = (pFTFace->size->metrics.ascender- pFTFace->size->metrics.descender)>>6;
		nDataW = kBitmap.width;
		nDataH = kBitmap.rows;
        //pFTFace->size->metrics.ascender;
        //pFTFace->size->metrics.descender;
  
        //下划线
        nUnderlineHeight = static_cast<int16_t>(Convert26Dot6ToRoundedPixel<int32_t>(FT_MulFix(pFTFace->underline_thickness, pFTFace->size->metrics.y_scale)));
        nUnderlineHeight = nUnderlineHeight > 0 ? nUnderlineHeight : 1;
        int16_t OutUnderlinePos = static_cast<int16_t>(Convert26Dot6ToRoundedPixel<int32_t>(FT_MulFix(pFTFace->underline_position, pFTFace->size->metrics.y_scale)));
        nUnderlineTop = static_cast<int16_t>(Convert26Dot6ToRoundedPixel<int32_t>(pFTFace->size->metrics.ascender)) - OutUnderlinePos - (int)(0.5f * nUnderlineTop);

        pBitmap = new unsigned char [kBitmap.rows*kBitmap.width];
		if(kBitmap.width== kBitmap.pitch)
        {
			memcpy(pBitmap, kBitmap.buffer, kBitmap.rows*kBitmap.width);
		}
        else
        {
			for( int y=0; y<kBitmap.rows; y++)
            {
				memcpy(&pBitmap[y*kBitmap.width], &kBitmap.buffer[y*kBitmap.pitch], kBitmap.width);
			}
		}
	}
    bool JCFreeTypeFontRender::setFontFaceFromUrl(const char* sFontFamily, const char* sTTFFileName)
    {
        m_kLoadGlyphLock.lock();
        bool bRet = false;
        FT_Face pFTFace = getFTFaceFromFile(sTTFFileName);
        if (pFTFace)
        {
            MapFTFace::iterator iter = m_vCustomMadeFT.find(sFontFamily);
            if (iter!= m_vCustomMadeFT.end())
            {
                delete iter->second;
            }
            m_vCustomMadeFT[sFontFamily] = new FTFaceRecord(pFTFace);
            bRet = true;
        }
        m_kLoadGlyphLock.unlock();
        return bRet;
    }
    bool JCFreeTypeFontRender::setFontFaceFromBuffer(const char* sFontFamily, char* pBuffer, int nBuferLen)
    {
        m_kLoadGlyphLock.lock();
        bool bRet = false;
        //arrayBuffer will be GC , so copy
        char* copyBuffer = new char[nBuferLen];
        memcpy(copyBuffer, pBuffer, nBuferLen);
        FT_Face pFTFace = getFTFaceFromBuffer(copyBuffer, nBuferLen);
        if (pFTFace)
        {
            MapFTFace::iterator iter = m_vCustomMadeFT.find(sFontFamily);
            if (iter != m_vCustomMadeFT.end())
            {
                delete iter->second;
            }
            m_vCustomMadeFT[sFontFamily] = new FTFaceRecord(pFTFace, copyBuffer);
            bRet = true;
        }
        m_kLoadGlyphLock.unlock();
        return bRet;
    }
    bool JCFreeTypeFontRender::removeFont(const char* sFontFamily)
    {
        m_kLoadGlyphLock.lock();
        bool bRet = false;
        MapFTFace::iterator iter = m_vCustomMadeFT.find(sFontFamily);
        if( iter != m_vCustomMadeFT.end() )
        {
            delete iter->second;
            iter = m_vCustomMadeFT.erase(iter);
            bRet = true;
        }
        m_kLoadGlyphLock.unlock();
        return bRet;
    }
    void JCFreeTypeFontRender::setIOSFTInterface(JCIOSFTInterface* pIOSFTInterface)
    {
        m_pIOSFTInterface = pIOSFTInterface;
    }
    void JCFreeTypeFontRender::measureText( char* p_sWord, JCFontInfo* p_pFontInfo, int& p_nW, int& p_nH)
    {
        if (!p_sWord)
        {
            p_nW = p_nH = 0;
            return;
        }
        int pA = 0;
        static unsigned short ucStr[8];
        int nResultLen = UTF8StrToUnicodeStr((unsigned char*)p_sWord, ucStr, 8);
        if (nResultLen > 1)
        {
            measureTexts(p_sWord, p_pFontInfo, p_nW, p_nH);
            return;
        }
        if (nResultLen)
        {
            int nWord = ucStr[0];
            m_kLoadGlyphLock.lock();
            setFont(p_pFontInfo->m_sFamily);
            setFontSize(p_pFontInfo->m_nFontSize, 0);
            getMetric(nWord, p_nW, p_nH);

            int nBorderSize = p_pFontInfo->m_nBorderSize > TEXT_SIZE_ALLOWANCE / 2 ? TEXT_SIZE_ALLOWANCE / 2 : p_pFontInfo->m_nBorderSize;
            p_nW += nBorderSize * 2;
            p_nH += nBorderSize * 2;

            m_kLoadGlyphLock.unlock();
        }
    }
    void JCFreeTypeFontRender::measureTexts(char* p_sWord, JCFontInfo* p_pFontInfo, int& p_nW, int& p_nH)
    {
        if (!p_sWord)
        {
            p_nW = p_nH = 0;
            return;
        }
        int pA = 0;
        int nWord;
        int wordlen = strlen(p_sWord);
        m_kLoadGlyphLock.lock();
        setFont(p_pFontInfo->m_sFamily);
        setFontSize(p_pFontInfo->m_nFontSize, 0);
        int maxHeight = 0;
        int width = 0;
        while (true)
        {
            int ulen = UTF8ToUnicode((unsigned char*)p_sWord, &nWord);
            wordlen -= ulen;
            getMetric(nWord, p_nW, p_nH);
            pA += p_nW;
            width += p_nW;

            int nBorderSize = p_pFontInfo->m_nBorderSize > TEXT_SIZE_ALLOWANCE / 2 ? TEXT_SIZE_ALLOWANCE / 2 : p_pFontInfo->m_nBorderSize;
            width += nBorderSize * 2;
            p_nH += nBorderSize * 2;

            if (p_nH > maxHeight) maxHeight = p_nH;
            if (wordlen <= 0)
                break;
            p_sWord += ulen;
        }
        p_nW = width;
        p_nH = maxHeight;
        m_kLoadGlyphLock.unlock();
    }

    bool JCFreeTypeFontRender::getTextBitmapData(char* str, BitmapData* bitmapData, int nColor, JCFontInfo* fontInfo, float scaleX, float scaleY)
    {
        unsigned char firstv = str[0];
        if (firstv == '\r' || firstv == '\n')
        {
            bitmapData->m_nWidth = bitmapData->m_nHeight = 0;
            return true;
        }
        int nInBuffLen = bitmapData->m_nWidth*bitmapData->m_nHeight;
        int nTextWidth = 0;
        int nTextHeight = 0;
        int dataw = 0, datah = 0;
        int datal = 0, datat = 0;
        unsigned char* pBmp = NULL;
        static unsigned short ucStr[8];
        int nUnderlineTop = 0;
        int nUnderlineHeight = 0;
        int nResultLen = UTF8StrToUnicodeStr((unsigned char*)str, ucStr, 8);
        if (nResultLen)
        {
            int nWord = ucStr[0];
            m_kLoadGlyphLock.lock();
            setFont(fontInfo->m_sFamily);
            setFontSize(ceilf(fontInfo->m_nFontSize * scaleX), ceilf(fontInfo->m_nFontSize * scaleY));
            getBitmapData(nWord, pBmp, nTextWidth, nTextHeight, dataw, datah, datal, datat, nUnderlineTop, nUnderlineHeight);
            m_kLoadGlyphLock.unlock();
        }
        datal = datal > 0 ? datal : 0;
        datat = datat > 0 ? datat : 0;
        if (nTextWidth<dataw + datal)
        {
            nTextWidth = dataw + datal;
        }
        if (nTextHeight<datah + datat)
        {
            nTextHeight = datah + datat;
        }
        int nBorderSize = fontInfo->m_nBorderSize > TEXT_SIZE_ALLOWANCE / 2 ? TEXT_SIZE_ALLOWANCE / 2 : fontInfo->m_nBorderSize;
        nTextWidth += nBorderSize * 2;
        nTextHeight += nBorderSize * 2;
        if (nTextHeight*nTextWidth>nInBuffLen)
        {
            LOGW("JCFreeTypeFontRender::getTextBitmapData the buffersize is insufficient.");
            delete pBmp;
            return false;
        }
        unsigned int* pBuff = (unsigned int*)bitmapData->m_pImageData;//  new unsigned int [nTextWidth*nTextHeight];
        memset(pBuff, 0, nTextWidth*nTextHeight * 4);//TODO 为了效率这个能去掉么
        if (nBorderSize > 0)
        {
            //这里先把描边绘制好
            int nW = dataw + nBorderSize * 2;
            int nH = datah + nBorderSize * 2;
            unsigned short* pBorderBmp = m_pWordBmpBorder;
            memset(pBorderBmp, 0, nW * nH * sizeof(uint64_t));
            for (int y = 0; y < datah; y++)
            {
                for (int x = 0; x < dataw; x++)
                {
                    unsigned char bmpColor = pBmp[y*dataw + x];
                    //8个方向扩充
                    pBorderBmp[y * nW + x] += bmpColor;//左上
                    pBorderBmp[y * nW + x + nBorderSize] += bmpColor;//上
                    pBorderBmp[y * nW + x + nBorderSize * 2] += bmpColor;//右上
                    pBorderBmp[(y + nBorderSize)*nW + x] += bmpColor;//左中
                    pBorderBmp[(y + nBorderSize)*nW + x + nBorderSize * 2] += bmpColor;//右中
                    pBorderBmp[(y + nBorderSize * 2)*nW + x] += bmpColor;//左下
                    pBorderBmp[(y + nBorderSize * 2)*nW + x + nBorderSize] += bmpColor;//下
                    pBorderBmp[(y + nBorderSize * 2)*nW + x + nBorderSize * 2] += bmpColor;//右下
                }
            }
            {
                int nBB = fontInfo->m_nBorderColor & 0xff;
                int nBG = (fontInfo->m_nBorderColor >> 8) & 0xff;
                int nBR = (fontInfo->m_nBorderColor >> 16) & 0xff;
                for (int y = 0; y < nH; y++)
                {
                    for (int x = 0; x < nW; x++)
                    {
                        uint64_t bmpColor = pBorderBmp[x + y*nW];
                        bmpColor = bmpColor > 255 ? 255 : bmpColor;
                        float fAlpha = bmpColor / 255.0f;
                        int b = nBB * fAlpha;
                        int g = nBG * fAlpha;
                        int r = nBR * fAlpha;
                        pBuff[x + datal + (y + datat)*nTextWidth] = (bmpColor << 24) + (b << 16) + (g << 8) + r;
                    }
                }
            }

            //文字本身的颜色
            int nTB = nColor & 255;
            int nTG = (nColor >> 8) & 255;
            int nTR = (nColor >> 16) & 255;
            //文字描边的颜色
            int nBB = fontInfo->m_nBorderColor & 0xff;
            int nBG = (fontInfo->m_nBorderColor >> 8) & 0xff;
            int nBR = (fontInfo->m_nBorderColor >> 16) & 0xff;
            for (int y = 0; y < datah; y++)
            {
                for (int x = 0; x < dataw; x++)
                {
                    int alpha = (int)(pBmp[x + y*dataw]);
                    float fAlpha = alpha / 255.0f;
                    int nPos = (y + datat + nBorderSize) * nTextWidth + x + nBorderSize + datal;
                    if (alpha > 0)
                    {
                        int r = (int)(nTR * fAlpha + nBR * (1 - fAlpha));
                        int g = (int)(nTG * fAlpha + nBG * (1 - fAlpha));
                        int b = (int)(nTB * fAlpha + nBB * (1 - fAlpha));
                        pBuff[nPos] = (255 << 24) + (b << 16) + (g << 8) + r;
                    }
                }
            }
            //下划线
            if (fontInfo->m_nDecorationLine == 1)
            {
                int nLineColor = fontInfo->m_nDecorationColor;
                unsigned char* pLineColor = (unsigned char*)&nLineColor;
                std::swap(pLineColor[0], pLineColor[2]);
                int nLineTop = nUnderlineTop + nBorderSize*2;
                for (int y = 0; y < nUnderlineHeight; y++) {
                    for (int x = 0; x < nTextWidth; x++) {
                        pBuff[x + (y + nLineTop)*nTextWidth] = *(int*)(pLineColor);
                    }
                }
                if (nLineTop + nUnderlineHeight > nTextHeight)
                {
                    nTextHeight = nUnderlineTop + nUnderlineHeight;
                }
            }
        }
        else
        {
            int nTB = nColor & 255;
            int nTG = (nColor >> 8) & 255;
            int nTR = (nColor >> 16) & 255;
            for (int y = 0; y < datah; y++) {
                for (int x = 0; x < dataw; x++) {
                    unsigned char a = pBmp[x + y*dataw];
                    float fAlpha = a / 255.0f;
                    int b = nTB * fAlpha;
                    int g = nTG * fAlpha;
                    int r = nTR * fAlpha;
                    pBuff[x + datal + (y + datat)*nTextWidth] = (a << 24) + (b << 16) + (g << 8) + r;
                }
            }
            //下划线
            if (fontInfo->m_nDecorationLine == 1)
            {
                int nLineColor = fontInfo->m_nDecorationColor;
                unsigned char* pLineColor = (unsigned char*)&nLineColor;
                std::swap(pLineColor[0], pLineColor[2]);
                for (int y = 0; y < nUnderlineHeight; y++) {
                    for (int x = 0; x < nTextWidth; x++) {
                        pBuff[x + (y + nUnderlineTop)*nTextWidth] = *(int*)(pLineColor);
                    }
                }
                if (nUnderlineTop + nUnderlineHeight > nTextHeight)
                {
                    nTextHeight = nUnderlineTop + nUnderlineHeight;
                }
            }
        }
        bitmapData->m_nWidth = nTextWidth;
        bitmapData->m_nHeight = nTextHeight;
        bitmapData->m_bPremultiplyAlpha = true;//防止多次预乘
        delete[] pBmp;
        pBmp = NULL;
        return true;
    }
    std::string JCFreeTypeFontRender::getIOSFontTTFPath()
    {
        return m_pIOSFTInterface->getIOSFontTTFPath();
    }
    std::string JCFreeTypeFontRender::writeIOSFontTTF()
    {
        return m_pIOSFTInterface->writeIOSFontTTF();
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
