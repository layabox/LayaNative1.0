/**
@file			JCConchMesh2D.cpp
@brief			
@author			James
@version		1.0
@date			2016_12_7
*/

#include "JCConchMesh2D.h"
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
#ifdef WIN32
    #include <Windows.h>
#endif
#include "../geom/JCMatrix44.h"

namespace laya 
{
    JCConchMesh2D::JCConchMesh2D()
    {
        m_pAllNamedData[0] = NULL;
        m_pAllNamedData[1] = NULL;
    }
    JCConchMesh2D::~JCConchMesh2D()
    {

    }
    void JCConchMesh2D::setNamedData(JCNamedData* pMtl, JCNamedData* pGlobal)
    {
        m_pAllNamedData[0] = pMtl;
        m_pAllNamedData[1] = pGlobal;
        regVertexDesc({ { "position",GL_FLOAT_VEC2 },{ "texcoord",GL_FLOAT_VEC2 },{ "color",GL_FLOAT_VEC4 } }, XYUVRGBA);
        regVertexDesc({ { "position",GL_FLOAT_VEC2 },{ "texcoord",GL_FLOAT_VEC2 } }, XYUV);
        regVertexDesc({ { "a_CornerTextureCoordinate",GL_FLOAT_VEC4 },{ "a_Position",GL_FLOAT_VEC3 },{ "a_Velocity",GL_FLOAT_VEC3 },{ "a_StartColor",GL_FLOAT_VEC4 },{ "a_EndColor",GL_FLOAT_VEC4 },{ "a_SizeRotation",GL_FLOAT_VEC3 },{ "a_Radius",GL_FLOAT_VEC2 },{ "a_Radian",GL_FLOAT_VEC4 },{ "a_AgeAddScale",GL_FLOAT },{ "a_Time",GL_FLOAT } }, PARTICLE2D);
    }

    JCRenderGroupData* JCConchMesh2D::pushParticle2D(int  p_nMaxPartical, int p_nFirstNewElement, int p_nFirstActiveElement, int p_nFirstFreeElement, const char* pVB, const char* pIB, float* pObjMatrix, JCMaterial* pMtl)
    {
        int perSize = getVertexDesc(4).m_VertDescs[0].m_nStride;

        JCRenderGroupData* pGroupData = NULL;
        int particleNum = 0;// m_pDynamicMesh->getVertexDesc(4).m_VertDescs[0].m_nStride;
                            //if (p_nNumElement == 39 || p_nNumElement == 174)
        int p_nStartIndex = 0, p_nNumElement = 0;
        if (p_nFirstActiveElement < p_nFirstFreeElement)
        {
            p_nStartIndex = p_nFirstActiveElement * 12;
            particleNum = p_nFirstFreeElement - p_nFirstActiveElement;
            p_nNumElement = particleNum * 6;
            pGroupData = pushElements(4, pMtl,
                particleNum * 4, (void*)(pVB + 0), (particleNum + p_nFirstActiveElement) * 4 * perSize,
                (unsigned short*)(pIB + p_nStartIndex), p_nNumElement * 2, true);

            for (int i = 0; i < 16; i++) pGroupData->m_matObject[i] = pObjMatrix[i];
            pGroupData->m_bHasObjMat = true;
        }
        else
        {
            // settings.maxPartices - _firstActiveElement) * 6, WebGLContext.UNSIGNED_SHORT, _firstActiveElement * 6 * 2);

            p_nStartIndex = p_nFirstActiveElement * 12;
            particleNum = (p_nMaxPartical - p_nFirstActiveElement);
            p_nNumElement = particleNum * 6;

            pGroupData = pushElements(4, pMtl,
                particleNum * 4, (void*)(pVB + 0), (p_nMaxPartical)* 4 * perSize,
                (unsigned short*)(pIB + p_nStartIndex), p_nNumElement * 2, true);
            for (int i = 0; i < 16; i++) pGroupData->m_matObject[i] = pObjMatrix[i];
            pGroupData->m_bHasObjMat = true;

            if (p_nFirstFreeElement > 0)
            {
                p_nStartIndex = 0;
                particleNum = p_nFirstFreeElement;
                p_nNumElement = particleNum * 6;

                pGroupData = pushElements(4, pMtl,
                    particleNum * 4, (void*)(pVB + 0), particleNum * 4 * perSize,
                    (unsigned short*)(pIB + p_nStartIndex), p_nNumElement * 2, true);
                for (int i = 0; i < 16; i++) pGroupData->m_matObject[i] = pObjMatrix[i];
                pGroupData->m_bHasObjMat = true;
            }
        }
        return pGroupData;
    }

