/**
@file			JCConchMesh.cpp
@brief			
@author			guo
@version		1.0
@date			2016_5_31
*/

#include "JCConchMesh.h"
#include "JCMaterial.h"
#include "JCRenderGroupData.h"
#include "JCGpuProgram.h"
#include "JCRenderer.h"
#include "../buffer/JCBuffer.h"
#include "../util/Log.h"
#include "../util/JCCommonMethod.h"
#include "../geom/RectGeometry.h"
#include "JCMaterialManager.h"
#include "JCVertexBuffer.h"
#include <math.h>
#ifdef WIN32
    #include <Windows.h>
#endif

namespace laya 
{
    int JCConchMesh::s_nCurrentFrameGroupNum = 0;
    int JCConchMesh::s_nCurrentFrameVertextNum = 0;
    JCConchMesh::JCConchMesh() {
        m_vRenderGroupData.resize(1);
        m_pCurGroup = &m_vRenderGroupData[0];
        m_nNamedDataDescCount = 0;
    }
    JCConchMesh::~JCConchMesh() {
        for (auto sl : m_ShaderLinkCacheVert) {
            delete sl.second;
        }
        for (auto sl : m_ShaderLinkCacheUni) {
            delete sl.second;
        }
        if (m_pNamedDataValue) {
            delete[] m_pNamedDataValue;
            m_pNamedDataValue = nullptr;
        }
    }
    bool JCConchMesh::getOrAddGroup(bool bForceNew, int p_nVertType, int p_nGeoMode,JCMaterialBase* p_pMtl, bool p_bHasIndex) {
        bool bChg =
            bForceNew ||
            p_nGeoMode == GL_TRIANGLE_STRIP ||
            p_nGeoMode == GL_LINE_STRIP ||
            m_pCurGroup->m_pMaterial == nullptr ||
            m_pCurGroup->m_nVertexDesc != p_nVertType ||
            m_pCurGroup->m_nGeoMode != p_nGeoMode ||
            m_pCurGroup->m_pMaterial->getKey() != p_pMtl->getKey() ||
            m_pCurGroup->m_nVertNum>32 * 1024 ||  //vb太大也要分组
            m_pCurGroup->m_bHasIndex != p_bHasIndex;
        if (bChg) {
            if (m_nCurGroupPos < m_vRenderGroupData.size())
                m_pCurGroup = &m_vRenderGroupData[m_nCurGroupPos];
            else {
                m_vRenderGroupData.push_back(JCRenderGroupData());
                m_pCurGroup = &m_vRenderGroupData.back();
            }
            m_pCurGroup->m_SetFunc.clear();
            m_pCurGroup->m_pMesh = this;
            m_pCurGroup->m_pMaterial = p_pMtl;
            m_pCurGroup->m_nVBBegin = m_pVB->getMemBuffer(0)->getDataSize();
            m_pCurGroup->m_bHasIndex = p_bHasIndex;
            m_pCurGroup->m_nGeoMode = p_nGeoMode;
            m_pCurGroup->m_nVertexDesc = p_nVertType;
            m_pCurGroup->m_nEleNum = 0;
            m_pCurGroup->m_nVertNum = 0;
            m_pCurGroup->m_bHasObjMat = false;
            m_pCurGroup->m_pShaderUniformInfo = nullptr;
            m_pCurGroup->m_pShaderVertexInfo = nullptr;

            m_nCurGroupPos++;
        }
        return bChg;
    }
    JCRenderGroupData* JCConchMesh::pushVertex(int p_nVertType, int p_nGeoMode, JCMaterialBase* p_pMtl,
        int p_nVertNum, void* p_pData, int p_nDataLen) {
#ifdef _DEBUG
        assert(m_vAllVertexDesc.find(p_nVertType) != m_vAllVertexDesc.end());
#endif
        bool bChg = getOrAddGroup(false, p_nVertType, p_nGeoMode, p_pMtl, false);
        if (bChg) {
        }
        //如果是多流的话，这里需要改一下，只能是外部提供顶点个数。
        //int vertNum = p_nDataLen / m_vAllVertexDesc[p_nVertType].m_VertDescs[0].m_nStride;
        m_nVertNum += p_nVertNum;
        m_pCurGroup->m_nVertNum += p_nVertNum;
        m_pCurGroup->m_nEleNum += p_nVertNum;
        m_pVB->getMemBuffer(0)->append(p_pData, p_nDataLen);
        return m_pCurGroup;
    }
    JCRenderGroupData* JCConchMesh::pushVertex(int p_nVertType, int p_nGeoMode, JCMaterialBase* p_pMtl, int p_nVertNum,
        std::initializer_list<float> verts) {
        return pushVertex(p_nVertType, p_nGeoMode, p_pMtl, p_nVertNum, (void*)verts.begin(), verts.size()*sizeof(float));
    }
    JCRenderGroupData*  JCConchMesh::pushElements(int p_nVertType, JCMaterialBase* p_pMtl, int p_nVertNum,
        std::initializer_list<float> verts,
        std::initializer_list<unsigned short> indices) {
        int nVertDataLen = verts.size();
        int nIdxDataLen = indices.size();
        JCRenderGroupData* pRet = pushElements(p_nVertType, p_pMtl,
            p_nVertNum, (void*)verts.begin(), nVertDataLen*sizeof(float),
            (unsigned short*)indices.begin(), nIdxDataLen*sizeof(unsigned short));
        return pRet;
    }
    JCRenderGroupData* JCConchMesh::pushElements(
        int p_nVertType, JCMaterialBase* p_pMtl,
        int p_nVertNum, void* p_pVBData, int p_nVbLen,
        unsigned short* p_pIBData, int p_nIBLen, bool p_bNoMerge) {
        bool bChg = getOrAddGroup(p_bNoMerge, p_nVertType, GL_TRIANGLES, p_pMtl, true);
        if (bChg) {
            m_pCurGroup->m_nIBBegin = m_pIB->getMemBuffer(0)->getDataSize();
        }
        int lastVertNum = m_nVertNum;
        m_nVertNum += p_nVertNum;
        m_pVB->getMemBuffer(0)->append(p_pVBData, p_nVbLen);
        if (bChg || lastVertNum == 0)//改变组了，或者当前是第一个组
            m_pIB->getMemBuffer(0)->append(p_pIBData, p_nIBLen);
        else {
            //合并mesh的话，需要调整index
            JCMemClass* pmem = m_pIB->getMemBuffer(0);
            unsigned short* pDstIdx = (unsigned short*)pmem->appendEmpty(p_nIBLen);
            for (int i = 0; i < p_nIBLen / 2; i++) {
                *pDstIdx++ = (*p_pIBData++) + m_pCurGroup->m_nVertNum;
            }
        }
        m_pCurGroup->m_nEleNum += p_nIBLen / 2;
        m_pCurGroup->m_nVertNum += p_nVertNum;
        return m_pCurGroup;
    }

