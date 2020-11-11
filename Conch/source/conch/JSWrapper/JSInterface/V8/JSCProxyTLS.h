//
//  JSCProxyTLS.h
//  jsc_test
//
//  Created by 蒋 宇彤 on 13-11-25.
//  Copyright (c) 2013年 蒋 宇彤. All rights reserved.
//

#ifndef jsc_test_JSCProxyTLS_h
#define jsc_test_JSCProxyTLS_h

#include <v8.h>
#include <string>
#include <util/JCCommonMethod.h>
#include <util/Log.h>
#include <sstream>

namespace laya{
    extern void JSAlert(const char* p_sBuffer);
    class __JsThrow    {
    public:
        static void Throw( const char *p_pszInfo ) {
			v8::Isolate::GetCurrent()->ThrowException(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), (0==p_pszInfo)?"unknown error":p_pszInfo));
        }
    };
    extern bool gbAlertException;
    class __JSRun  {
    public:
		// Extracts a C string from a V8 Utf8Value.
		static const char* ToCString(const v8::String::Utf8Value& value) {
		  return *value ? *value : "<string conversion failed>";
		}

        static void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch) {
            v8::HandleScope handle_scope(isolate);
            v8::String::Utf8Value exception(try_catch->Exception());
            const char* exception_string = ToCString(exception);
            v8::Handle<v8::Message> message = try_catch->Message();
            static char errInfo[2048];
            int curpos = 0;
            if (message.IsEmpty()) {
                // V8 didn't provide any extra information about this error; just
                // print the exception.
                sprintf(errInfo, "%s\n", exception_string);

                //通知全局错误处理脚本
                std::string kBuf = "if(conch.onerror){conch.onerror('";
                kBuf += UrlEncode(exception_string);
                kBuf += "','";
                kBuf += "undefined";
                kBuf += "','";
                kBuf += "undefined";
                kBuf += "','";
                kBuf += "undefined";
                kBuf += "','";
                kBuf += UrlEncode(exception_string);
                kBuf += "');};";
                __JSRun::Run(kBuf.c_str());



            }
            else {
                // Print (filename):(line number): (message).
                v8::String::Utf8Value filename(message->GetScriptResourceName());
                const char* filename_string = ToCString(filename);
                int linenum = message->GetLineNumber();
                curpos += sprintf(errInfo, "%s:%i: %s\n", filename_string, linenum, exception_string);
                // Print line of source code.
                v8::String::Utf8Value sourceline(message->GetSourceLine());
                int start = message->GetStartColumn();
                int end = message->GetEndColumn();
                const char* sourceline_string = ToCString(sourceline);
                if (strlen(sourceline_string) > 100) {
                    if (end - start < 100) {
                        int st = start - 50>0 ? start - 50 : 0;
                        memcpy(errInfo + curpos, sourceline_string + st, 100);
                        curpos += 100;
                    }
                }
                else {
                    curpos += sprintf(errInfo + curpos, "%s\n", sourceline_string);
                    // Print wavy underline (GetUnderline is deprecated).
                    for (int i = 0; i < start; i++) {
                        curpos += sprintf(errInfo + curpos, " ");
                    }
                    for (int i = start; i < end; i++) {
                        curpos += sprintf(errInfo + curpos, "^");
                    }
                }
                curpos += sprintf(errInfo + curpos, "\n");
                v8::String::Utf8Value stack_trace(try_catch->StackTrace());
                if (stack_trace.length() > 0) {
                    const char* stack_trace_string = ToCString(stack_trace);
                    curpos += sprintf(errInfo + curpos, "%s", stack_trace_string);
                }

                //通知全局错误处理脚本
                std::string kBuf = "if(conch.onerror){conch.onerror('";
                kBuf += UrlEncode(exception_string);
                kBuf += "','";
                kBuf += UrlEncode(filename_string);
                kBuf += "','";
                //kBuf += std::to_string(linenum);
                std::ostringstream os;
                os << linenum;
                kBuf += os.str();
                kBuf += "','";
                kBuf += "undefined";
                kBuf += "','";
                kBuf += UrlEncode(errInfo);
                kBuf += "');};";
                __JSRun::Run(kBuf.c_str());

            }
            
            if (gbAlertException)
                JSAlert(errInfo);
            else {
                LOGE("==JSERROR:\n%s", errInfo);
            }
        }

        static bool Run( const char *p_pszScript ) {
			v8::Isolate* isolate = v8::Isolate::GetCurrent();
			v8::HandleScope handle_scope(isolate);
			v8::TryCatch try_catch;

			v8::Handle<v8::String> source = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), p_pszScript);

			v8::Handle<v8::Script> script = v8::Script::Compile(source);
			if( script.IsEmpty() ){
				//打印编译错误信息
				printf("---Compile script error---\n");
				ReportException(isolate, &try_catch);
				return false;
			}

			v8::Handle<v8::Value> res = script->Run();

			if( !res.IsEmpty() && !res->IsUndefined() )
			{
				v8::String::Utf8Value ascii(res);
				printf("run result: [%s]\n", *ascii);
			}

			if( try_catch.HasCaught()){
				printf("---run script error---\n");
				ReportException(isolate, &try_catch);
			}
            
            return true;
        }
    };
}

#endif
