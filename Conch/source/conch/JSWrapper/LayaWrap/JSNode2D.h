/**
@file			JSNode2D.h
@brief			
@author			James
@version		1.0
@date			2016_6_13
*/

#ifndef __JSNode2D_H__
#define __JSNode2D_H__

#include "../JSInterface/JSInterface.h"

namespace laya
{
    /** 
     * @brief 
    */
    class JSNode2D :public JsObjBase, public JSObjNode
    {
    public:

        static JsObjClassInfo JSCLSINFO;

        static void exportJS();

        JSNode2D();

        ~JSNode2D();

        int getID();

        void setRootNode();

        void setFPSNode();

        void setLoadingViewNode();

    public:

        int     m_nID;

    };
}

#endif //__JSNode2D_H__

//-----------------------------END FILE--------------------------------