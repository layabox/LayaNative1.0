/**
@file			JCPVRImg.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#ifndef __JCPVRImg_H__
#define __JCPVRImg_H__

#include <memory>
#include <functional>
#include "../3DRS/JCGL.h"

namespace laya
{
    typedef struct _PVRTexHeader
    {
        unsigned int headerLength;
        unsigned int height;
        unsigned int width;
        unsigned int numMipmaps;
        unsigned int flags;
        unsigned int dataLength;
        unsigned int bpp;
        unsigned int bitmaskRed;
        unsigned int bitmaskGreen;
        unsigned int bitmaskBlue;
        unsigned int bitmaskAlpha;
        unsigned int pvrTag;
        unsigned int numSurfs;
    } PVRv2TexHeader;

#ifdef WIN32
#pragma pack(push,1)
#endif
    typedef struct
    {
        uint32_t version;
        uint32_t flags;
        uint64_t pixelFormat;
        uint32_t colorSpace;
        uint32_t channelType;
        uint32_t height;
        uint32_t width;
        uint32_t depth;
        uint32_t numberOfSurfaces;
        uint32_t numberOfFaces;
        uint32_t numberOfMipmaps;
        uint32_t metadataLength;
#ifdef WIN32
    } PVRv3TexHeader;
#pragma pack(pop)
#else
} __attribute__((packed)) PVRv3TexHeader;
#endif


#ifdef WIN32
#pragma pack(push,1)
#endif
typedef struct
{
    uint32_t _fourCC;     // LAYA
    uint32_t  _key;       // 0
    uint32_t  _dataSize;  // sizeof(rect)
    uint32_t x;           // uint8_t*  _data; rect
    uint32_t y;
    uint32_t width;
    uint32_t height;
#ifdef WIN32
} PVRv3LayaMetaData;
#pragma pack(pop)
#else
} __attribute__((packed)) PVRv3LayaMetaData;
#endif

bool isPvrV2(const char* pData, int nLength);
bool isPvrV3(const char* pData, int nLength);
}
//------------------------------------------------------------------------------


#endif //__JCPVRImg_H__

//-----------------------------END FILE--------------------------------
