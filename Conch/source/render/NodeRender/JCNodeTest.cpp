
#include "JCNodeTest.h"
#include "../Html5Render/JCHtml5Context.h"

namespace laya {
    void JCNodeTest::build() 
    {
        bool bColor = false;
        bool bImage = false;
        if (bColor)
        {
            //RectGeometry::VertexInfo* pVertextInfo = (RectGeometry::VertexInfo*)(m_pMesh->m_pMesh->getVBMem(0)->getBuffer());
        }
        if (bImage)
        {

        }
        //m_pMesh->pushGeo();
    }

    void JCNodeTest::render(JCHtml5Context* ctx) {
        //ctx->m_pDynamicMesh->pushDynamicMesh(m_pMesh);
    }
    void JCNodeTest::bgcolor(int nColor)
    {
        m_nColor = nColor;
        m_nType |= 1;
    }

    void JCNodeTest::image(int nImageID)
    {
        m_nImageID = nImageID;
        m_nType |= 2;
    }
}