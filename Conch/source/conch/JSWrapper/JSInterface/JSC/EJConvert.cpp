#import "EJConvert.h"

// JSValueToNumberFast blindly assumes that the given JSValueRef is a
// a number. Everything else will be silently converted to 0.
// This functions comes in a 64bit and 32bit flavor, since the NaN-Boxing
// in JSC works a bit differently on each platforms. For an explanation
// of the taggging refer to JSC/runtime/JSCJSValue.h
// The 32bit version just calls the normal JSValueToNumber() function
// and is thus a lot slower.

double JSValueToNumberFast(JSContextRef ctx, JSValueRef v) {
	#if __LP64__ // arm64 version
		union {
			int64_t asInt64;
			double asDouble;
			struct { int32_t asInt; int32_t tag; } asBits;
		} taggedValue = { .asInt64 = (int64_t)v };
		
		#define DoubleEncodeOffset 0x1000000000000ll
		#define TagTypeNumber 0xffff0000
		#define ValueTrue 0x7
		
		if( (taggedValue.asBits.tag & TagTypeNumber) == TagTypeNumber ) {
			return taggedValue.asBits.asInt;
		}
		else if( taggedValue.asBits.tag & TagTypeNumber ) {
			taggedValue.asInt64 -= DoubleEncodeOffset;
			return taggedValue.asDouble;
		}
		else if( taggedValue.asBits.asInt == ValueTrue ) {
			return 1.0;
		}
		else {
			return 0; // false, undefined, null, object
		}
	#else // armv7 version
		return JSValueToNumber(ctx, v, NULL);
	#endif
}

void JSValueUnprotectSafe( JSContextRef ctx, JSValueRef v ) {
	if( ctx && v ) {
		JSValueUnprotect(ctx, v);
	}
}