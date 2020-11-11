/**
@file			JSGlobalExportCFun.h
@brief			
@author			wyw
@version		1.0
@date			2013_11_12
*/

#ifndef __JSGlobalExportCFun_H__
#define __JSGlobalExportCFun_H__

//包含头文件
#include "../JSInterface/JSInterface.h"
 
namespace laya {
	
    void JSPrint( const char* p_sBuffer );

    void LayaAlert(const char* p_sBuffer);

	void JSAlert( const char* p_sBuffer );

	void evalJS( const char* p_sSource );

	void JSGlobalExportC();
    
    bool IsStreamMode();

}
#endif //__JSGlobalExportCFun_H__

//-----------------------------END FILE--------------------------------
