/**
@file			JCJpegImg.cpp
@brief
@author			hugao
@version		1.0
@date			2016_5_11
*/

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "JCImageRW.h"
#include "JCETCImg.h"
#include "../util/JCCommonMethod.h"
#include "../util/Log.h"
#include "../3DRS/JCRenderer.h"
#pragma warning (disable: 4996)
//------------------------------------------------------------------------------
namespace laya
{
    static const char kMagic1[] = { 'P', 'K', 'M', ' ', '1', '0' };
    static const char kMagic2[] = { 'P', 'K', 'M', ' ', '2', '0' };
    static const unsigned int ETC1_PKM_FORMAT_OFFSET = 6;
    static const unsigned int ETC1_PKM_ENCODED_WIDTH_OFFSET = 8;
    static const unsigned int ETC1_PKM_ENCODED_HEIGHT_OFFSET = 10;
    static const unsigned int ETC1_PKM_WIDTH_OFFSET = 12;
    static const unsigned int ETC1_PKM_HEIGHT_OFFSET = 14;

    enum
    {
        ETC1_RGB_NO_MIPMAPS,
        ETC2PACKAGE_RGB_NO_MIPMAPS,
        ETC2PACKAGE_RGBA_NO_MIPMAPS_OLD,
        ETC2PACKAGE_RGBA_NO_MIPMAPS,
        ETC2PACKAGE_RGBA1_NO_MIPMAPS,
        ETC2PACKAGE_R_NO_MIPMAPS,
        ETC2PACKAGE_RG_NO_MIPMAPS,
        ETC2PACKAGE_R_SIGNED_NO_MIPMAPS,
        ETC2PACKAGE_RG_SIGNED_NO_MIPMAPS,
        ETC2PACKAGE_sRGB_NO_MIPMAPS,
        ETC2PACKAGE_sRGBA_NO_MIPMAPS,
        ETC2PACKAGE_sRGBA1_NO_MIPMAPS
    };
#define ETC_PKM_HEADER_SIZE 16

    static void writeBEUint16(unsigned char* pOut, unsigned int data) {
        pOut[0] = (unsigned char)(data >> 8);
        pOut[1] = (unsigned char)data;
    }

    static unsigned int readBEUint16(const unsigned char* pIn) {
        return (pIn[0] << 8) | pIn[1];
    }

    unsigned int etc1_pkm_get_width(const unsigned char* pHeader) {
        return readBEUint16(pHeader + ETC1_PKM_WIDTH_OFFSET);
    }

    unsigned int etc1_pkm_get_height(const unsigned char* pHeader) {
        return readBEUint16(pHeader + ETC1_PKM_HEIGHT_OFFSET);
    }

    bool isPKM1(const char* data, int nLength)
    {
        if (memcmp(data, kMagic1, sizeof(kMagic1))) {
            return false;
        }
        unsigned int format = readBEUint16((unsigned char*)data + ETC1_PKM_FORMAT_OFFSET);
        unsigned int encodedWidth = readBEUint16((unsigned char*)data + ETC1_PKM_ENCODED_WIDTH_OFFSET);
        unsigned int encodedHeight = readBEUint16((unsigned char*)data + ETC1_PKM_ENCODED_HEIGHT_OFFSET);
        unsigned int width = readBEUint16((unsigned char*)data + ETC1_PKM_WIDTH_OFFSET);
        unsigned int height = readBEUint16((unsigned char*)data + ETC1_PKM_HEIGHT_OFFSET);
        return format == ETC1_RGB_NO_MIPMAPS &&
            encodedWidth >= width && encodedWidth - width < 4 &&
            encodedHeight >= height && encodedHeight - height < 4;
    }
    bool isPKM2(const char* data, int nLength)
    {
        if (memcmp(data, kMagic2, sizeof(kMagic2))) {
            return false;
        }
        unsigned int format = readBEUint16((unsigned char*)data + ETC1_PKM_FORMAT_OFFSET);
        return format >= ETC1_RGB_NO_MIPMAPS && format <= ETC2PACKAGE_sRGBA1_NO_MIPMAPS && format != ETC2PACKAGE_RGBA_NO_MIPMAPS_OLD;
    }

    bool isEtc(const char* data, int nLength)
    {
        if (isPKM1(data, nLength))
            return true;
        if (isPKM2(data, nLength))
            return true;
        return false;
    }

    bool LoadETCFromMem(BitmapData* pBitmapData, unsigned  char* data, int size)
    {
        if (!isEtc((const char*)data, size))
        {
            return  false;
        }
        const unsigned char* header = static_cast<const unsigned char*>(data);
        pBitmapData->m_nWidth = etc1_pkm_get_width(header);
        pBitmapData->m_nHeight = etc1_pkm_get_height(header);

        if (0 == pBitmapData->m_nWidth || 0 == pBitmapData->m_nHeight)
        {
            LOGW("Load ETC error: Image height and width must be greater than 0");
            return false;
        }

        if ((!JCRenderer::s_nGLCaps & GLC_NOPT) && (pBitmapData->m_nHeight != mathCeilPowerOfTwo(pBitmapData->m_nHeight) || pBitmapData->m_nWidth != mathCeilPowerOfTwo(pBitmapData->m_nWidth)))
        {
            LOGW("Load ETC warning: Image height or width is not a power of 2, some devices may not support");
        }

        unsigned int format = readBEUint16((unsigned char*)data + ETC1_PKM_FORMAT_OFFSET);
#ifndef GL_ETC1_RGB8_OES
        return false;
#else
        switch (format)
        {
        case ETC1_RGB_NO_MIPMAPS:
            pBitmapData->m_nFormat = GL_ETC1_RGB8_OES;
            break;
        case ETC2PACKAGE_RGB_NO_MIPMAPS:
            pBitmapData->m_nFormat = 0x9274;// GL_COMPRESSED_RGB8_ETC2;
            break;
        case ETC2PACKAGE_RGBA_NO_MIPMAPS:
            pBitmapData->m_nFormat = 0x9278;// GL_COMPRESSED_RGBA8_ETC2_EAC;
            break;
        case ETC2PACKAGE_RGBA1_NO_MIPMAPS:
            pBitmapData->m_nFormat = 0x9276;// GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2;
            break;
        default:
            LOGE("Load ETC error! format %d not supported", format);
            return false;
            break;
        }

        pBitmapData->m_bIsCompressed = true;
        int length = pBitmapData->m_nLength = size - ETC_PKM_HEADER_SIZE;
        pBitmapData->m_pImageData = new char[length * sizeof(char)];
        memcpy(pBitmapData->m_pImageData, data + ETC_PKM_HEADER_SIZE, length);
        return true;
#endif
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
