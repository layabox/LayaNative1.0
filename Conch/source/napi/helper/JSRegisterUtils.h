#ifndef CC_OH_JsRegister_H
#define CC_OH_JsRegister_H

#include <napi/native_api.h>
#include "NapiHelper.h"

napi_value registerFunction(napi_env env, napi_callback_info info) {
    LOGI("====begin to registerFunction!");
	napi_status status;
	napi_value exports;
	size_t argc = 2;
	napi_value args[2];

	napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

	if (argc != 2) {
		napi_throw_type_error(env, NULL, "Wrong number of arguments");
		return nullptr;
	}

	napi_valuetype valuetype;
	status = napi_typeof(env, args[0], &valuetype);
	if (status != napi_ok) {
		return nullptr;
	}
	if (valuetype != napi_string) {
		napi_throw_type_error(env, NULL, "Wrong arguments");
		return nullptr;
	}
    
    auto jsArg = args[0];
    size_t len = 0;
    status = napi_get_value_string_utf8(env, jsArg, nullptr, 0, &len);    
    std::string functionName = "";
    functionName.resize(len, '\0');
    status = napi_get_value_string_utf8(env, jsArg, (char*)functionName.data(), functionName.size() + 1, &len);    

	napi_valuetype functionType;
	status = napi_typeof(env, args[1], &functionType);
	if (status != napi_ok) {
		return nullptr;
	}
	if (functionType != napi_function) {
		napi_throw_type_error(env, NULL, "Wrong arguments");
		return nullptr;
	}    

    napi_ref fucRef;
    napi_create_reference(env, args[1], 1, &fucRef);
    
    char* name = new char[functionName.length() + 1];
    strcpy(name, functionName.c_str());
    JSFunction* jsFunction = new JSFunction(name, env, fucRef);

    JSFunction::addFunction(name, jsFunction);
       
    LOGI("begin to return!");
    return nullptr;
}

#endif //CC_OH_JsRegister_H