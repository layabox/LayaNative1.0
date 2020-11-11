/**
@file			JCMaterial.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_30
*/

#include "JCMaterial.h"
#include "JCGpuProgram.h"
#include "../util/JCCrypto.h"
#include "JCMaterial.h"
#include "JC2DShaderManager.h"
#include "../util/Log.h"

namespace laya{

    JCNamedData* JCMaterial::getNamedData() {
        static JCNamedData ret;
        static bool inited = false;
        if (!inited) {
            JCMaterial* pMaterial = nullptr;
            //ret.add(JCGpuProgram::shader_UniformAlpha, (short)(long)&(pM->m_fAlpha), JCNamedData::tp_f32, 1);
            ret.add("g_Color", (short)(long)&(pMaterial->m_fColor), JCNamedData::tp_fvec4, 1);
            ret.add("g_TexRange", (short)(long)&(pMaterial->m_pShaderParam2), JCNamedData::tp_fvec4, 1);
            ret.add("g_Filter", (short)(long)&(pMaterial->m_pShaderParam1), JCNamedData::tp_fvec4, 1);
            inited = true;
        }
        return &ret;
    }
    JCMaterial::JCMaterial(){
        m_pTexture[0] = m_pTexture[1] = NULL;
        m_fAlpha = 1;
        m_nBlendType = BLEND_TYPE_NONE;
        m_nKey = 0;
        m_nGpuProgramType=0;
        m_pFilterData[0]=0;
        m_pFilterData[1]=0;
        m_pFilterData[2]=0;
        m_pFilterData[3]=0;
        m_bEnableZ=false;
        m_pGPUProgram = NULL;
        m_bChanged=true;
        m_nBlendSrcFactor = GL_ONE;
        m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
		m_nAlphaSrcFactor = GL_ONE;
        m_nAlphaDstFacort = GL_ONE;
    }
    JCMaterial::~JCMaterial(){
        m_pTexture[0] = m_pTexture[1] = NULL;
    }

    void JCMaterial::setTexture(int id, JCTexture* pTex) {
        if (id < 2 && id >= 0) {
            m_pTexture[id] = pTex;
        }
    }

