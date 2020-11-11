/**
@file			JCContextSaveData.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_25
*/

#include "JCContextSaveData.h"
#include <util/Log.h>

//------------------------------------------------------------------------------
namespace laya
{
    #define SAVE_DATA_NUM 20
    JCContextSaveData::JCContextSaveData()
    {
        m_nSaveDataSize = SAVE_DATA_NUM * sizeof(ContextDataInfo);
        m_pBuffer = new char[m_nSaveDataSize];
        memset( m_pBuffer,0,m_nSaveDataSize );
        //放入一段默认的
        ContextDataInfo kData;
        memcpy(m_pBuffer, &kData, sizeof(ContextDataInfo));
        //当前的data指向默认
        m_pCurrentContextData = (ContextDataInfo*)(m_pBuffer);
        m_nCurrentBufferPos = 0;
    }
    JCContextSaveData::~JCContextSaveData()
    {
        if (m_pBuffer)
        {
            delete[] m_pBuffer;
            m_pBuffer = NULL;
        }
        m_pCurrentContextData = NULL;
    }
    bool JCContextSaveData::reset(int w, int h)
    {
        memset(m_pBuffer, 0, m_nSaveDataSize);
        //放入一段默认的
        ContextDataInfo kData;
        kData.kRect.width = w;
        kData.kRect.height = h;
        kData.kClipRect.width = w;
        kData.kClipRect.height = h;
        memcpy(m_pBuffer, &kData, sizeof(ContextDataInfo));
        //当前的data指向默认
        m_pCurrentContextData = (ContextDataInfo*)(m_pBuffer);
        m_nCurrentBufferPos = 0;
        return true;
    }
    bool JCContextSaveData::save()
    {
        m_nCurrentBufferPos += sizeof(ContextDataInfo);
        if (m_nCurrentBufferPos >= m_nSaveDataSize )
        {
            int nEnlargeSize = SAVE_DATA_NUM * sizeof(ContextDataInfo);
            char* pBuffer = new char[m_nSaveDataSize + nEnlargeSize];
            memset(pBuffer, 0, m_nSaveDataSize + nEnlargeSize);
            memcpy(pBuffer, m_pBuffer, m_nSaveDataSize);
            m_nSaveDataSize += nEnlargeSize;
            memcpy(pBuffer + m_nCurrentBufferPos, m_pCurrentContextData, sizeof(ContextDataInfo));
            if (m_pBuffer)
            {
                delete[] m_pBuffer;
                m_pBuffer = NULL;
            }
            m_pBuffer = pBuffer;
        }
        else
        {
            memcpy(m_pBuffer + m_nCurrentBufferPos, m_pCurrentContextData, sizeof(ContextDataInfo));
        }
        //重新指向新的位置
        m_pCurrentContextData = (ContextDataInfo*)(m_pBuffer + m_nCurrentBufferPos);
        m_pCurrentContextData->nClipType = MASK_TYPE_NO;
        return true;
    }
    bool JCContextSaveData::restore()
    {
        if (m_nCurrentBufferPos > 0)
        {
            m_nCurrentBufferPos -= sizeof(ContextDataInfo);
            m_pCurrentContextData = (ContextDataInfo*)(m_pBuffer + m_nCurrentBufferPos);
            return true;
        }
        else
        {
            LOGE("JCContextSaveData::restore save and restore don't make a pair.");
            return false;
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------