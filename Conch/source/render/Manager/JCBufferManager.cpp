/**
@file			JCBufferManager.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_6_11
*/

//包含头文件
#include "JCBufferManager.h"


//------------------------------------------------------------------------------
namespace laya
{
    JCBufferManager::JCBufferManager()
    {
        m_nDeleteCount = 0;
    }

    JCBufferManager::~JCBufferManager()
    {
        clear();
    }

    void JCBufferManager::clear()
    {
        MapVertexBuffer::iterator it = m_VertexBufferMap.begin();
        MapVertexBuffer::iterator ed = m_VertexBufferMap.end();
        while (it != ed) {
            delete (*it).second;
            it++;
        }
        m_VertexBufferMap.clear();
    }

    void JCBufferManager::createOrUpdateBuffer(int id,int length ,char *buffer)
    {
        BufferContent* pVertexBuffer = getBufferById(id);
        if (pVertexBuffer == NULL)
        {
            pVertexBuffer = new BufferContent();
            m_VertexBufferMap[id] = pVertexBuffer;
        }
        if (pVertexBuffer->buffer)
        {
            delete[] pVertexBuffer->buffer;
        }
        pVertexBuffer->nLength = length;
        char* pBuf = new char[length];
        memcpy(pBuf, buffer, length);
        pVertexBuffer->buffer = pBuf;
    }
    void JCBufferManager::deleteBuffer(int id)
    {
        if (m_nDeleteCount < m_vDeleteIDs.size())
        {
            m_vDeleteIDs[m_nDeleteCount] = id;
        }
        else
        {
            m_vDeleteIDs.push_back(id);
        }
        m_nDeleteCount++;
    }
    void JCBufferManager::_deleteBuffer(int id)
    {
        MapVertexBuffer::iterator iter = m_VertexBufferMap.find(id);
        if (iter != m_VertexBufferMap.end())
        {
            JCBufferManager::BufferContent* pBuffer = iter->second;
            if (pBuffer)
            {
                delete pBuffer;
                pBuffer = NULL;
            }
            m_VertexBufferMap.erase(iter);
        }
    }
    JCBufferManager::BufferContent* JCBufferManager::getBufferById(int id)
    {
        MapVertexBuffer::iterator it=m_VertexBufferMap.find(id);
        if (it == m_VertexBufferMap.end()) {
            return NULL;
        }
        return (*it).second;
    }
    void JCBufferManager::update()
    {
        if (m_nDeleteCount <= 0)return;
        for (int i = 0; i < m_nDeleteCount; i++)
        {
            _deleteBuffer(m_vDeleteIDs[i]);
        }
        m_nDeleteCount = 0;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
