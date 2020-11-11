/**
@file			JCMesh.cpp
@brief			
@author			guo
@version		1.0
@date			2016_5_31
*/

#include "JCVertexBuffer.h"
#include "JCMesh.h"
#if __APPLE__
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#else
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif
#include "JCGpuProgram.h"
#include "JCMaterial.h"
#include "JCNamedData.h"
#include "JCRenderGroupData.h"
#include "../util/Log.h"
#include "../buffer/JCBuffer.h"
#include <assert.h>
#include "JCRenderer.h"
#include "../geom/RectGeometry.h"

namespace laya {

    JCVertexDesc::Desc* JCVertexDesc::getVertexDesc(const char* p_pszName) {
        for (int i = 0, sz = m_VertDescs.size(); i < sz; i++) {
            if (strcmp(m_VertDescs[i].m_strAttribName.c_str(), p_pszName) == 0)
                return &m_VertDescs[i];
        }
        return nullptr;
    }
    JCVertexDesc::Desc* JCVertexDesc::getVertexDesc(int nSementicsEnum)
    {
        for (int i = 0, sz = m_VertDescs.size(); i < sz; i++) {
            if ( m_VertDescs[i].m_nSemanticsEnum == nSementicsEnum)
                return &m_VertDescs[i];
        }
        return nullptr;
    }
    JCVertexDesc::JCVertexDesc(std::initializer_list<DescSimp> descs) {
        m_nStreanNum = 1;
        int n = descs.size();
        m_VertDescs.clear();
        m_VertDescs.reserve(n);
        short nOff = 0;
        for (auto d : descs) {
            m_VertDescs.push_back({ d.pName,d.nType,nOff,0,0 });//stride先写0
            switch (d.nType) {
            case GL_INT:case GL_FLOAT: nOff += sizeof(int); break;
            case GL_FLOAT_VEC2:case GL_INT_VEC2:nOff += sizeof(float) * 2; break;
            case GL_FLOAT_VEC3:case GL_INT_VEC3:nOff += sizeof(float) * 3; break;
            case GL_FLOAT_VEC4:case GL_INT_VEC4:nOff += sizeof(float) * 4; break;
            default:
                assert(0);
            }
        }
        for (auto& d : m_VertDescs) {
            d.m_nStride = nOff;
        }
    }
	void JCVertexDesc::addShaderDefine(long long nValue)
	{
		m_kShaderDefine.addShaderDefine(nValue);
	}
	void JCVertexDesc::removeShaderDefine(long long nValue)
	{
		m_kShaderDefine.removeShaderDefine(nValue);
	}
    JCMesh::JCMesh() {
        m_pVB = new JCVB();
        m_pVB->create(100);
        m_pIB = new JCIB();
        m_pIB->create(100);
        m_nVertNum = m_nIdxNum = 0;
    }
    JCMesh::~JCMesh() {
        delete m_pVB;
        delete m_pIB;
        m_pVB = nullptr;
        m_pIB = nullptr;
    }
    void JCMesh::regVertexDesc(const JCVertexDesc& p_Desc, int id) {
        m_vAllVertexDesc[id] = p_Desc;
    }
    int JCMesh::getGroupNum(){
        return m_vRenderGroupData.size();
    }
    void JCMesh::upload() {
        m_pVB->upload();
        m_pIB->upload();
    }
    void JCMesh::freeGLResource() {
        if (m_pVB) m_pVB->freeGLResource();
        if (m_pIB) m_pIB->freeGLResource();
    }
}

