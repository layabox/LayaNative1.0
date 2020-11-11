/**
@file			JSConsole.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_18
*/

#ifndef __JSConsole_H__
#define __JSConsole_H__


//包含头文件
//------------------------------------------------------------------------------
#include <stdio.h>
#include "../JSInterface/JSInterface.h"

/** 
 * @brief 
*/
namespace laya 
{
class JSConsole:public JsObjBase
{
public:
	static JsObjClassInfo JSCLSINFO;
	static void exportJS();

	JSConsole();

	~JSConsole();

	static JSConsole* GetInstance();
	static void DelInstance();

public:

	void log(int p_nType , const char* p_sBuffer );


public:
	static JSConsole*		m_spConsole;

};
}
//------------------------------------------------------------------------------


#endif //__JSConsole_H__

//-----------------------------END FILE--------------------------------