/**
@file			JCPngImg.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/
#ifdef TPG
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "../util/Log.h"
#include "../util/JCMarcos.h"
#include "JCTPGImg.h"
#include "JCImageRW.h"
#include <algorithm>
#include <assert.h>
#include "../util/JCCommonMethod.h"
#include "tpgDecoder.h"
#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#endif
//------------------------------------------------------------------------------
#pragma warning (disable: 4996)
namespace laya
{
    
#ifdef WIN32
    typedef void* (*fTPGDecCreate)(const unsigned char*, int);
    typedef void(*fTPGDecDestroy)(void*);
    typedef TPGStatusCode(*fTPGDecodeImage)(void*, unsigned char*, int, int, TPGOutFrame *);
    typedef TPGStatusCode(*fTPGParseHeader)(const unsigned char* pData, int len, TPGFeatures* features);
    typedef TPGStatusCode(*fTPGGetDelayTime)(void*, const unsigned char*, int, int, int*);
    typedef TPGStatusCode(*fTPGGetAdditionalInfo)(void*, const unsigned char*, int, int, const unsigned char**, int*);
    typedef int(*fTPGDecGetVersion)(void);
    typedef TPGStatusCode(*fTPGCanDecode)(void*, const unsigned char*, int, int);

    static fTPGDecCreate TPGDecCreate = NULL;
    static fTPGDecDestroy TPGDecDestroy = NULL;
    static fTPGDecodeImage TPGDecodeImage = NULL;
    static fTPGParseHeader TPGParseHeader = NULL;
    static fTPGGetDelayTime TPGGetDelayTime = NULL;
    static fTPGGetAdditionalInfo TPGGetAdditionalInfo = NULL;
    static fTPGDecGetVersion TPGDecGetVersion = NULL;
    static fTPGCanDecode TPGCanDecode = NULL;

    void initializeTPGProc()
    {
        static bool bInitialized = false;
        if (bInitialized)
            return;

        HMODULE e_handle = LoadLibrary("TPGDec.dll");

        if (e_handle != NULL)
        {
            TPGDecCreate = (fTPGDecCreate)GetProcAddress((HMODULE)e_handle, "TPGDecCreate");
            if (TPGDecCreate == NULL)
            {
                printf("sharpDecCreate not find\n");
                return;
            }

            TPGDecDestroy = (fTPGDecDestroy)GetProcAddress((HMODULE)e_handle, "TPGDecDestroy");
            if (TPGDecDestroy == NULL)
            {
                printf("TPGDecDestroy not find\n");
                return;
            }

            TPGDecodeImage = (fTPGDecodeImage)GetProcAddress((HMODULE)e_handle, "TPGDecodeImage");
            if (TPGDecodeImage == NULL)
            {
                printf("TPGDecodeImage not find\n");
                return;
            }

            TPGParseHeader = (fTPGParseHeader)GetProcAddress((HMODULE)e_handle, "TPGParseHeader");
            if (TPGParseHeader == NULL)
            {
                printf("TPGParseHeader not find\n");
                return;
            }

            TPGGetDelayTime = (fTPGGetDelayTime)GetProcAddress((HMODULE)e_handle, "TPGGetDelayTime");
            if (TPGGetDelayTime == NULL)
            {
                printf("TPGGetDelayTime not find\n");
                return;
            }

            TPGGetAdditionalInfo = (fTPGGetAdditionalInfo)GetProcAddress((HMODULE)e_handle, "TPGGetAdditionalInfo");
            if (TPGGetAdditionalInfo == NULL)
            {
                printf("TPGGetAdditionalInfo not find\n");
                return;
            }

            TPGDecGetVersion = (fTPGDecGetVersion)GetProcAddress((HMODULE)e_handle, "TPGDecGetVersion");
            if (TPGDecGetVersion == NULL)
            {
                printf("TPGDecGetVersion not find\n");
                return;
            }

            TPGCanDecode = (fTPGCanDecode)GetProcAddress((HMODULE)e_handle, "TPGCanDecode");
            if (TPGCanDecode == NULL)
            {
                printf("TPGCanDecode not find\n");
                return;
            }

            bInitialized = true;
        }
    }
#endif
    bool isTpg(const char* pData, int nLength)
    {
#ifdef WIN32
        initializeTPGProc();
#endif
        TPGFeatures features = { 0 };
        TPGStatusCode stats = TPGParseHeader((const unsigned char*)pData, nLength, &features);
        return TPG_STATUS_OK == stats;
    }

    bool LoadTPGFromMem(BitmapData* pBitmapData, unsigned  char * data, int nLength)
    {
#ifdef WIN32
        initializeTPGProc();
#endif
        TPGFeatures features = { 0 };
        TPGStatusCode stats = TPGParseHeader((const unsigned char*)data, nLength, &features);
        if (TPG_STATUS_OK != stats)
        {
            return false;
        }
        int nPixelSize;
        enRawDataFormat fmt = enRawDataFormat::FORMAT_RGB;
        if (features.image_mode == emMode_Normal || features.image_mode == emMode_BlendAlpha)
        {
            LOGE("Load TPG error!  TPG emMode_Normal and emMode_BlendAlpha are not supported!");
            return false;
        }
        else if (features.image_mode == emMode_EncodeAlpha)
        {
            nPixelSize = 4;
            pBitmapData->m_nFormat = GL_RGBA;
            fmt = FORMAT_RGBA;
        }
        else if (features.image_mode == emMode_Animation)
        {
            LOGE("Load TPG error!  TPG emMode_Animation is not supported!");
            return false;
        }
        else if (features.image_mode == emMode_AnimationWithAlpha)
        {
            LOGE("Load TPG error! TPG emMode_AnimationWithAlpha is not supported!");
            return false;
        }
        else
        {
            LOGE("Load TPG error! TPG file format error!");
            return false;
        }

        void *TPGDec = TPGDecCreate(data, nLength);

        if (TPGDec == NULL)
        {
            LOGE("Load TPG error! TPG init failed!");
            return false;
        }

        stats = TPGCanDecode(TPGDec, data, nLength, 0);
        if (TPG_STATUS_OK != stats)
        {
            LOGE("Load TPG error! TPG TPGCanDecode failed!");
            return false;
        }


        int nWidth = pBitmapData->m_nWidth = features.width;
        int nHeight = pBitmapData->m_nHeight = features.height;
        int nSize = pBitmapData->m_nLength = nWidth * nHeight * nPixelSize;
        pBitmapData->m_pImageData = new char[nSize];

        TPGOutFrame outFrame = { 0 };
        outFrame.dstWidth = nWidth;
        outFrame.dstHeight = nHeight;
        outFrame.pOutBuf = (unsigned char*)pBitmapData->m_pImageData;
        outFrame.bufsize = nSize;
        //outFrame.fmt = enRawDataFormat::FORMAT_RGBA_BLEND_ALPHA;
        outFrame.fmt = fmt;

        stats = TPGDecodeImage(TPGDec, data, nLength, 0, &outFrame);
        if (TPG_STATUS_OK != stats)
        {
            delete[] pBitmapData->m_pImageData;
            pBitmapData->m_pImageData = NULL;
            LOGE("Load TPG error! decode TPG error!");
            return false;
        }

        pBitmapData->m_bIsCompressed = false;
        pBitmapData->m_nOffsetX = 0.0f;
        pBitmapData->m_nOffsetY = 0.0f;
        pBitmapData->m_nTextureWidth = nWidth;
        pBitmapData->m_nTextureHeight = nHeight;
        return true;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
#endif
