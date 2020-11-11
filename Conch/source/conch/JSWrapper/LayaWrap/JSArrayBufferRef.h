/**
@file			JSArrayBufferRef.h
@brief			
@author			James
@version		1.0
@date			2017_11_29
*/

#ifndef __JSArrayBufferRef_H__
#define __JSArrayBufferRef_H__

#include <stdio.h>
#include "../JSInterface/JSInterface.h"
#include "resource/Audio/JCAudioInterface.h"

/** 
 * @brief 
*/
namespace laya 
{
    class JSArrayBufferRef : public JsObjBase, public JSObjNode
    {
    public:
	    
        static JsObjClassInfo JSCLSINFO;

	    static void exportJS();

        JSArrayBufferRef();

        ~JSArrayBufferRef();

        int getID();

        int setID(int nID);

    public:

        int     m_nID;

    };
}
//------------------------------------------------------------------------------


#endif //__JSArrayBufferRef_H__

//-----------------------------END FILE--------------------------------