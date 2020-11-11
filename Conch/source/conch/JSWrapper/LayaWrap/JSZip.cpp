/**
@file			JSZip.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_18
*/

//包含头文件
#define NULL 0
#include "JSZip.h"
#include "util/Log.h"
#include <zip.h>

namespace laya 
{
	ADDJSCLSINFO(JSZip, JSObjNode);
	JSZip::JSZip() {
		m_pZip = NULL;
	}

	JSZip::~JSZip() {
		if (m_pZip) {
			zip_close((zip*)m_pZip);
		}
	}

	bool JSZip::openZip(const char* p_pszFile) {
		int error =0;
		zip* pZip = zip_open(p_pszFile, 0, &error);
		if (!pZip) {
			switch (error) {
			case ZIP_ER_NOENT:
				LOGE("JSZip::openZip The file specified by path does not exist and ZIP_CREATE is not set [%d]\n", error);
				break;
			case ZIP_ER_EXISTS:
				LOGE("JSZip::openZip The file specified by path exists and ZIP_EXCL is set [%d]\n", error);
				break;
			case ZIP_ER_INVAL:
				LOGE("JSZip::openZip The path argument is NULL [%d]\n", error);
				break;
			case ZIP_ER_NOZIP:
				LOGE("JSZip::openZip The file specified by path is not a zip archive [%d]\n", error);
				break;
			case ZIP_ER_OPEN:
				LOGE("JSZip::openZip The file specified by path could not be opened [%d]\n", error);
				break;
			case ZIP_ER_READ:
				LOGE("JSZip::openZip A read error occurred; see errno for details [%d]\n", error);
				break;
			case ZIP_ER_SEEK:
				LOGE("JSZip::openZip The file specified by path does not allow seeks [%d]\n", error);
				break;
			default:
				LOGE("JSZip::openZip unknown err [%d]\n", error);
				break;
			}
			return false;
		}
		m_pZip = pZip;
		return true;
	}

	bool JSZip::setSrc(const char* p_pSrc) {
        close();
		return openZip(p_pSrc);
	}

	JsValue JSZip::readFileExp(int idx) {
		return readFile(idx);
	}

	JsValue JSZip::readFile(int idx) {
		if (!m_pZip)//TODO 抛出js异常
			return JSP_TO_JS_NULL;
		zip* pZip = (zip*)m_pZip;
		struct zip_stat sb;
		zip_stat_init(&sb);
		if (zip_stat_index(pZip, idx, 0, &sb) < 0) {
		}else {
			zip_file* pzf = zip_fopen_index(pZip, idx, 0);
			if (pzf == NULL) {
				int a = 0;
			}
			else {
				//laya::JSArrayBuffer* pAB = laya::JSArrayBuffer::create(sb.size);
				char* pBuff = new char[(unsigned int)sb.size];
				zip_fread(pzf, pBuff, sb.size);
				zip_fclose(pzf);
				JsValue ab = createJSAB(pBuff,(int)sb.size);
				delete [] pBuff;
				//return pAB;
				return ab;
			}
		}
		return JSP_TO_JS_NULL;
	}

	void JSZip::enumFiles( JSValueAsParam pObj) {
		if (!m_pZip)//TODO 抛出js异常
			return;
		zip* pZip = (zip*)m_pZip;
		int entNum = zip_get_num_files(pZip);
		for (int i = 0; i < entNum; i++) {
			struct zip_stat sb;
			zip_stat_init(&sb);
			if (zip_stat_index(pZip, i, 0, &sb) < 0) {
			}
			else {
				//printf("name=%s,size=%lld\n", sb.name, sb.size);
				bool bDir = sb.name[strlen(sb.name) - 1] == '/';
#ifdef JS_V8
				if (!pObj->IsFunction())return;
				v8::Local<v8::Function> jsfun = v8::Local<v8::Function>::Cast(pObj);
				callJsFunc(jsfun, i, sb.name, bDir, sb.size);
#elif JS_JSC
                JSContextRef ctx = laya::__TlsData::GetInstance()->GetCurContext();
                if (!JSValueIsObject(ctx, pObj))return;
                if (!JSObjectIsFunction(ctx, JSValueToObject(ctx, pObj, nullptr)))return;
                callJsFunc(pObj, i, sb.name, bDir, sb.size);
#endif
			}
		}
		//zip_close(pZip);
	}
	void JSZip::close() {
		if (m_pZip) {
			zip_close((zip*)m_pZip);
			m_pZip = NULL;
		}
	}
	void JSZip::exportJS() 
    {
        JSP_CLASS("ZipFile", JSZip);
		JSP_ADD_METHOD("setSrc", JSZip::setSrc);
		JSP_ADD_METHOD("forEach", JSZip::enumFiles);
		JSP_ADD_METHOD("readFile1", JSZip::readFileExp);			//这个现在有问题,在导出函数中调用这个无法多次调用。例如enumFiles中调用,只有一次有效。
		JSP_ADD_METHOD("readFile", JSZip::readFile);
		JSP_ADD_METHOD("close", JSZip::close);
		JSP_INSTALL_CLASS("ZipFile", JSZip);
	}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------