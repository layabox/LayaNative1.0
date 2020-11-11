
#ifndef __JS_FILE_SYSTEM_H__20150707__
#define __JS_FILE_SYSTEM_H__20150707__
#include "../JSInterface/JSInterface.h"

namespace laya{
	class JSFileSystem{
	public:
		static void exportJS();
		static bool exists(const char* p_pszPath );
		static bool mkdir( const char* p_pszPath );
		static bool rm(const char* p_pszFile);
		static bool rmDir(const char* p_pszPath, JSValueAsParam onprogress, JSValueAsParam oncomplete, JSValueAsParam onerror);
		static bool rmDirSync(const char* p_pszPath);
		static JsValue readBinFileSync(const char* p_pszFile);
		static JsValue readdirSync(const char* pPath);
		static JsValue lstatSync(const char* pPath);
        static bool JSWriteFileSync(const char* p_sUrl, JSValueAsParam args);
	};
}

#endif