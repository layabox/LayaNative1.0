/**
@file			JSZip.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_18
*/

#ifndef __JSZip_H__
#define __JSZip_H__


//包含头文件
//------------------------------------------------------------------------------
#include "../JSInterface/jsobjbase.h"
#include "../JSInterface/JSInterface.h"

namespace laya {
	class JSArrayBuffer;
	class JSZip: public JsObjBase, public JSObjNode {
	public:
		static JsObjClassInfo JSCLSINFO;
		static void exportJS();
		JSZip();
		~JSZip();
		bool openZip(const char* p_pszFile);
		void enumFiles( JSValueAsParam pObj);
		bool setSrc(const char* p_pSrc);
		JsValue readFile(int idx);
		JsValue readFileExp(int idx);
		void close();
	protected:
		void*	m_pZip;
	};
}
//------------------------------------------------------------------------------


#endif //__JSZip_H__

//-----------------------------END FILE--------------------------------