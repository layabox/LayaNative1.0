/**
@file			JSContext.h
@brief			
@author			wyw
@version		1.0
@date			2013_11_12
*/

#ifndef __JSLayaContext_H__
#define __JSLayaContext_H__

//包含头文件
#include "../JSInterface/JSInterface.h"

/** 
 * @brief 
*/
namespace laya 
{
    enum 
    {
        getpixelsid,
        tobase64id,
    };
class JSLayaContext:public JsObjBase, public JSObjNode
{
public:
	static JsObjClassInfo JSCLSINFO;

    static void exportJS();

    JSLayaContext();

    ~JSLayaContext();

    void setSize(int w,int h);

    JsValue measureText(const char* sFont, const char* sText);
    
    void setGetPixelsCallback(JSValueAsParam p_pCallback);
    void setToBase64Callback(JSValueAsParam p_pCallback);

    int getID();
    
    void onGetPixelsCallbackCallJS(unsigned char *pPixels, int nLength, int w, int h);
    void onToBase64CallbackCallJS(const char* pBase64);
public:

    int     m_nWidth;       //宽
    int     m_nHeight;      //高
    int     m_nID;          //id
    JsObjHandle m_pGetPixelsCallBackFunction;
    JsObjHandle m_pToBase64CallBackFunction;
    static std::map<int, JSLayaContext*> m_sMapContexts;
};
}
#endif //__JSContext_H

//-----------------------------END FILE--------------------------------