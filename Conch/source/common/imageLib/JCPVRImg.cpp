/**
@file			JCPngImg.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#include <string.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "../util/Log.h"
#include "../util/JCMarcos.h"
#include "JCPVRImg.h"
#include "JCImageRW.h"
#include <algorithm>
#include <assert.h>
#include "../util/JCCommonMethod.h"
//------------------------------------------------------------------------------
#pragma warning (disable: 4996)
namespace laya
{
#define PVR_TEXTURE_FLAG_TYPE_MASK  0xff
    enum
    {
        kPVRTextureFlagTypePVRTC_2 = 24,
        kPVRTextureFlagTypePVRTC_4
    };
    // Values taken from PVRTexture.h from http://www.imgtec.com
    enum class PVR2TextureFlag
    {
        Mipmap = (1 << 8),        // has mip map levels
        Twiddle = (1 << 9),        // is twiddled
        Bumpmap = (1 << 10),       // has normals encoded for a bump map
        Tiling = (1 << 11),       // is bordered for tiled pvr
        Cubemap = (1 << 12),       // is a cubemap/skybox
        FalseMipCol = (1 << 13),       // are there false colored MIP levels
        Volume = (1 << 14),       // is this a volume texture
        Alpha = (1 << 15),       // v2.1 is there transparency info in the texture
        VerticalFlip = (1 << 16),       // v2.1 is the texture vertically flipped
    };
    enum class PVR3TextureFlag
    {
        PremultipliedAlpha = (1 << 1)    // has premultiplied alpha
    };
    static const char gPVRTexIdentifier[5] = "PVR!";
    bool isPvrV2(const char* pData, int nLength)
    {
        if (static_cast<size_t>(nLength) < sizeof(PVRv2TexHeader))
        {
            return false;
        }

        const PVRv2TexHeader* headerv2 = static_cast<const PVRv2TexHeader*>(static_cast<const void*>(pData));

        return memcmp(&headerv2->pvrTag, gPVRTexIdentifier, strlen(gPVRTexIdentifier)) == 0;
    }
    bool isPvrV3(const char* pData, int nLength)
    {
        if (static_cast<size_t>(nLength) < sizeof(PVRv3TexHeader))
        {
            return false;
        }

        const PVRv3TexHeader* headerv3 = static_cast<const PVRv3TexHeader*>(static_cast<const void*>(pData));

        return LAYA_SWAP_INT32_BIG_TO_HOST(headerv3->version) == 0x50565203;
    }

    /*bool LoadPVRV2FromMem(BitmapData* pBitmapData, unsigned  char * data, int nLength)
    {
        int dataLength = 0, dataOffset = 0, dataSize = 0;
        int blockSize = 0, widthBlocks = 0, heightBlocks = 0;
        int width = 0, height = 0;

        const PVRv2TexHeader *header = static_cast<const PVRv2TexHeader *>(static_cast<const void*>(data));

        if (memcmp(&header->pvrTag, gPVRTexIdentifier, strlen(gPVRTexIdentifier)) != 0)
        {
            return false;
        }

        pBitmapData->m_bIsCompressed = true;
        unsigned int flags = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->flags);
        int bpp = 0;
        if ((flags & PVR_TEXTURE_FLAG_TYPE_MASK) == kPVRTextureFlagTypePVRTC_4)
        {
            pBitmapData->m_nFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
            bpp = 4;
        }
        else if ((flags & PVR_TEXTURE_FLAG_TYPE_MASK) == kPVRTextureFlagTypePVRTC_2)
        {      
            pBitmapData->m_nFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
            bpp = 2;
        }
        else
        {
            LOGE("Load PVR error! invalid pvrv2 format only support GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG and GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG");
            return false;
        }

        bool flipped = (flags & (unsigned int)PVR2TextureFlag::VerticalFlip) ? true : false;
        if (flipped)
        {
            LOGW("Load PVR warning: Image is flipped");
        }

        int _numberOfMipmaps = 0;

        width = pBitmapData->m_nWidth = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->width);
        height = pBitmapData->m_nHeight = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->height);

        if (pBitmapData->m_nWidth != pBitmapData->m_nHeight)
        {
            LOGE("Load PVR error: Image must be square (height==width)");
            return false;
        }

        if (pBitmapData->m_nHeight != mathCeilPowerOfTwo(pBitmapData->m_nHeight) || pBitmapData->m_nWidth != mathCeilPowerOfTwo(pBitmapData->m_nWidth))
        {
            LOGW("Load PVR error: Image height and width must be a power of 2");
            return false;
        }

        dataLength = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->dataLength);

        int _dataLen = nLength - sizeof(PVRv2TexHeader);
        pBitmapData->m_nLength = _dataLen;
        pBitmapData->m_pImageData = new char[_dataLen * sizeof(char)];
        memcpy(pBitmapData->m_pImageData, (unsigned char*)data + sizeof(PVRv2TexHeader), _dataLen);
        pBitmapData->m_vecMipmapData.clear();//多次加载必须先清空
        while (dataOffset < dataLength)
        {
            if (pBitmapData->m_nFormat == GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG)
            {

                blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
                widthBlocks = width / 8;
                heightBlocks = height / 4;
            }
            else
            {
                blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
                widthBlocks = width / 4;
                heightBlocks = height / 4;
            }

            // Clamp to minimum number of blocks
            if (widthBlocks < 2)
            {
                widthBlocks = 2;
            }
            if (heightBlocks < 2)
            {
                heightBlocks = 2;
            }

            dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
            int packetLength = (dataLength - dataOffset);
            packetLength = packetLength > dataSize ? dataSize : packetLength;

            MipmapData mip;
            mip.m_pData = pBitmapData->m_pImageData + dataOffset;
            mip.m_nLength = packetLength;
            pBitmapData->m_vecMipmapData.push_back(mip);

            _numberOfMipmaps++;

            dataOffset += packetLength;

            //Update width and height to the next lower power of two
            width = std::max(width >> 1, 1);
            height = std::max(height >> 1, 1);
        }
        if (_numberOfMipmaps > BitmapData::MIPMAP_MAX)
        {
            LOGW("Load PVR warning! pvrv2 file number of mipmaps is %d max limited is %d ", _numberOfMipmaps, BitmapData::MIPMAP_MAX);
        }
        return true;
    }*/
    /*bool LoadPVRV3FromMem(BitmapData* pBitmapData, unsigned  char * data, int nLength)
    {
        if (static_cast<size_t>(nLength) < sizeof(PVRv3TexHeader))
        {
            return false;
        }

        const PVRv3TexHeader *header = static_cast<const PVRv3TexHeader *>(static_cast<const void*>(data));

        if (LAYA_SWAP_INT32_BIG_TO_HOST(header->version) != 0x50565203)
        {
            LOGE("Load PVR error! invalid pvrv3 file");
            return false;
        }
        
        int flags = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->flags);
        int bpp = 0;
        int format = header->pixelFormat;
        pBitmapData->m_bIsCompressed = true;
        if (format == 0) {
            pBitmapData->m_nFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
            bpp = 2;
        }
        else if (format == 1) {
            pBitmapData->m_nFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
            bpp = 2;
            if (!(flags & (unsigned int)PVR3TextureFlag::PremultipliedAlpha))
            {
                LOGW("Load PVR warning! pvrv3 file must exported premultiplied alpha");
            }
        }
        else if (format == 2) {
            pBitmapData->m_nFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
            bpp = 4;
        }
        else if (format == 3) {
            pBitmapData->m_nFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
            bpp = 4;
            if (!(flags & (unsigned int)PVR3TextureFlag::PremultipliedAlpha))
            {
                LOGW("Load PVR warning! pvrv3 file must exported premultiplied alpha");
            }
        }
        else {
            LOGE("Load PVR error! invalid pvrv3 format only support GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG and GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG");
            return false;
        }

        int numberOfMipmaps = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->numberOfMipmaps);
        if (numberOfMipmaps > BitmapData::MIPMAP_MAX)
        {
            LOGW("Load PVR warning! pvrv3 file number of mipmaps is %d max limited is %d ", numberOfMipmaps, BitmapData::MIPMAP_MAX);
        }

        int width = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->width);
        int height = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->height);
        pBitmapData->m_nWidth = width;
        pBitmapData->m_nHeight = height;

        if (pBitmapData->m_nWidth != pBitmapData->m_nHeight)
        {
            LOGE("Load PVR error: Image must be square (height==width)");
            return false;
        }

        if (pBitmapData->m_nHeight != mathCeilPowerOfTwo(pBitmapData->m_nHeight) || pBitmapData->m_nWidth != mathCeilPowerOfTwo(pBitmapData->m_nWidth))
        {
            LOGW("Load PVR error: Image height and width must be a power of 2");
            return false;
        }

        int dataOffset = 0, dataSize = 0;
        int blockSize = 0, widthBlocks = 0, heightBlocks = 0;

        int _dataLen = nLength - (sizeof(PVRv3TexHeader) + LAYA_SWAP_INT32_LITTLE_TO_HOST(header->metadataLength));
        pBitmapData->m_nLength = _dataLen;
        pBitmapData->m_pImageData = new char[_dataLen * sizeof(char)];
        memcpy(pBitmapData->m_pImageData, static_cast<const unsigned char*>(data) + sizeof(PVRv3TexHeader) + LAYA_SWAP_INT32_LITTLE_TO_HOST(header->metadataLength), _dataLen);
        pBitmapData->m_vecMipmapData.clear();//多次加载必须先清空
        pBitmapData->m_vecMipmapData.resize(numberOfMipmaps);
        for (int i = 0; i < numberOfMipmaps; i++)
        {
            if (pBitmapData->m_nFormat == GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG || pBitmapData->m_nFormat == GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG)
            {
                blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
                widthBlocks = width / 8;
                heightBlocks = height / 4;
            }
            else
            {
                blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
                widthBlocks = width / 4;
                heightBlocks = height / 4;
            }
            if (widthBlocks < 2)
            {
                widthBlocks = 2;
            }
            if (heightBlocks < 2)
            {
                heightBlocks = 2;
            }

            dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
            auto packetLength = _dataLen - dataOffset;
            packetLength = packetLength > dataSize ? dataSize : packetLength;

            pBitmapData->m_vecMipmapData[i].m_pData = pBitmapData->m_pImageData + dataOffset;
            pBitmapData->m_vecMipmapData[i].m_nLength = static_cast<int>(packetLength);

            dataOffset += packetLength;
            assert(dataOffset <= _dataLen && "pvr: invalid length");

            width = std::max(width >> 1, 1);
            height = std::max(height >> 1, 1);
        }
        return true;
    }*/
    enum class PVR3TexturePixelFormat : uint64_t
    {
        PVRTC2BPP_RGB = 0ULL,
        PVRTC2BPP_RGBA = 1ULL,
        PVRTC4BPP_RGB = 2ULL,
        PVRTC4BPP_RGBA = 3ULL,
        PVRTC2_2BPP_RGBA = 4ULL,
        PVRTC2_4BPP_RGBA = 5ULL,
        ETC1 = 6ULL,
        DXT1 = 7ULL,
        DXT2 = 8ULL,
        DXT3 = 9ULL,
        DXT4 = 10ULL,
        DXT5 = 11ULL,
        BC1 = 7ULL,
        BC2 = 9ULL,
        BC3 = 11ULL,
        BC4 = 12ULL,
        BC5 = 13ULL,
        BC6 = 14ULL,
        BC7 = 15ULL,
        UYVY = 16ULL,
        YUY2 = 17ULL,
        BW1bpp = 18ULL,
        R9G9B9E5 = 19ULL,
        RGBG8888 = 20ULL,
        GRGB8888 = 21ULL,
        ETC2_RGB = 22ULL,
        ETC2_RGBA = 23ULL,
        ETC2_RGBA1 = 24ULL,
        EAC_R11_Unsigned = 25ULL,
        EAC_R11_Signed = 26ULL,
        EAC_RG11_Unsigned = 27ULL,
        EAC_RG11_Signed = 28ULL,

        BGRA8888 = 0x0808080861726762ULL,
        RGBA8888 = 0x0808080861626772ULL,
        RGBA4444 = 0x0404040461626772ULL,
        RGBA5551 = 0x0105050561626772ULL,
        RGB565 = 0x0005060500626772ULL,
        RGB888 = 0x0008080800626772ULL,
        A8 = 0x0000000800000061ULL,
        L8 = 0x000000080000006cULL,
        LA88 = 0x000008080000616cULL,
    };
    struct MetaDataBlock
    {
        uint32_t _fourCC;
        uint32_t  _key;
        uint32_t  _dataSize;
    };
    const MetaDataBlock* findMetaData(unsigned char* pvrData, int metadataLength, uint32_t fourCC, uint32_t  key)
    {
        int offset = 0;
        while (offset < metadataLength)
        {
            const MetaDataBlock *layaMetaData = static_cast<const MetaDataBlock*>(static_cast<const void*>(pvrData + sizeof(PVRv3TexHeader) + offset));
            uint32_t _fourCC = LAYA_SWAP_INT32_LITTLE_TO_HOST(layaMetaData->_fourCC);
            uint32_t  _key = LAYA_SWAP_INT32_LITTLE_TO_HOST(layaMetaData->_key);
            uint32_t  _dataSize = LAYA_SWAP_INT32_LITTLE_TO_HOST(layaMetaData->_dataSize);
            if (fourCC == _fourCC && _key == key)
            {
                return layaMetaData;
            }
            offset += sizeof(MetaDataBlock) + layaMetaData->_dataSize;
        }
        return nullptr;
    }
    const uint32_t PVRv3LayaMetaData_fourCC = 0x4c415941;
    const int PVRv3LayaMetaData_key = 0;
    const int PVRv3LayaMetaData_CustomnString_key = 1;
    const int PVRv3LayaMetaData_dataSize = 4 * 4;
    bool LoadPVRV3NewFromMem(BitmapData* pBitmapData, unsigned  char * data, int nLength)
    {
        if (static_cast<size_t>(nLength) < sizeof(PVRv3TexHeader))
        {
            return false;
        }

        const PVRv3TexHeader *header = static_cast<const PVRv3TexHeader *>(static_cast<const void*>(data));

        if (LAYA_SWAP_INT32_BIG_TO_HOST(header->version) != 0x50565203)
        {
            LOGE("Load PVR error! invalid pvrv3 file");
            return false;
        }

        int flags = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->flags);
        
        PVR3TexturePixelFormat pixelFormat = static_cast<PVR3TexturePixelFormat>(header->pixelFormat);
        pBitmapData->m_bIsCompressed = true;

        bool bIsPVR = false;
        switch ((PVR3TexturePixelFormat)pixelFormat)
        {
        case PVR3TexturePixelFormat::PVRTC2BPP_RGB:
            pBitmapData->m_nFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG; bIsPVR = true;
            break;
        case PVR3TexturePixelFormat::PVRTC2BPP_RGBA:
            pBitmapData->m_nFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG; bIsPVR = true;
            break;
        case PVR3TexturePixelFormat::PVRTC4BPP_RGB:
            pBitmapData->m_nFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG; bIsPVR = true;
            break;
        case PVR3TexturePixelFormat::PVRTC4BPP_RGBA:
            pBitmapData->m_nFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG; bIsPVR = true;
            break;
        case PVR3TexturePixelFormat::ETC1:
            pBitmapData->m_nFormat = 0x8D64;// GL_ETC1_RGB8_OES;
            break;
        case PVR3TexturePixelFormat::ETC2_RGB:
            pBitmapData->m_nFormat = 0x9274;// GL_COMPRESSED_RGB8_ETC2;
            break;
        case PVR3TexturePixelFormat::ETC2_RGBA:
            pBitmapData->m_nFormat = 0x9278;// GL_COMPRESSED_RGBA8_ETC2_EAC;
            break;
        case PVR3TexturePixelFormat::ETC2_RGBA1:
            pBitmapData->m_nFormat = 0x9276;// GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2;
            break;
        default:
            LOGE("Load PVR error! format %d not supported", pixelFormat);
            return false;
            break;
        }
        if (!(flags & (unsigned int)PVR3TextureFlag::PremultipliedAlpha))
        {
            LOGW("Load PVR warning! pvrv3 file must exported premultiplied alpha");
        }

        int numberOfMipmaps = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->numberOfMipmaps);
        if (numberOfMipmaps > 1)
        {
            LOGW("Load PVR error! mipmaps is not supported");
            return false;
        }

        int width = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->width);
        int height = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->height);

        if (bIsPVR && width != height)
        {
            LOGW("Load PVR warning: PVR Image must be square (height==width)");
        }

        if (height != mathCeilPowerOfTwo(height) || width != mathCeilPowerOfTwo(width))
        {
            LOGW("Load PVR warning: Compress Image height and width must be a power of 2, or some deices may not supported");
        }

        int metadataLength = LAYA_SWAP_INT32_LITTLE_TO_HOST(header->metadataLength);

        pBitmapData->m_nWidth = width;
        pBitmapData->m_nHeight = height;
        pBitmapData->m_nOffsetX = 0.0f;
        pBitmapData->m_nOffsetY = 0.0f; 
        pBitmapData->m_nTextureWidth = width;
        pBitmapData->m_nTextureHeight = height;
       

        {
            const MetaDataBlock* metaData = findMetaData(data, metadataLength, PVRv3LayaMetaData_fourCC, PVRv3LayaMetaData_key);
            {
                const PVRv3LayaMetaData *layaMetaData = (const PVRv3LayaMetaData*)metaData;
                pBitmapData->m_nOffsetX = LAYA_SWAP_INT32_LITTLE_TO_HOST(layaMetaData->x);
                pBitmapData->m_nOffsetY = LAYA_SWAP_INT32_LITTLE_TO_HOST(layaMetaData->y);
                pBitmapData->m_nWidth = LAYA_SWAP_INT32_LITTLE_TO_HOST(layaMetaData->width);
                pBitmapData->m_nHeight = LAYA_SWAP_INT32_LITTLE_TO_HOST(layaMetaData->height);
            }
        }
        {
            const MetaDataBlock* metaData = findMetaData(data, metadataLength, PVRv3LayaMetaData_fourCC, PVRv3LayaMetaData_CustomnString_key);
            if (metaData != nullptr)
            {
                char* data = (char*)metaData + sizeof(MetaDataBlock);
                pBitmapData->m_pUserData = new char[metaData->_dataSize];
                memcpy(pBitmapData->m_pUserData, data, metaData->_dataSize);
            }
        }

        int texDataOffset = sizeof(PVRv3TexHeader) + metadataLength;
        int texDataLength = pBitmapData->m_nLength = nLength - texDataOffset;
        pBitmapData->m_pImageData = new char[texDataLength * sizeof(char)];
        memcpy(pBitmapData->m_pImageData, static_cast<const unsigned char*>(data) + texDataOffset, texDataLength);
        return true;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