    void JCConchMesh::resetData() {
        m_pVB->getMemBuffer(0)->clearData();
        m_pIB->getMemBuffer(0)->clearData();
        m_nCurGroupPos = 0;
        m_nIdxNum = 0;
        //初始化第一个数据。
        m_pCurGroup = new (&m_vRenderGroupData[0])JCRenderGroupData;// &m_vRenderGroupData[0];
        m_nVertNum = 0;
    }
    JCRenderGroupData* JCConchMesh::pushCmd(const std::function<void(void)>& func) {
        if (m_pCurGroup->m_pMaterial == nullptr && m_nCurGroupPos>0) {
        }
        else {
            getOrAddGroup(true, 0, 0, nullptr, false);
        }
        m_pCurGroup->m_SetFunc.push_back(func);
        return m_pCurGroup;
    }
    JCRenderGroupData* JCConchMesh::pushIBVB(int nVertType, const char* pVB, int nVBOff,  const char* pIB, int nIBOff, int nEleNum,
        float* pObjMatrix, JCMaterialBase* pMtl, float* pTexTrans, int vertNum) {
        int nVertNum = vertNum?vertNum:(nEleNum / 6 * 4);
        int vblen = nVertNum * sizeof(RectGeometry::VertexInfo);   //顶点

        JCMemClass* pVertMem = m_pVB->getMemBuffer(0);
        int curVertSz = pVertMem->getDataSize();

        //TEST
        //JCRenderGroupData* pg1 = pushVertex(nVertType, GL_TRIANGLE_STRIP, pMtl, nVertNum, (void*)(pVB + nVBOff),
        //    vblen);
        //for (int i = 0; i < 16; i++) pg1->m_matObject[i] = pObjMatrix[i];
        //pg1->m_bHasObjMat = true;
        //TEST

        JCRenderGroupData* pGroupData = pushElements(/*XYUVRGBA*/nVertType, pMtl,
            nVertNum, (void*)(pVB + nVBOff), vblen,
            (unsigned short*)(pIB + nIBOff), nEleNum * 2,true);

        //修改push进去的顶点信息
        RectGeometry::VertexInfo* pData = (RectGeometry::VertexInfo*)(pVertMem->getBuffer() + curVertSz);

        float rectw = pTexTrans[2] - pTexTrans[0];
        float recth = pTexTrans[3] - pTexTrans[1];
        if (rectw < 1.0f || recth < 1.0f) {
            for (int i = 0; i < nVertNum; i++) {
                pData->u = pData->u*rectw + pTexTrans[0];
                pData->v = pData->v*recth + pTexTrans[1];
                pData++;
            }
        }

        for (int i = 0; i < 16; i++) pGroupData->m_matObject[i] = pObjMatrix[i];
        pGroupData->m_bHasObjMat = true;
        return pGroupData;
    }

