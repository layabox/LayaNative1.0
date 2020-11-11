/**
@file			JCRenderer.cpp
@brief			
@author			guo
@version		1.0
@date			2016_5_31
*/

#include "JCRenderer.h"
#include "JCMaterial.h"
#include "JCVertexBuffer.h"
#include "JCMesh.h"
#include "JCGpuProgram.h"
#include "JCRenderGroupData.h"
#include "../util/Log.h"
#include "JCGL.h"

#define verify(v,desc)    {if(!(v)){ LOGE(desc); throw -1;}}
void GL_APIENTRY laya_glUniformMatrix4fv (GLint location, GLsizei count, const GLfloat* value){
    glUniformMatrix4fv(location,count,false,value);
}
void GL_APIENTRY laya_glUniformMatrix3fv (GLint location, GLsizei count, const GLfloat* value){
    glUniformMatrix3fv(location,count,false,value);
}
void GL_APIENTRY laya_glUniformMatrix2fv (GLint location, GLsizei count, const GLfloat* value){
    glUniformMatrix2fv(location,count,false,value);
}
namespace laya{
    JCGL    gmygl;
    unsigned int JCRenderer::s_nLastVertexAttrib = 0;
    int JCRenderer::s_nGLCaps = GLC_NONE;
    int gnMaxVertAttrib = 16;
    void JCRenderer::init() {
        const char* pstrVersion = (const char*)glGetString(GL_VERSION);
        LOGI("OpenGL ES version [%s]", pstrVersion);
#ifdef TPG
        s_nGLCaps |= GLC_TEXTURE_TPG;
#endif

        char* pStrExt = (char *)glGetString(GL_EXTENSIONS);
        if (pStrExt != nullptr){
            if (strstr(pStrExt, "GL_IMG_texture_compression_pvrtc")) {
                s_nGLCaps |= GLC_TEXTURE_COMPRESSION_PVR;
            }
            if (strstr(pstrVersion, "OpenGL ES 3.")) {
                s_nGLCaps |= GLC_NOPT;
            }
#ifdef ANDROID
            if (strstr(pstrVersion, "OpenGL ES 3.")) {
                s_nGLCaps |= GLC_TEXTURE_COMPRESSION_ETC1;
                s_nGLCaps |= GLC_TEXTURE_COMPRESSION_ETC2;
            }
#endif
#ifdef GL_ETC1_RGB8_OES
            if (strstr(pStrExt, "GL_OES_compressed_ETC1_RGB8_texture")) {
                s_nGLCaps |= GLC_TEXTURE_COMPRESSION_ETC1;
            }
#endif
        }
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &gnMaxVertAttrib);
        LOGI("INFO:gles 支持的最大顶点属性个数=%d", gnMaxVertAttrib);
    }
    bool JCRenderer::link_shader_vertex( JCShaderLink_Vertex& p_ShaderInfo, JCVertexDesc* p_pVertexDesc, JCGpuProgram* p_pGPUProgram){
        int nProgram = p_pGPUProgram->getGpuProgram();
        if (nProgram <= 0) {
            return false;
        }
        verify((nProgram>0),"gl program should be valid\n");
        p_ShaderInfo.m_nProgramID = nProgram;
        p_ShaderInfo.m_AttribInfo.clear();
        char sVarName[64];
        int length,size;
        GLenum type;
        //shader中的属性
        int nAttribNum=0;
        glGetProgramiv(nProgram,GL_ACTIVE_ATTRIBUTES,&nAttribNum);
        int nAttribSize=0,nAttribType=0;
        for( int i=0; i<nAttribNum; i++){
            glGetActiveAttrib(nProgram, i, sizeof(sVarName), &length, &size, &type, sVarName);
            int pos = glGetAttribLocation(nProgram, sVarName);
            int nSemantics = p_pGPUProgram->m_pTemp->getAttrSemanticsName(sVarName);
            JCVertexDesc::Desc* pVerteDesc = NULL;
            if (nSemantics < 0)
            {
                pVerteDesc = p_pVertexDesc->getVertexDesc(sVarName);
            }
            else
            {
                pVerteDesc = p_pVertexDesc->getVertexDesc(nSemantics);
            }
            if (pVerteDesc) 
            {
                type = pVerteDesc->m_nType;//如果不行加上只有vec4和vec3的限制
            }
            switch( type ){
            case GL_INT_VEC2:nAttribSize=2; nAttribType=GL_INT; break;
            case GL_INT_VEC3:nAttribSize=3; nAttribType=GL_INT; break;
            case GL_INT_VEC4:nAttribSize=4; nAttribType=GL_INT; break;
            case GL_FLOAT:nAttribSize = 1; nAttribType=GL_FLOAT; break;
            case GL_FLOAT_VEC2:nAttribSize=2; nAttribType=GL_FLOAT;        break;
            case GL_FLOAT_VEC3:nAttribSize=3; nAttribType=GL_FLOAT;        break;
            case GL_FLOAT_VEC4:nAttribSize=4; nAttribType=GL_FLOAT;        break;
            case GL_FLOAT_MAT2:nAttribSize=4; nAttribType=GL_FLOAT;        break;
            case GL_FLOAT_MAT3:nAttribSize=9; nAttribType=GL_FLOAT;        break;
            case GL_FLOAT_MAT4:nAttribSize=16; nAttribType=GL_FLOAT;        break;
            }
            if( pVerteDesc /*&& pVerteDesc->m_nType == type*/ ){
                JCShaderLink_Vertex::AttribInfo aif =
                {pVerteDesc->m_nStreamID, pos, nAttribSize, nAttribType, pVerteDesc->m_nOffset, pVerteDesc->m_nStride};
                p_ShaderInfo.m_AttribInfo.push_back(aif);            
                p_ShaderInfo.m_EnabledVertexAttrib |= (1 << pos);
            }
        }
        return true;
    }
    bool JCRenderer::link_shader_uniformdata(JCShaderLink_Uniform& p_ShaderInfo, JCGpuProgram* p_pGPUProgram,
        JCNamedData** p_pNamedData, int p_nNamedDataNum, std::map<int, int>* pMapTextureDesc) {
        int nProgram = p_pGPUProgram->getGpuProgram();
        if (nProgram <= 0) {
            return false;
        }
        verify((nProgram>0), "gl program should be valid\n");
        p_ShaderInfo.m_nProgramID = nProgram;
        p_ShaderInfo.m_Textures.clear();
        p_ShaderInfo.m_MaterialUniform.clear();
        char sVarName[64];
        int length, size;
        GLenum type;
        //shader中的变量
        int nUniformNum = 0;
        glGetProgramiv(nProgram, GL_ACTIVE_UNIFORMS, &nUniformNum);
        if (nUniformNum>0 && (!p_pNamedData || p_nNamedDataNum == 0)) {
            LOGW("JCRenderer::link_shader_uniformdata renderer::bindShaderFetch 出错。没有设置p_pNameData");
            return false;
        }
        static int SamplerPrefixLen = strlen(JCGpuProgram::shader_SamplerPre);
        static int CubeSamplerPrefixLen = strlen(JCGpuProgram::shader_CubeSamplerPre);
        int nSampler2DNum = 0;  //允许命名不符合g_Texn 这时候就用这个来表示使用哪个贴图。
        int nCubeSamplerNum = 0;
        int nSID = 0;
        for (int i = 0; i<nUniformNum; i++) {
            //size 是数组的长度。通常为1
            glGetActiveUniform(nProgram, i, sizeof(sVarName), &length, &size, &type, sVarName);
            int location = glGetUniformLocation(nProgram, sVarName);
            //处理 bones[0] 这样的uniform
            int nVarNameLenght = strlen(sVarName);
            if ( nVarNameLenght > 4 && strcmp( sVarName+(nVarNameLenght-3),"[0]" ) == 0)
            {
                sVarName[nVarNameLenght - 3] = 0;
            }
            JCNamedData::dataDesc* pDesc = NULL;
            int ni = 0;
            JCGpuProgramTemplate::UniformInfo* pUnformInfo = p_pGPUProgram->m_pTemp->getUniformInfo(sVarName);
            if (pUnformInfo != NULL)
            {
                if (pUnformInfo->m_nType < p_nNamedDataNum)
                {
                    ni = pUnformInfo->m_nType;
                    JCNamedData* pNamedData = p_pNamedData[pUnformInfo->m_nType];
                    if (pNamedData && pUnformInfo->m_nSemanticsEnum < pNamedData->m_AllData.size())
                    {
                        pDesc = pNamedData->m_AllData[pUnformInfo->m_nSemanticsEnum];
                    }
                }
                else
                {
                    continue;
                }
            }
            else
            {
                //2D部分，肯定找不到
                for (ni = 0; ni < p_nNamedDataNum; ni++)
                {
                    JCNamedData* pNamedData = p_pNamedData[ni];
                    if (pNamedData)
                    {
                        int pos = pNamedData->get(sVarName);
                        if (pos >= 0)
                        {
                            pDesc = pNamedData->m_AllData[pos];
                        }
                        if (pDesc)break;
                    }
                }
            }
            switch (type)
            {
            case GL_SAMPLER_2D:
                nSID = nSampler2DNum;
                if( pUnformInfo )
                {
                    //3D部分，肯定可以找到
                    std::string sKey = sVarName;
                    if (pMapTextureDesc) {
                        std::map<int, int>::iterator iter = pMapTextureDesc->find(pUnformInfo->m_nSemanticsEnum);
                        if (iter != pMapTextureDesc->end())
                        {
                            nSID = iter->second;
                        }
                    }
                }
                else
                {
                    //2D部分
                    if ( memcmp(sVarName, JCGpuProgram::shader_SamplerPre, SamplerPrefixLen) == 0) 
                    {
                        int varlen = strlen(sVarName);
                        if (varlen > SamplerPrefixLen) {
                            if (varlen = SamplerPrefixLen + 1) {
                                nSID = sVarName[SamplerPrefixLen] - '0';
                            }
                            else if (varlen = SamplerPrefixLen + 2) {
                                nSID = (sVarName[SamplerPrefixLen] - '0') * 10 + sVarName[SamplerPrefixLen + 1] - '0';
                            }
                            else {
                                LOGE("JCRenderer::link_shader_uniformdata shader uniform error:%s\n", sVarName);
                            }
                        }
                    }
                }
                p_ShaderInfo.m_Textures.push_back(JCShaderLink_Uniform::TEXBIND(location, nSID));
                nSampler2DNum++;
                break;
            case GL_SAMPLER_CUBE:
                nSID = nCubeSamplerNum;
                if(pUnformInfo)
                {
                    //3D部分
                    if (pMapTextureDesc) 
                    {
                        std::map<int, int>::iterator iter = pMapTextureDesc->find(pUnformInfo->m_nSemanticsEnum);
                        if (iter != pMapTextureDesc->end())
                        {
                            nSID = iter->second;
                        }
                    }
                }
                else
                {
                    //2D部分
                    if (memcmp(sVarName, JCGpuProgram::shader_CubeSamplerPre, CubeSamplerPrefixLen) == 0) 
                    {
                        int varlen = strlen(sVarName);
                        if (varlen > SamplerPrefixLen) {
                            if (varlen = SamplerPrefixLen + 1) {
                                nSID = sVarName[SamplerPrefixLen] - '0';
                            }
                            else if (varlen = SamplerPrefixLen + 2) {
                                nSID = (sVarName[SamplerPrefixLen] - '0') * 10 + sVarName[SamplerPrefixLen + 1] - '0';
                            }
                            else {
                                LOGE("JCRenderer::link_shader_uniformdata shader uniform error:%s\n", sVarName);
                            }
                        }
                    }
                }
                p_ShaderInfo.m_CubTex.push_back(JCShaderLink_Uniform::TEXBIND(location, nSID));
                nCubeSamplerNum++;
                break;
            case GL_FLOAT:  if ((pDesc/*&&pDesc->m_nType == JCNamedData::tp_f32*/)) {
                JCShaderLink_Uniform::UniformInfo uif = { ni,glUniform1fv,location,size,pDesc->m_nOffset };
                p_ShaderInfo.m_MaterialUniform.push_back(uif);
                //p_Cmds.pushCmd( fglUniform1fv(location,1,(GLfloat*)(p_pMaterial+pDesc->m_nOffset)));
            }; break;
            case GL_FLOAT_VEC2: if ((pDesc/*&&pDesc->m_nType == JCNamedData::tp_fvec2*/)) {
                JCShaderLink_Uniform::UniformInfo uif = { ni,glUniform2fv,location,size,pDesc->m_nOffset };
                p_ShaderInfo.m_MaterialUniform.push_back(uif);
            }; break;
            case GL_FLOAT_VEC3: if ((pDesc/*&&pDesc->m_nType == JCNamedData::tp_fvec3*/)) {
                JCShaderLink_Uniform::UniformInfo uif = { ni,glUniform3fv,location,size,pDesc->m_nOffset };
                p_ShaderInfo.m_MaterialUniform.push_back(uif);
            }; break;
            case GL_FLOAT_VEC4: if ((pDesc/*&&pDesc->m_nType == JCNamedData::tp_fvec4*/)) {
                JCShaderLink_Uniform::UniformInfo uif = { ni,glUniform4fv,location,size,pDesc->m_nOffset };
                p_ShaderInfo.m_MaterialUniform.push_back(uif);
                //p_Cmds.pushCmd( fglUniform4fv(location,1,(GLfloat*)((int)p_pMaterial+pDesc->m_nOffset)) );
            }; break;
            case GL_FLOAT_MAT2: if ((pDesc/*&&pDesc->m_nType == JCNamedData::tp_mat2*/)) {
                JCShaderLink_Uniform::UniformInfo uif = { ni,laya_glUniformMatrix2fv,location,size,pDesc->m_nOffset };
                p_ShaderInfo.m_MaterialUniform.push_back(uif);
            }; break;
            case GL_FLOAT_MAT3: if ((pDesc/*&&pDesc->m_nType == JCNamedData::tp_mat3*/)) {
                JCShaderLink_Uniform::UniformInfo uif = { ni,laya_glUniformMatrix3fv,location,size,pDesc->m_nOffset };
                p_ShaderInfo.m_MaterialUniform.push_back(uif);
            }; break;
            case GL_FLOAT_MAT4: if ((pDesc/*&&pDesc->m_nType == JCNamedData::tp_mat4*/)) {
                JCShaderLink_Uniform::UniformInfo uif = { ni,laya_glUniformMatrix4fv,location,size,pDesc->m_nOffset };
                p_ShaderInfo.m_MaterialUniform.push_back(uif);
            }; break;
            }
        }
        return true;
    }
    bool JCRenderer::link_shader_vertex_uniformdata(
        JCShaderLink_Vertex& p_ShaderInfoVertex,
        JCShaderLink_Uniform& p_ShaderInfoUniform,
        JCVertexDesc* p_pVertexDesc,
        JCGpuProgram* p_pGPUProgram,
        JCNamedData** p_pNamedData,
        int p_nNamedDataNum,
        std::map<int, int>* pMapTextureDesc) {
        return  link_shader_vertex(p_ShaderInfoVertex, p_pVertexDesc, p_pGPUProgram) && 
            link_shader_uniformdata(p_ShaderInfoUniform, p_pGPUProgram, p_pNamedData, p_nNamedDataNum, pMapTextureDesc);

    }
    /**
    * @brief
    *      渲染一个mesh的一部分。为了简单这个先不支持多流。
    *      因为每个group的index都是从0开始，就要glVertexAttribPointer中设置偏移，如果设置偏移，就不能静态的描述
    *      顶点信息，即JCVertexDesc中写死的偏移是无效的，需要再加上当前group的偏移。
    */
    void JCRenderer::renderMesh(JCMesh* p_pMesh, JCRenderGroupData* p_pGroup, char** p_pDataVec, int p_nDataNum) {
        if (p_pGroup->m_nEleNum <= 0)
            return;
        JCMaterial* pMaterial = (JCMaterial*)p_pGroup->m_pMaterial;
        p_pMesh->upload();
        //应用非shader相关的设置
        glBlendFuncSeparate(pMaterial->m_nBlendSrcFactor, pMaterial->m_nBlendDstFacort, pMaterial->m_nAlphaSrcFactor, pMaterial->m_nAlphaDstFacort);
        if (pMaterial->getEnableZ()) {
            glEnable(GL_DEPTH_TEST);
        }
        pMaterial->m_pGPUProgram->useProgram();
        //应用材质的应用序列
        JCShaderLink_Vertex* pShaderVertInfo = (JCShaderLink_Vertex*)p_pGroup->m_pShaderVertexInfo;
        JCShaderLink_Uniform* pShaderUinformInfo = (JCShaderLink_Uniform*)p_pGroup->m_pShaderUniformInfo;
        if (pShaderVertInfo && pShaderUinformInfo) {
            //属性
            JCShaderLink_Vertex::VecAtrrib& attribs = pShaderVertInfo->m_AttribInfo;
            for (int i = 0, sz = attribs.size(); i<sz; i++) {
                JCShaderLink_Vertex::AttribInfo& aif = attribs[i];
                glEnableVertexAttribArray(aif.nLocation);
                glVertexAttribPointer(aif.nLocation, aif.nSize, aif.nType, false,
                    aif.nStride,
                    (const GLvoid*)(p_pGroup->m_nVBBegin + aif.nOffset));//多流的话，自己保证m_nVBBegin在正确的位置。
            }
            //贴图
            for (int i = 0, sz = pShaderUinformInfo->m_Textures.size(); i<sz; i++) {
                int nSampler = pShaderUinformInfo->m_Textures[i].sampler;
                glActiveTexture(GL_TEXTURE0 + nSampler);
                if (pMaterial->getTexture(nSampler) == NULL)continue;
                //要先还原texture
                glBindTexture(GL_TEXTURE_2D, pMaterial->getTexture(nSampler)->m_pGpuTexture);
                glUniform1i(pShaderUinformInfo->m_Textures[i].location, nSampler);
            }
            //变量
            JCShaderLink_Uniform::VecUniform& uniforms = pShaderUinformInfo->m_MaterialUniform;
            for (int i = 0, sz = uniforms.size(); i<sz; i++) {
                JCShaderLink_Uniform::UniformInfo& uif = uniforms[i];
                if (uif.nDataGroupID<p_nDataNum) {
                    uif.function(uif.nLocation, uif.nNum, (const float*)((long)(p_pDataVec[uif.nDataGroupID]) + uif.nOffset));
                }
            }
        }
        //开始渲染
        int eleNum = p_pGroup->m_nEleNum;//顶点个数，或者索引个数
        if (p_pGroup->m_bHasIndex) {
            glDrawElements(GL_TRIANGLES, eleNum, GL_UNSIGNED_SHORT, (GLvoid*)p_pGroup->m_nIBBegin);
        }
        else {
            glDrawArrays(p_pGroup->m_nGeoMode, 0, eleNum);
        }
        if (pShaderVertInfo) {
            JCShaderLink_Vertex::VecAtrrib& attribs = pShaderVertInfo->m_AttribInfo;
            for (int i = 0, sz = attribs.size(); i<sz; i++) {
                JCShaderLink_Vertex::AttribInfo& aif = attribs[i];
                glDisableVertexAttribArray(aif.nLocation);
            }
        }
        //应用材质的清理序列
        //TODO 这个是用假设的方法写死还是也用link得到结果
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
