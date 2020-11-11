/**
@file			JCConchMesh2D.h
@brief			
@author			James
@version		1.0
@date			2016_12_7
*/

#ifndef __JCConchMesh2D_H__
#define __JCConchMesh2D_H__

#include "JCConchMesh.h"
#include <map>
#include <functional>
#include "JCNamedData.h"

namespace laya {
    class JCNamedData;
    class JCConchMesh2D :public JCConchMesh 
    {
    public:
        enum VertexType
        {
            XYUVRGBA = 1,
            V4V4 = 2,
            XYUV = 3,
            PARTICLE2D,
        };
        JCConchMesh2D();
        ~JCConchMesh2D();
        JCRenderGroupData* pushParticle2D(int p_nMaxPartical, int p_nFirstNewElement, int p_nFirstActiveElement, int p_nFirstFreeElement, const char* pVB, const char* pIB, float* pObjMatrix, JCMaterial* pMtl);
        void render(char* pGlobalRenderData);
        void renderToMemoryCanvas(char* pGlobalRenderData, float fAlpha);
        void setNamedData(JCNamedData* pMtl, JCNamedData* pGlobal);
    public:
        JCNamedData*    m_pAllNamedData[2]; //0是材质。1是全局信息。 2 是为setIBVB准备的。如果需要，可以再添加    
    };
}
//------------------------------------------------------------------------------


#endif //__JCConchMesh2D_H__

//-----------------------------END FILE--------------------------------