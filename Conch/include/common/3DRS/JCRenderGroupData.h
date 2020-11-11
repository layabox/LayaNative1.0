/**
@file			JCRenderGroupData.h
@brief			
@author			guo
@version		1.0
@date			2016_5_31
*/

#ifndef __JCRenderGroupData_H__
#define __JCRenderGroupData_H__

#include "JCMesh.h"
#include "../geom/Rectangle.h"
#include <functional>

namespace laya 
{
    class JCMaterialBase;
    class JCRenderGroupData{
    public:
        JCMesh*             m_pMesh=nullptr;                    ///<Mesh
        void*               m_pShaderVertexInfo = nullptr;      //shaderInfo外部设置，只是提供位置保存，本身并不知道什么意义
        void*               m_pShaderUniformInfo = nullptr;
        JCMaterialBase*     m_pMaterial=nullptr;                //material
        int                 m_nVBBegin=0;                       //vb开始的位置，这个会用来设置 glVertexAttribPointer ，这样就能避免修改ib
        int                 m_nIBBegin = 0;
        int                 m_nEleNum = 0;
        int                 m_nGeoMode= GL_TRIANGLES;           //例如 GL_TRIANGLES
        int                 m_nVertexDesc=0;                    //使用 m_pMesh中的哪个vertexdesc
        bool                m_bHasIndex = false;
        bool                m_bHasObjMat=false;
        float               m_matObject[16];
        int                 m_nVertNum = 0;                     //group中的顶点个数。例如修改index需要。
        int                 m_nNamedDataCount = 0;              //为了3D用的，存储setShaderValue的个数的
        std::vector<std::function<void(void)>> m_SetFunc;       //不是每个都有的设置函数
        //bool                m_bScissor = false;
        //float               m_ClipRect[4] = { 0 };
    };
}
//------------------------------------------------------------------------------


#endif //__JCRenderGroupData_H__

//-----------------------------END FILE--------------------------------