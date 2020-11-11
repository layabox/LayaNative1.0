/**
@file			JSCustomContext.h
@brief			
@author			James
@version		1.0
@date			2016_6_17
*/

#ifndef __JSCustomContext_H__
#define __JSCustomContext_H__

//包含头文件
#include "../JSInterface/JSInterface.h"

/** 
 * @brief 
*/
namespace laya
{
    class JSCustomContext :public JsObjBase, public JSObjNode
    {
    public:
        static JsObjClassInfo JSCLSINFO;

        static void exportJS();

        JSCustomContext();

        ~JSCustomContext();

        void setSize(int w, int h);

        JsValue measureText(const char* sFont, const char* sText);

        int getID();

    public:

        int     m_nWidth;       //宽
        int     m_nHeight;      //高
        int     m_nID;          //id

    };
}

#endif //__JSCustomContext_H__

//-----------------------------END FILE--------------------------------