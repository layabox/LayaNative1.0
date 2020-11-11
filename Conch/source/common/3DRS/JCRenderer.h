/**
@file			JCRenderer.h
@brief			
@author			guo
@version		1.0
@date			2016_5_31
*/

#ifndef __JCRenderer_H__
#define __JCRenderer_H__

#include <vector>
#include <map>
#include <string>

namespace laya{

    //几个固定模式的渲染流程。为了速度。
    class JCMesh;
    class JCVertexDesc;
    class JCNamedData;
    class JCMaterial;
    class JCGpuProgram;
    class JCShaderLink_Vertex {
    public:
        struct AttribInfo {
            int nStream;    //这个与JCMesh的stream关联起来
            int nLocation;  //location in shader
            int nSize;
            int nType;
            int nOffset;
            int nStride;
        };
        typedef std::vector<AttribInfo> VecAtrrib;
        int                     m_nProgramID;
        VecAtrrib               m_AttribInfo;
        unsigned int            m_EnabledVertexAttrib = 0;
    };

    class JCShaderLink_Uniform {
    public:
        struct TEXBIND {
            int location;
            int sampler;    //0,1,2...
            TEXBIND(int a, int b) {
                location = a; sampler = b;
            }
        };
        struct UniformInfo {
            int nDataGroupID;        //4 属于哪个运行时数据组。
                                     //全部用glUniformXXv的方法传递
            #ifdef WIN32
            typedef void(__stdcall* UNIFORMPROC)(int, int, const float*);
            #else
            typedef void(*UNIFORMPROC)(int, int, const float*);
            #endif
            UNIFORMPROC    function;
            int nLocation;
            int nNum;
            int nOffset;
        };
        typedef std::vector<UniformInfo> VecUniform;
        int                     m_nProgramID;
        std::vector<TEXBIND>    m_Textures;
        std::vector<TEXBIND>    m_CubTex;
        VecUniform              m_MaterialUniform;    //与material绑定的变量
    };
    enum GL_CAPS
    {
        GLC_NONE = 0,
        GLC_TEXTURE_COMPRESSION_PVR = 1 << 1,
        GLC_TEXTURE_COMPRESSION_ETC1 = 1 << 2,
        GLC_TEXTURE_COMPRESSION_ETC2 = 1 << 3,
        GLC_NOPT = 1 << 4,
        GLC_TEXTURE_TPG = 1 << 5,
    };
    class JCRenderGroupData;
    class JCRenderer{
    public:
        static void init();
        /**
         *  @brief 根据顶点格式和材质格式给 p_ShaderInfo 赋值，p_ShaderInfo这个会用在实际渲染中。
         *         有了 p_ShaderInfo，就可以避免render的时候不断查询shader的变量等信息。
         *         p_pNamedData 和 p_pVertexDesc 都是类型定义，不是实际数据。
         *         判断使用哪个texture的规则：
         *              1. 如果uniform命名符合 g_Texn的规则，则n就是表示第几个贴图。
         *              2. 否则，第一个出现的sampler就用第一个贴图。
         *  @param[out] p_ShaderInfo 输出连接结果。
         *  @param[in] p_pVertexDesc 顶点的描述。
         *  @param[in] p_nGPUProgram shader。
         *  @param[in] p_pNamedData 给shader提供数据的对象的描述，一般是材质的描述。这一个JCNamedData*的数组。
         *  @param[in] p_nNamedDataNum p_pNamedData中包含几个描述。
         *  @return
         */
        static bool link_shader_vertex(JCShaderLink_Vertex& p_ShaderInfo, JCVertexDesc* p_pVertexDesc, JCGpuProgram* p_pGPUProgram);
        static bool link_shader_uniformdata(JCShaderLink_Uniform& p_ShaderInfo, JCGpuProgram* p_pGPUProgram, 
            JCNamedData** p_pNamedData, int p_nNamedDataNum,std::map<int,int>* pMapTextureDesc);
        static bool link_shader_vertex_uniformdata(
            JCShaderLink_Vertex& p_ShaderInfoVertex, 
            JCShaderLink_Uniform& p_ShaderInfoUniform,
            JCVertexDesc* p_pVertexDesc,
            JCGpuProgram* p_pGPUProgram, 
            JCNamedData** p_pNamedData, 
            int p_nNamedDataNum,
            std::map<int, int>* pMapTextureDesc);
        static void renderMesh(JCMesh* p_pMesh, JCRenderGroupData* p_pGroup, char** p_pDataVec, int p_nDataNum);
        //static void renderMesh1(JCMesh* p_pMesh, int nIdxBegin, int nIdxEnd, int gpuProgram, JCShaderLinkInfo* pLinkInfo,
        //    int* pTexes, int nTexNum, int* pCubeTexes, int nCubeTexNum, char** pRundDataArr, int p_nRunDataNum);
        //打开的顶点属性。最多允许32个。标准好像是16个。
        static unsigned int s_nLastVertexAttrib;
        static int s_nGLCaps;
    };
}
//------------------------------------------------------------------------------


#endif //__JCRenderer_H__

//-----------------------------END FILE--------------------------------