    void JCConchMesh::freeGLResource() {
        JCMesh::freeGLResource();
        for (auto sl : m_ShaderLinkCacheVert) {
            delete sl.second;
        }
        m_ShaderLinkCacheVert.clear();
        for (auto sl : m_ShaderLinkCacheUni) {
            delete sl.second;
        }
        m_ShaderLinkCacheUni.clear();
    }
    int JCConchMesh::getCurrentGroupPos()
    {
        return m_nCurGroupPos;
    }
    JCShaderLink_Vertex* JCConchMesh::getShaderVertexLink(int p_nVertexDesc, JCGpuProgram* p_pGpuProgram) {
        int slkey = ((p_nVertexDesc & 0xffff) << 16) | (p_pGpuProgram->getGpuProgram() & 0xffff);
        auto itCachedV = m_ShaderLinkCacheVert.find(slkey);
        if (itCachedV == m_ShaderLinkCacheVert.end()) {
            JCShaderLink_Vertex* linkinfo = new JCShaderLink_Vertex();
            //JCRenderer::link_shader_vertex(*linkinfo, &m_vAllVertexDesc[p_nVertexDesc], p_pGpuProgram);
            m_ShaderLinkCacheVert[slkey] = linkinfo;
            return linkinfo;
        }
        else {
            return (*itCachedV).second;
        }
    }
    JCShaderLink_Uniform* JCConchMesh::getShaderUniformLink(JCGpuProgram* p_pGpuProgram) {
        int nGpuProg = p_pGpuProgram->getGpuProgram();
        auto itCachedU = m_ShaderLinkCacheUni.find(nGpuProg);
        if (itCachedU == m_ShaderLinkCacheUni.end()) {
            JCShaderLink_Uniform* linkinfo = new JCShaderLink_Uniform();
            //JCRenderer::link_shader_uniformdata(*linkinfo, p_pGpuProgram, m_pAllNamedData, 2);
            m_ShaderLinkCacheUni[nGpuProg] = linkinfo;
            return linkinfo;
        }
        else {
            return (*itCachedU).second;
        }
    }
    void JCConchMesh::setShaderValue(int nSemantics, char* pData, int nSize)
    {
        if (nSemantics >= m_NamedDataDesc.m_AllData.size())
        {
            m_nNamedDataDescCount++;
            int nOldSize = m_NamedDataDesc.getNamedDataSize();
            m_NamedDataDesc.add(nSemantics, nOldSize, JCNamedData::tp_unknown, 1, nSize);
            char* pOldData = m_pNamedDataValue;
            m_pNamedDataValue = new char[nOldSize + nSize];
            if (nOldSize > 0)
            {
                memcpy(m_pNamedDataValue, pOldData, nOldSize);
                delete[] pOldData;
            }
        }
        else if (m_NamedDataDesc.m_AllData[nSemantics] == NULL)
        {
            m_nNamedDataDescCount++;
            int nOldSize = m_NamedDataDesc.getNamedDataSize();
            char* pOldData = m_pNamedDataValue;
            //比如现在要插入的nSemantics = 6  size=10
            //统计0-5，的大小
            int nPreSize = 0;
            for (int i = nSemantics - 1; i >= 0; i--)
            {
                JCNamedData::dataDesc* pDataDesc = m_NamedDataDesc.m_AllData[i];
                if (pDataDesc)
                {
                    nPreSize = pDataDesc->m_nOffset + pDataDesc->m_nLen;
                    break;
                }
            }
            //把第6个插入
            m_NamedDataDesc.add(nSemantics, nPreSize, JCNamedData::tp_unknown, 1, nSize);
            m_pNamedDataValue = new char[nOldSize + nSize];
            if (nPreSize > 0)
            {
                memcpy(m_pNamedDataValue, pOldData, nPreSize);
            }

            //修改 7 - 9 的偏移
            int nCountSize = 0;
            for (int i = nSemantics + 1, n = m_NamedDataDesc.m_AllData.size(); i < n; i++)
            {
                JCNamedData::dataDesc* pDataDesc = m_NamedDataDesc.m_AllData[i];
                if (pDataDesc)
                {
                    pDataDesc->m_nOffset += nSize;
                    nCountSize += pDataDesc->m_nLen;
                }
            }
            memcpy(m_pNamedDataValue + nPreSize + nSize, pOldData + nPreSize, nCountSize);
            delete[] pOldData;
        }
        m_NamedDataDesc.setData(nSemantics, m_pNamedDataValue, pData, nSize);
    }
}