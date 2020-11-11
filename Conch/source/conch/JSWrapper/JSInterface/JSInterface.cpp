/**
@file			JSInterface.cpp
@brief
@author			wyw
@version		1.0
@date			2014_7_29
*/

#include "JSInterface.h"
#include "../LayaWrap/JSConchConfig.h"
#ifdef JS_JSC
    //#include <JavaScriptCore/JSBasePrivate.h>
#endif

namespace laya 
{
    unsigned int gnJSClsID = 1;
    void AdjustAmountOfExternalAllocatedMemory(int p_nMemorySize)
    {
#ifdef JS_V8
        v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(p_nMemorySize);
#elif JS_JSC
        //JSReportExtraMemoryCost(__TlsData::GetInstance()->GetCurContext(), p_nMemorySize);
#endif
    }
    JsObjClassInfo JSObjNode::JSCLSINFO = { "JSObjNode",NULL,0 };
    simpList*     JSObjNode::s_pListJSObj=NULL;
    JSObjNode::JSObjNode() 
    {
#ifdef JS_V8
        s_pListJSObj->push_back(this);
#endif
        m_pClsInfo = &JSObjNode::JSCLSINFO;
    }
    JSObjNode::~JSObjNode() 
    {
#ifdef JS_V8
        s_pListJSObj->delNode(this);
#endif
    }
    JsValue getNativeObj(JSValueAsParam p_pJsObj, char* p_strName) 
    {
#ifdef JS_V8
        if (p_pJsObj->IsObject()) {
            v8::Local<v8::Object> pobj = v8::Local<v8::Object>::Cast(p_pJsObj);
            JsValue nativeObj = pobj->Get(JSP_TO_JS(char*, p_strName));
            if (!nativeObj.IsEmpty() && nativeObj->IsObject()) {
                return nativeObj;
            }
        }
        return p_pJsObj;
#elif JS_JSC
        JSContextRef ctx = __TlsData::GetInstance()->GetCurContext();
        if (JSValueIsObject(ctx, p_pJsObj)) {
            JSObjectRef obj = JSValueToObject(ctx, p_pJsObj, nullptr);
            if (obj != nullptr) {
                JSStringRef name = JSStringCreateWithUTF8CString(p_strName);
                JSValueRef ret = JSObjectGetProperty(ctx, obj, name, nullptr);
                JSStringRelease(name);
                if (!JSValueIsNull(ctx, ret) && !JSValueIsUndefined(ctx, ret)) {
                    return ret;
                }
            }
        }
        return p_pJsObj;
#endif
    }
    
    JsArrayBufferData::JsArrayBufferData(bool needDelete):data(nullptr),len(0),needDelete(needDelete)
    {
    }
    JsArrayBufferData::~JsArrayBufferData()
    {
        if (needDelete && data != nullptr)
        {
            delete[] data;
            data = nullptr;
        }
    }
    bool isSupportTypedArrayAPI()
    {
#ifdef JS_JSC
        static bool isSupported = false;
        static bool isInited = false;
        if (!isInited)
        {
            float version = JSConchConfig::getInstance()->getCurrentDeviceSystemVersion();
            isSupported = (version >= 10.0f);
            isInited = true;
        }
        return isSupported;
#else
        return true;
#endif
    }
}



//-----------------------------END FILE--------------------------------
