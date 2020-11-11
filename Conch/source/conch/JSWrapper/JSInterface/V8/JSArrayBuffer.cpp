#ifdef JS_V8

#include "JSArrayBuffer.h"
#include <util/JCMemorySurvey.h>
#include <util/Log.h>
#include "../JSInterface.h"
#include "JSCProxyTrnasfer.h"

namespace laya {
	//TODO 这里没有考虑多线程的问题，如果js要支持多线程的话，需要修改。
	struct JsStrBuff {
		char* _buff;
		int _len;
		static std::vector<JsStrBuff> jsstrbuffs;
		static int curIdx;
		JsStrBuff() {
			_buff = NULL;
			_len = 0;
		}

		static JsStrBuff& getAData() {
			if (JsStrBuff::curIdx >= (int)JsStrBuff::jsstrbuffs.size()) {
				JsStrBuff::jsstrbuffs.push_back(JsStrBuff());
				JsStrBuff::curIdx++;
				return JsStrBuff::jsstrbuffs.back();
			}
			else {
				return JsStrBuff::jsstrbuffs[JsStrBuff::curIdx++];
			}
		}
	};
	std::vector<JsStrBuff> JsStrBuff::jsstrbuffs;
	int JsStrBuff::curIdx = 0;

	//
	void resetJsStrBuf() {
		JsStrBuff::curIdx = 0;
	}
	char* JsCharToC(Local<Value> p_vl) {
		int len = 0;
		Local<String> str = p_vl->ToString();
		len = str->Utf8Length();
		if (len <= 0)
			return "";
		JsStrBuff& curdata = JsStrBuff::getAData();
		char*& tocharBuf = curdata._buff;
		int& tocharBufLen = curdata._len;

		//tocharBuf= new char[len + 1];
		if (len > tocharBufLen) {
			tocharBufLen = len;
			if (tocharBuf != NULL)
				delete[] tocharBuf;
			tocharBuf = new char[len+1];
		}
		else {
			//如果占用空间太大，也要重新分配
			if (tocharBufLen > 1024 ) {
				tocharBufLen = len;
				if (tocharBuf != NULL)
					delete[] tocharBuf;
				tocharBuf = new char[len+1];
			}
		}
		str->WriteUtf8(tocharBuf);
		return tocharBuf;
	}
}

namespace laya{
    ArrayBufferAllocator::ArrayBufferAllocator() {
    }

    ArrayBufferAllocator::~ArrayBufferAllocator() {
    }

	void* ArrayBufferAllocator::Allocate(size_t length) { 
		char* pRet = new char[length]; 
		memset(pRet,0,length); 
		return pRet; 
	}
	void* ArrayBufferAllocator::AllocateUninitialized(size_t length){
		char* pRet = new char[length]; 
		return pRet;
	};
	void ArrayBufferAllocator::Free(void* data, size_t length) 
    {
        if (data != NULL || length > 0)
        {
            delete[](data);
        }
        else
        {
            LOGI("ArrayBufferAllocator::Free data=%d length=%d",(intptr_t)data, length);
        }
	}

	ArrayBufferAllocator* ArrayBufferAllocator::getInstance(){
		return new ArrayBufferAllocator();
	}

	v8::Local<v8::ArrayBuffer> createJSAB(char* pData, int len) {
		v8::Local<v8::ArrayBuffer> ab = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), len);
        v8::ArrayBuffer::Contents contents = ab->GetContents();// ab->Externalize();
		char* pPtr = (char*)contents.Data();
		memcpy(pPtr, pData, len);
		//Externalize 以后会减去内存占用，导致不能正确GC，所以再给加回来。不知道管理ArrayBuffer的正确方法是什么。
		//v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(len);
		return ab;
	}

    v8::Local<v8::ArrayBuffer> createJSABAligned(char* pData, int len) {
        int asz = (len + 3) & 0xfffffffc;
        v8::Local<v8::ArrayBuffer> ab = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), asz);
        v8::ArrayBuffer::Contents contents = ab->GetContents();// ab->Externalize();
        char* pPtr = (char*)contents.Data();
        memcpy(pPtr, pData, len);
        //ArrayBuffer 自己已经初始化为0了
        //Externalize 以后会减去内存占用，导致不能正确GC，所以再给加回来。不知道管理ArrayBuffer的正确方法是什么。
        //v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(asz);
        return ab;
    }

    bool extractJSAB(JsValue jsval, JsArrayBufferData& data) {
		v8::Local<v8::ArrayBuffer> ab;
		if (jsval->IsArrayBufferView()) {
			v8::Local<v8::ArrayBufferView> abv = v8::Local<v8::ArrayBufferView>::Cast(jsval);
            data.len = abv->ByteLength();
			ab = abv->Buffer();
            v8::ArrayBuffer::Contents contents = ab->GetContents();
            //len = contents.ByteLength();  这种情况下，用view的长度，因为可能多个view公用一个大buffer
            data.data = (char*)contents.Data()+abv->ByteOffset();
        }
		else if (jsval->IsArrayBuffer()) {
			ab = v8::Local<v8::ArrayBuffer>::Cast(jsval);
            v8::ArrayBuffer::Contents contents = ab->GetContents();
            data.len = contents.ByteLength();
            data.data = (char*)contents.Data();
        }
		else {
			data.data = NULL;
			data.len = 0;
			return false;
		}

        /*
		if (ab->IsExternal()) {
			v8::ArrayBuffer::Contents contents = ab->GetContents();
			len = contents.ByteLength();
			data = (char*)contents.Data();
		}
		else {
			v8::ArrayBuffer::Contents contents = ab->Externalize();
			len = contents.ByteLength();
			v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(len);
			data = (char*)contents.Data();
		}
        */
		return true;
	}

}
#endif