    void JCConchMesh2D::render(char* pGlobalRenderData) {
        s_nCurrentFrameGroupNum += m_nCurGroupPos;
        s_nCurrentFrameVertextNum += m_nVertNum;
        for (int i = 0; i < m_nCurGroupPos; i++) {
            JCRenderGroupData *pRenderGroup = &m_vRenderGroupData[i];
            int cmdsz = pRenderGroup->m_SetFunc.size();
            if (cmdsz) {
                for (int ci = 0; ci < cmdsz; ci++) {
                    pRenderGroup->m_SetFunc[ci]();
                }
                continue;
            }
            JCMaterial* pMaterial = (JCMaterial*)pRenderGroup->m_pMaterial;
            if (pMaterial == nullptr) continue;
            JCGpuProgram* pProgram = pMaterial->getGpuProgram();
            if (pProgram == nullptr)  continue;
            //if (pRenderGroup->m_bScissor) {
            //    glEnable(GL_SCISSOR_TEST);
            //    glScissor(pRenderGroup->m_ClipRect[0],
            //        pRenderGroup->m_ClipRect[1],
            //        pRenderGroup->m_ClipRect[2],
            //        pRenderGroup->m_ClipRect[3]);
            //}
            //如果shader还没有编译
            int nGpuProg = pProgram->getGpuProgram();
            if ( nGpuProg<= 0) {
                continue;
            }
            int slkey = (((pRenderGroup->m_nVertexDesc)&0xffff)<<16)|(nGpuProg &0xffff);
            auto itCachedV = m_ShaderLinkCacheVert.find(slkey);
            if (itCachedV == m_ShaderLinkCacheVert.end()) {
                JCShaderLink_Vertex* linkinfo = new JCShaderLink_Vertex();
                JCRenderer::link_shader_vertex(*linkinfo, &m_vAllVertexDesc[pRenderGroup->m_nVertexDesc],pProgram);
                m_ShaderLinkCacheVert[slkey] = linkinfo;
                pRenderGroup->m_pShaderVertexInfo= linkinfo;
            }
            else {
                pRenderGroup->m_pShaderVertexInfo = (*itCachedV).second;
            }
            auto itCachedU = m_ShaderLinkCacheUni.find(nGpuProg);
            if (itCachedU == m_ShaderLinkCacheUni.end()) {
                JCShaderLink_Uniform* linkinfo = new JCShaderLink_Uniform();
                JCRenderer::link_shader_uniformdata(*linkinfo, pProgram, m_pAllNamedData, 2, nullptr);
                m_ShaderLinkCacheUni[nGpuProg] = linkinfo;
                pRenderGroup->m_pShaderUniformInfo = linkinfo;
            }
            else {
                pRenderGroup->m_pShaderUniformInfo = (*itCachedU).second;
            }
            char* pShaderData[] = { (char*)pMaterial , pGlobalRenderData };
            //TODO 临时 直接修改context的矩阵
            if (pRenderGroup->m_bHasObjMat) {
                float* pObjMat = (float*)pGlobalRenderData + 2;
                for (int i = 0; i < 16; i++)
                    pObjMat[i] = pRenderGroup->m_matObject[i];
            }
            float* pCtxAlpha = (float*)pGlobalRenderData + 66;
            *pCtxAlpha = pMaterial->m_fAlpha;
            JCRenderer::renderMesh(pRenderGroup->m_pMesh, pRenderGroup, pShaderData, 2);
            //if (pRenderGroup->m_bScissor) {
            //    glDisable(GL_SCISSOR_TEST);
            //}
        }
        resetData();
    }
    void JCConchMesh2D::renderToMemoryCanvas(char* pGlobalRenderData,float fAlpha)
    {
        s_nCurrentFrameGroupNum += m_nCurGroupPos;
        s_nCurrentFrameVertextNum += m_nVertNum;
        for (int i = 0; i < m_nCurGroupPos; i++) 
        {
            JCRenderGroupData *pRenderGroup = &m_vRenderGroupData[i];
            int cmdsz = pRenderGroup->m_SetFunc.size();
            if (cmdsz) 
            {
                for (int ci = 0; ci < cmdsz; ci++) 
                {
                    pRenderGroup->m_SetFunc[ci]();
                }
                continue;
            }
            JCMaterial* pMaterial = (JCMaterial*)pRenderGroup->m_pMaterial;
            if (pMaterial == nullptr) continue;
            //普通图片进行 enable
            JCTexture* pTexture = pMaterial->getTexture(0);
            if (pTexture && pTexture->m_nType == TEXTURE_TYPE_NORMAL)
            {
                pTexture->enable();
            }
            JCGpuProgram* pProgram = pMaterial->getGpuProgram();
            if (pProgram == nullptr)  continue;
            //如果shader还没有编译
            int nGpuProg = pProgram->getGpuProgram();
            if (nGpuProg<= 0) {
                continue;
            }
            int slkey = (((pRenderGroup->m_nVertexDesc) & 0xffff) << 16) | (nGpuProg & 0xffff);
            auto itCachedV = m_ShaderLinkCacheVert.find(slkey);
            if (itCachedV == m_ShaderLinkCacheVert.end()) {
                JCShaderLink_Vertex* linkinfo = new JCShaderLink_Vertex();
                JCRenderer::link_shader_vertex(*linkinfo, &m_vAllVertexDesc[pRenderGroup->m_nVertexDesc], pProgram);
                m_ShaderLinkCacheVert[slkey] = linkinfo;
                pRenderGroup->m_pShaderVertexInfo = linkinfo;
            }
            else {
                pRenderGroup->m_pShaderVertexInfo = (*itCachedV).second;
            }
            auto itCachedU = m_ShaderLinkCacheUni.find(nGpuProg);
            if (itCachedU == m_ShaderLinkCacheUni.end()) {
                JCShaderLink_Uniform* linkinfo = new JCShaderLink_Uniform();
                JCRenderer::link_shader_uniformdata(*linkinfo, pProgram, m_pAllNamedData, 2,nullptr);
                m_ShaderLinkCacheUni[nGpuProg] = linkinfo;
                pRenderGroup->m_pShaderUniformInfo = linkinfo;
            }
            else {
                pRenderGroup->m_pShaderUniformInfo = (*itCachedU).second;
            }
            char* pShaderData[] = { (char*)pMaterial , pGlobalRenderData };
            //TODO 临时 直接修改context的矩阵
            float* pWorldMat = (float*)pGlobalRenderData + 18;
            JCMatrix44* pCtxMat = (JCMatrix44*)pWorldMat;
            JCMatrix44* pObjMat = (JCMatrix44*)((float*)pGlobalRenderData + 2);
            if (pRenderGroup->m_bHasObjMat) {
                float* pObjMat = (float*)pGlobalRenderData + 2;
                for (int i = 0; i < 16; i++)
                    pObjMat[i] = pRenderGroup->m_matObject[i];
            }
            float* pCtxAlpha = (float*)pGlobalRenderData + 66;
            *pCtxAlpha = (pMaterial->m_fAlpha * fAlpha);
            //TODO 不知道谁在前面，目前都是对的。
            *pObjMat = (*pObjMat) * (*pCtxMat);
            JCRenderer::renderMesh(pRenderGroup->m_pMesh, pRenderGroup, pShaderData, 2);
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------