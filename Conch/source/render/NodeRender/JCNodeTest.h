
/**
@file			NodeTest.h
@brief			
@author			guo
@version		1.0
@date			2016_5_30
*/

#ifndef __NodeTest_H__
#define __NodeTest_H__

/** 
 * @brief 2D Node的渲染流程演示
 */
namespace laya {
    class JCHtml5Context;
    class JCNodeTest {
    protected:

    public:
        void build();

        void bgcolor( int nColor );

        void image(int nImageID);

        void render(JCHtml5Context* ctx);
        /** 
         *  @brief 如果是renderor主动的话，就通过这个函数获得mesh，例如
         *         JCDynamicMesh* pMesh = pNode->getMesh();
         *         pMesh->setGlobalRenderDataDesc(pGlobalDataDesc);
         *         pMesh->render(pGlobalData);
         *  @return 
         */
        //JCDynamicMesh* getMesh();

        int m_nColor;
        int m_nImageID;

        int m_nType;
    };
}


#endif //__NodeTest_H__
