#ifndef __EJ_CONVERT_
#define __EJ_CONVERT_
#include "JavaScriptCore/JavaScriptCore.h"


#ifdef __cplusplus
extern "C" {
#endif

double JSValueToNumberFast( JSContextRef ctx, JSValueRef v );
void JSValueUnprotectSafe( JSContextRef ctx, JSValueRef v );

static inline void *JSValueGetPrivate(JSValueRef v) {
	// On 64bit systems we can not safely call JSObjectGetPrivate with any
	// JSValueRef. Doing so with immediate values (numbers, null, bool,
	// undefined) will crash the app. So we check for these first.

	#if __LP64__
		return !((int64_t)v & 0xffff000000000002ll)
			? JSObjectGetPrivate((JSObjectRef)v)
			: NULL;
	#else
		return JSObjectGetPrivate((JSObjectRef)v);
	#endif
}

#ifdef __cplusplus
}
#endif
#endif