    float JCMaterial::getAlpha(){
        return m_fAlpha;
    }  
    int JCMaterial::getBlendType(){
        return m_nBlendType;
    }
    float* JCMaterial::getFilterData(){
        return m_pFilterData;
    }
    unsigned int JCMaterial::getKey(){
        if (m_bChanged) {
            m_nKey = JCBKDRHash::hashMem((const unsigned char*)&m_pTexture, (long)&m_nKey - (long)(char*)&m_pTexture);
        }
        m_bChanged = false;
        return m_nKey;
    }
    void JCMaterial::setAlpha(float a) {
        m_fAlpha = a;
        m_bChanged = true;
    }
    void JCMaterial::setShaderProgram(JCGpuProgram* pProg)
    {
        m_pGPUProgram = pProg;
        //因为这时候可能还没有gl的program对象，所以直接用对象,即算hash的时候把m_pShaderProgram也算在内
        m_bChanged = true;
    }
	void JCMaterial::setTargetBlendType(int blend)
	{
		m_nBlendType = blend;
		switch (blend) {
		case BLEND_TYPE_SOURCE_OVER:
			m_nBlendSrcFactor = GL_ONE;
			m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
			m_nAlphaSrcFactor = GL_ONE;
			m_nAlphaDstFacort = GL_ONE_MINUS_SRC_ALPHA;
			break;
		case BLEND_TYPE_SOURCE_ATOP:
			//TODO
			LOGW("JCMaterial::setTargetBlendType BLEND_TYPE_SOURCE_ATOP Not supported yet");
			m_nBlendSrcFactor = GL_ONE;
			m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
			m_nAlphaSrcFactor = m_nBlendSrcFactor;
			m_nAlphaDstFacort = m_nBlendDstFacort;
			break;
		case BLEND_TYPE_SOURCE_IN:
			m_nBlendSrcFactor = GL_DST_ALPHA;
			m_nBlendDstFacort = GL_ZERO;
            m_nAlphaSrcFactor = m_nBlendSrcFactor;
			m_nAlphaDstFacort = m_nBlendDstFacort;
			break;
		case BLEND_TYPE_SOURCE_OUT:
			m_nBlendSrcFactor = GL_ONE_MINUS_DST_ALPHA;
			m_nBlendDstFacort = GL_ZERO;
            m_nAlphaSrcFactor = m_nBlendSrcFactor;
            m_nAlphaDstFacort = m_nBlendDstFacort;
			break;
		case BLEND_TYPE_DESTINATION_OVER:
			//TODO
			LOGW("JCMaterial::setTargetBlendType BLEND_TYPE_DESTINATION_OVER Not supported yet");
			m_nBlendSrcFactor = GL_ONE;
			m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
            m_nAlphaSrcFactor = m_nBlendSrcFactor;
            m_nAlphaDstFacort = m_nBlendDstFacort;
			break;
		case BLEND_TYPE_DESTINATION_ATOP:
			//TODO
			LOGW("JCMaterial::setTargetBlendType BLEND_TYPE_DESTINATION_ATOP Not supported yet");
			m_nBlendSrcFactor = GL_ONE;
			m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
			m_nAlphaSrcFactor = m_nBlendSrcFactor;
            m_nAlphaDstFacort = m_nBlendDstFacort;
			break;
		case BLEND_TYPE_DESTINATION_IN:
			m_nBlendSrcFactor = GL_ZERO;
			m_nBlendDstFacort = GL_SRC_ALPHA;
			m_nAlphaSrcFactor = m_nBlendSrcFactor;
            m_nAlphaDstFacort = m_nBlendDstFacort;
			break;
		case BLEND_TYPE_DESTINATION_OUT:
			m_nBlendSrcFactor = GL_ZERO;
			m_nBlendDstFacort = GL_ZERO;
            m_nAlphaSrcFactor = m_nBlendSrcFactor;
            m_nAlphaDstFacort = m_nBlendDstFacort;
			break;
		case BLEND_TYPE_LIGHTER:
			m_nBlendSrcFactor = GL_ONE;
			m_nBlendDstFacort = GL_ONE;
            m_nAlphaSrcFactor = m_nBlendSrcFactor;
			m_nAlphaDstFacort = m_nBlendDstFacort;
			break;
		case BLEND_TYPE_COPY:
			m_nBlendSrcFactor = GL_ONE;
			m_nBlendDstFacort = GL_ZERO;
			m_nAlphaSrcFactor = m_nBlendSrcFactor;
            m_nAlphaDstFacort = GL_ONE;
			break;
        case BLEND_TYPE_XOR:
            LOGW("JCMaterial::setTargetBlendType BLEND_TYPE_XOR Not supported yet");
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
            m_nAlphaDstFacort = GL_ONE;
            m_nAlphaSrcFactor = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case BLEND_TYPE_NORMAL:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
            m_nAlphaDstFacort = GL_ONE;
            m_nAlphaSrcFactor = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case BLEND_TYPE_ADD:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_DST_ALPHA;
            m_nAlphaSrcFactor = m_nBlendSrcFactor;
            m_nAlphaDstFacort = m_nBlendDstFacort;
            break;
        case BLEND_TYPE_MULTIPLY:
            m_nBlendSrcFactor = GL_DST_COLOR;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
            m_nAlphaSrcFactor = m_nBlendSrcFactor;
            m_nAlphaDstFacort = m_nBlendDstFacort;
            break;
        case BLEND_TYPE_SCREEN:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE;
            m_nAlphaSrcFactor = m_nBlendSrcFactor;
            m_nAlphaDstFacort = m_nBlendDstFacort;
            break;
        case BLEND_TYPE_OVERLAY:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_COLOR;
            m_nAlphaSrcFactor = m_nBlendSrcFactor;
            m_nAlphaDstFacort = m_nBlendDstFacort;
            break;
        case BLEND_TYPE_LIGHT:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE;
            m_nAlphaSrcFactor = m_nBlendSrcFactor;
            m_nAlphaDstFacort = m_nBlendDstFacort;
            break;
        case BLEND_TYPE_MASK:
            m_nBlendSrcFactor = GL_ZERO;
            m_nBlendDstFacort = GL_SRC_ALPHA;
            m_nAlphaSrcFactor = m_nBlendSrcFactor;
            m_nAlphaDstFacort = m_nBlendDstFacort;
            break;
		default:
			m_nBlendSrcFactor = GL_ONE;
			m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
			m_nAlphaDstFacort = GL_ONE;
			m_nAlphaSrcFactor = GL_ONE_MINUS_SRC_ALPHA;
			break;
		}
		m_bChanged = true;
	}
    void JCMaterial::setBlendType(int blend) 
    {
        if (m_nBlendType == blend)return;
        m_nBlendType = blend;
        switch (blend) {
        case BLEND_TYPE_SOURCE_OVER:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case BLEND_TYPE_SOURCE_ATOP:
            //TODO
            LOGW("JCMaterial::setBlendType BLEND_TYPE_SOURCE_ATOP Not supported yet");
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case BLEND_TYPE_SOURCE_IN:
            m_nBlendSrcFactor = GL_DST_ALPHA;
            m_nBlendDstFacort = GL_ZERO;
            break;
        case BLEND_TYPE_SOURCE_OUT:
            m_nBlendSrcFactor = GL_ONE_MINUS_DST_ALPHA;
            m_nBlendDstFacort = GL_ZERO;
            break;
        case BLEND_TYPE_DESTINATION_OVER:
            //TODO
            LOGW("JCMaterial::setBlendType BLEND_TYPE_DESTINATION_OVER Not supported yet");
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case BLEND_TYPE_DESTINATION_ATOP:
            //TODO
            LOGW("JCMaterial::setBlendType BLEND_TYPE_DESTINATION_ATOP Not supported yet");
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case BLEND_TYPE_DESTINATION_IN:
            m_nBlendSrcFactor = GL_ZERO;
            m_nBlendDstFacort = GL_SRC_ALPHA;
            break;
        case BLEND_TYPE_DESTINATION_OUT:
            m_nBlendSrcFactor = GL_ZERO;
            m_nBlendDstFacort = GL_ZERO;
            break;
        case BLEND_TYPE_LIGHTER:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE;
            break;
        case BLEND_TYPE_COPY:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ZERO;
            break;
        case BLEND_TYPE_XOR:
            //TODO
            LOGW("JCMaterial::setBlendType BLEND_TYPE_XOR Not supported yet");
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case BLEND_TYPE_NORMAL:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case BLEND_TYPE_ADD:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_DST_ALPHA;
            break;
        case BLEND_TYPE_MULTIPLY:
            m_nBlendSrcFactor = GL_DST_COLOR;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case BLEND_TYPE_SCREEN:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE;
            break;
        case BLEND_TYPE_OVERLAY:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_COLOR;
            break;
        case BLEND_TYPE_LIGHT:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE;
            break;
        case BLEND_TYPE_MASK:
            m_nBlendSrcFactor = GL_ZERO;
            m_nBlendDstFacort = GL_SRC_ALPHA;
            break;
        default:
            m_nBlendSrcFactor = GL_ONE;
            m_nBlendDstFacort = GL_ONE_MINUS_SRC_ALPHA;
            break;
        }
		m_nAlphaSrcFactor = m_nBlendSrcFactor;
        m_nAlphaDstFacort = m_nBlendDstFacort;
        m_bChanged = true;
    }
    void JCMaterial::setFillColorShaderParam( float p_nAlpha, int p_nBlendType){
        m_pTexture[0] = NULL;
        m_fAlpha = p_nAlpha;
        setBlendType(p_nBlendType);
        m_bChanged = true;
    }
    void JCMaterial::setDrawImageShaderParam(JCTexture* pTexture, float p_nAlpha, int p_nBlendType){
        m_pTexture[0] = pTexture;
        m_fAlpha = p_nAlpha;
        setBlendType(p_nBlendType);
        m_bChanged = true;
    }
    void JCMaterial::setFillImageShaderParam(JCTexture* pTexture, float p_nAlpha, int p_nBlendType, float nParam1, float nParam2, float nParam3, float nParam4){
        m_pTexture[0] = pTexture;
        m_fAlpha = p_nAlpha;
        setBlendType(p_nBlendType);
        m_pShaderParam2[0] = nParam1;
        m_pShaderParam2[1] = nParam2;
        m_pShaderParam2[2] = nParam3;
        m_pShaderParam2[3] = nParam4;
        //如果是fillImage是必须自己单独绘制的
        m_nKey = -1;
        m_bChanged = true;
    }
    void JCMaterial::setDrawFilterImageShaderParam(JCTexture* pTexture, float p_nAlpha, int p_nBlendType, float p_nFilterR, float p_nFilterG, float p_nFilterB, float p_nFilterGray){
        m_pTexture[0] = pTexture;
        m_fAlpha = p_nAlpha;
        setBlendType(p_nBlendType);
        m_pFilterData[0] = p_nFilterR;
        m_pFilterData[1] = p_nFilterG;
        m_pFilterData[2] = p_nFilterB;
        m_pFilterData[3] = p_nFilterGray;
        m_bChanged = true;
        //生成shader参数
        m_pShaderParam1[0] = 1;
        m_pShaderParam1[1] = 1-(p_nFilterGray);
        m_pShaderParam1[2] = p_nFilterGray/3;
        m_pShaderParam1[3] = 0;
        m_pShaderParam2[0]= p_nFilterR;
        m_pShaderParam2[1]= p_nFilterG;
        m_pShaderParam2[2]= p_nFilterB;
        m_pShaderParam2[3]= 0;
    }
 
}

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
