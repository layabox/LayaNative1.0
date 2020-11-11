/**
@file			JCConchMesh.h
@brief			
@author			guo
@version		1.0
@date			2016_6_17
*/

#ifndef __JCConchMesh_H__
#define __JCConchMesh_H__
#include "JCMesh.h"
#include <map>
#include <functional>
#include "JCNamedData.h"

/** 
 * 单流的mesh。但是实际可以插入多流数据。
 * TODO  如果要添加多流的数据，必须一次把多个流都拷贝进来，然后off就成了相对于当前开始位置的off
 * @brief 为了应付本引擎的一些固定需求定制的mesh。
 */
namespace laya {
    class JCMaterialBase;
    class JCMaterial;
    class JCNamedData;
    class JCShaderLink_Vertex;
    class JCShaderLink_Uniform;
    class JCGpuProgram;
    class JCConchMesh :public JCMesh 
    {
    public:
        
        JCConchMesh();

        ~JCConchMesh();

        /** @brief 获得或者增加新的分组，根据材质，绘制类型来判断是否可以合并到当前分组中
         *  @param[in] 强制创建一个新的组
         *  @param[in] 顶点类型
         *  @param[in] Geo类型
         *  @param[in] 材质
        */
        bool getOrAddGroup(bool bForceNew, int p_nVertType, int p_nGeoMode, JCMaterialBase* p_pMtl, bool p_bHasIndex);

        /**
        *  @brief 直接push顶点。
        *  @param[in] p_nVertType 使用第几种顶点格式。
        *  @param[in] p_nGeoMode 顶点的几何类型，例如 GL_TRIANGLES
        *  @param[in] p_pMtl 材质
        *  @return
        */
        JCRenderGroupData* pushVertex(int p_nVertType, int p_nGeoMode, JCMaterialBase* p_pMtl, int p_nVertNum,std::initializer_list<float>);

        /**
        *  @brief 添加带索引的三角形。这种情况geomode固定为三角形
        *  @param[in]
        *  @param[in] relIdx 是否是相对的索引，如果是true，需要全部加上当前的顶点个数 (不要了)
        *  @return
        */
        JCRenderGroupData*  pushElements(int p_nVertType, JCMaterialBase* p_pMtl, int p_nVertNum,
            std::initializer_list<float>,std::initializer_list<unsigned short>);

        JCRenderGroupData* pushVertex(int p_nVertType, int p_nGeoMode, JCMaterialBase* p_pMtl,
            int p_nVertNum, void* p_pData, int p_nDataLen);

        /**
        *  @brief
        *  @param[in]
        *  @param [in] p_nIBLen 是buffer的长度，不是index的个数。
        *  @param [in] p_bNoMerge 强制不合并。例如有matrix的
        *  @param[out]
        *  @return
        */
        JCRenderGroupData* pushElements(
            int p_nVertType, JCMaterialBase* p_pMtl,
            int p_nVertNum, void* p_pVBData, int p_nVbLen,
            unsigned short* p_pIBData, int p_nIBLen, bool p_bNoMerge = false);

        JCRenderGroupData* pushCmd(const std::function<void(void)>& func);

        void resetData();

        void setShaderValue( int nSemantics, char* pData, int nSize);

        int getCurrentGroupPos();

        JCShaderLink_Vertex* getShaderVertexLink(int p_nVertexDesc, JCGpuProgram* p_pGpuProgram);

        JCShaderLink_Uniform* getShaderUniformLink(JCGpuProgram* p_pGpuProgram);

        /**
        *  @brief 这个是为了setIBVB准备的。
        *  @param[in] nVertType 顶点格式。具体定义看上层。
        *  @param[in] pVB 是固定格式的vertextbuffer，目前是{float x,y,u,v,r,g,b,a}
        *  @param[in] nVBOff 表示vb的偏移，即允许只拷贝vb的一部分。单位是byte
        *  @param[in] pIB short 类型的ib数据。
        *  @param[in] nIBOff
        *  @param[in] nEleNum 元素个数，就是要从ib拷贝的short的个数，例如一个四边形是6个
        *  @param[in] pMtl 这段数据使用什么材质
        *  @param[in] pTexTrans float[4] 用来的把[0,0; 1,1] 间的纹理坐标转换到大图合集空间，表示图片在大图集中的位置
        *  @return void
        */
        JCRenderGroupData* pushIBVB(int nVertType, const char* pVB, int nVBOff, const char* pIB, int nIBOff, int nEleNum, float* pObjMatrix, JCMaterialBase* pMtl,
            float* pTexTrans, int vertNum=0);
        virtual void freeGLResource();

    public:
        static int                              s_nCurrentFrameGroupNum;
        static int                              s_nCurrentFrameVertextNum;
    public:
        JCNamedData                             m_NamedDataDesc;
        char*                                   m_pNamedDataValue = nullptr;
        short                                   m_nNamedDataDescCount;      //如果m_NamedDataDesc变了，这个就会增加
    protected:
        JCRenderGroupData*                      m_pCurGroup = nullptr;
        int                                     m_nCurGroupPos = 0;
        std::map<int, JCShaderLink_Vertex*>     m_ShaderLinkCacheVert;
        std::map<int, JCShaderLink_Uniform*>    m_ShaderLinkCacheUni;
    };
}
#endif //__JCConchMesh_H__

