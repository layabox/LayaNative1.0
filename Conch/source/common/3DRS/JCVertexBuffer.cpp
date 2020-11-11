/**
@file			JCVertexBuffer.cpp
@brief			
@author			guo
@version		1.0
@date			2016_5_31
*/

#include "JCVertexBuffer.h"
#include "JCGpuProgram.h"
#include "JCMaterial.h"
#include "JCNamedData.h"
#include "../util/Log.h"

namespace laya{

    JCVertexBuffer::JCVertexBuffer(){
    }

    JCVertexBuffer::~JCVertexBuffer(){
        if(m_vbo){
            glDeleteBuffers(1,&m_vbo);
            m_vbo=0;
        }
        delSysMem();
    }

    void JCVertexBuffer::delSysMem(){
        if( m_pMemBuffer ){
            for( int i=0; i<m_nStreamNum; i++)
                delete m_pMemBuffer[i];
            delete m_pMemBuffer;
            m_pMemBuffer = nullptr;
        }
        m_vStreamStart.clear();
    }

    bool JCVertexBuffer::create(int p_nSize ){
        delSysMem();
        m_vStreamStart.clear();
        m_nStreamNum = 1;
        m_pMemBuffer = new JCMemClass*[m_nStreamNum];
        m_pMemBuffer[0] = new JCMemClass(p_nSize, p_nSize);
        m_vStreamStart.push_back(0);
        m_nTotalSize = p_nSize;
        return true;
    }

    bool JCVertexBuffer::createMultiStream(int* p_pStreamSize, int p_nStreamNum) {
        delSysMem();
        m_vStreamStart.resize(p_nStreamNum);
        m_nStreamNum = p_nStreamNum;
        m_pMemBuffer = new JCMemClass*[m_nStreamNum];
        int start = 0;
        for (int i = 0; i < m_nStreamNum; i++) {
            m_vStreamStart[i] = start;
            m_pMemBuffer[i] = new JCMemClass(p_pStreamSize[i], p_pStreamSize[i]);
            start += p_pStreamSize[i];
        }
        m_nTotalSize = start;
        if(p_nStreamNum==1)m_uploadTo=&JCVertexBuffer::_uploadTo1;
        else if (p_nStreamNum == 2)m_uploadTo = &JCVertexBuffer::_uploadTo;
        else m_uploadTo = &JCVertexBuffer::_uploadTo;

        return true;
    }

    bool JCVertexBuffer::createMultiStream(std::initializer_list<int> streamDescs) {
        int n = streamDescs.size();
        delSysMem();
        m_vStreamStart.resize(n);
        m_nStreamNum = n;
        m_pMemBuffer = new JCMemClass*[m_nStreamNum];

        int start = 0,i=0;
        for (auto d : streamDescs) {
            m_vStreamStart[i]=start;
            m_pMemBuffer[i] = new JCMemClass(d, d);
            start += d;
            i++;
        }
        m_nTotalSize = start;
        if (n == 1)m_uploadTo = &JCVertexBuffer::_uploadTo1;
        else if (n == 2)m_uploadTo = &JCVertexBuffer::_uploadTo;
        else m_uploadTo = &JCVertexBuffer::_uploadTo;

        return true;
    }

    void JCVertexBuffer::_uploadTo(int p_nTarget, bool p_bForceNewObj) {
        if (m_nStreamNum <= 0) return;

        //GLint lastvbo;
        //glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastvbo);

        //计算总大小
        m_nTotalSize = 0;
        for (int i = 0; i < m_nStreamNum; i++) {
            m_nTotalSize += m_pMemBuffer[i]->getDataSize();
        }

        if (m_vbo > 0 && (p_bForceNewObj || m_nTotalSize > m_nVBLength)) {
            glDeleteBuffers(1, &m_vbo);//TODO 用重用的方法更好
            m_vbo = 0;
        }

        if (m_vbo <= 0) {
            glGenBuffers(1, &m_vbo);
            glBindBuffer(p_nTarget, m_vbo);
            glBufferData(p_nTarget, m_nTotalSize, NULL, GL_DYNAMIC_DRAW);   //分配空间
        }

        glBindBuffer(p_nTarget, m_vbo);
        for (int i = 0; i < m_nStreamNum; i++) {
            JCMemClass*& pmem = m_pMemBuffer[i];
            if (!pmem->getChangedFlag())
                continue;
            glBufferSubData(p_nTarget, m_vStreamStart[i], pmem->getDataSize(), pmem->getBuffer());
            pmem->setChangedFlag(false);
        }
        m_nVBLength = m_nTotalSize;
    }

    void JCVertexBuffer::_uploadTo1(int p_nTarget, bool p_bForceNewObj) {
        m_nTotalSize = m_pMemBuffer[0]->getDataSize();
        if (m_vbo > 0 && (p_bForceNewObj || m_nTotalSize > m_nVBLength)) {
            glDeleteBuffers(1, &m_vbo);//TODO 用重用的方法更好
            m_vbo = 0;
        }

        if (m_vbo <= 0) {
            glGenBuffers(1, &m_vbo);
            glBindBuffer(p_nTarget, m_vbo);
            glBufferData(p_nTarget, m_nTotalSize, NULL, GL_DYNAMIC_DRAW);   //分配空间
        }

        glBindBuffer(p_nTarget, m_vbo);
        JCMemClass*& pmem = m_pMemBuffer[0];
        if (pmem->getChangedFlag()) {
            glBufferSubData(p_nTarget, m_vStreamStart[0], pmem->getDataSize(), pmem->getBuffer());
        }
        pmem->setChangedFlag(false);
        m_nVBLength = m_nTotalSize;
    }

    void JCVertexBuffer::freeGLResource(){
        glDeleteBuffers(1,&m_vbo);
        m_vbo=0;
        m_nVBLength=0;
        for (int i = 0; i < m_nStreamNum; i++){
            m_pMemBuffer[i]->setChangedFlag(true);
        }
    }

    void JCIB::appendData(unsigned short* p_pIdx, int p_nNum, int p_nVertOff) {
        m_nIndexNum += p_nNum;
        if(p_nVertOff==0)
            getMemBuffer(0)->append(p_pIdx, p_nNum * 2);
        else {
            JCMemClass* pmem = getMemBuffer(0);
            unsigned short* pSt = (unsigned short*)pmem->appendEmpty(p_nNum * 2);
            for (int i = 0; i < p_nNum; i++) {
                pSt[i] += p_nVertOff;
            }
        }
    }
}

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
