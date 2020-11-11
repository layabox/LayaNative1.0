/**
@file			JSGraphics.h
@brief			
@author			James
@version		1.0
@date			2016_6_14
*/

#ifndef __JSGraphics_H__
#define __JSGraphics_H__

#include "../JSInterface/JSInterface.h"
#include <Node/JCGraphics.h>

namespace laya
{
    /** 
     * @brief 
    */
    class JSGraphics :public JsObjBase, public JSObjNode
    {
    public:

        static JsObjClassInfo JSCLSINFO;

        static void exportJS();

        JSGraphics();

        ~JSGraphics();

        int getID();

    public:

        static TEXT_BASE_LINE_TYPE s_nTextBaseLineType;

        int     m_nID;

    };
}

#endif //__JSGraphics_H__

//-----------------------------END FILE--------------------------------