/**
@file			JSParticleTemplate2D.h
@brief			
@author			hugao
@version		1.0
@date			2016_10_31
*/

#ifndef __JSParticleTemplate2D_H__
#define __JSParticleTemplate2D_H__


//包含头文件
//------------------------------------------------------------------------------
//包含头文件
#include "../JSInterface/JSInterface.h"

/** 
 * @brief 
*/
namespace laya
{
    class JSParticleTemplate2D :public JsObjBase, public JSObjNode
    {
    public:
        static JsObjClassInfo JSCLSINFO;
        static void exportJS();

        JSParticleTemplate2D();

        ~JSParticleTemplate2D();
    public:

        int getID();

    public:
        int     m_nID;
    };
}
//------------------------------------------------------------------------------


#endif //__JSParticleTemplate2D_H__

//-----------------------------END FILE--------------------------------