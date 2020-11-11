#ifndef __EJ_CONVERT_TYPED_ARRAY_H_
#define __EJ_CONVERT_TYPED_ARRAY_H_
#include <JavaScriptCore/JavaScriptCore.h>
#include <vector>
namespace laya
{
/*!
@enum JSType
@abstract     A constant identifying the Typed Array type of a JSValue.
@constant     kJSTypedArrayTypeNone                 Not a Typed Array.
@constant     kJSTypedArrayTypeInt8Array            Int8Array
@constant     kJSTypedArrayTypeInt16Array           Int16Array
@constant     kJSTypedArrayTypeInt32Array           Int32Array
@constant     kJSTypedArrayTypeUint8Array           Int8Array
@constant     kJSTypedArrayTypeUint8ClampedArray    Int8ClampedArray
@constant     kJSTypedArrayTypeUint16Array          Uint16Array
@constant     kJSTypedArrayTypeUint32Array          Uint32Array
@constant     kJSTypedArrayTypeFloat32Array         Float32Array
@constant     kJSTypedArrayTypeFloat64Array         Float64Array
@constant     kJSTypedArrayTypeArrayBuffer          ArrayBuffer
*/
typedef enum {
	_kJSTypedArrayTypeNone = 0,
	_kJSTypedArrayTypeInt8Array = 1,
	_kJSTypedArrayTypeInt16Array = 2,
	_kJSTypedArrayTypeInt32Array = 3,
	_kJSTypedArrayTypeUint8Array = 4,
	_kJSTypedArrayTypeUint8ClampedArray = 5,
	_kJSTypedArrayTypeUint16Array = 6,
	_kJSTypedArrayTypeUint32Array = 7,
	_kJSTypedArrayTypeFloat32Array = 8,
	_kJSTypedArrayTypeFloat64Array = 9,
	_kJSTypedArrayTypeArrayBuffer = 10
} _JSTypedArrayType;

/*!
@function
@abstract           Setup the JSContext for use of the Typed Array functions.
@param ctx          The execution context to use
*/
void JSContextPrepareTypedArrayAPI(JSContextRef ctx);

/*!
@function
@abstract           Returns a JavaScript value's Typed Array type
@param ctx          The execution context to use.
@param value        The JSObject whose Typed Array type you want to obtain.
@result             A value of type JSTypedArrayType that identifies value's Typed Array type
*/
_JSTypedArrayType JSObjectGetTypedArrayType(JSContextRef ctx, JSObjectRef object);

/*!
@function
@abstract           Creates an empty JavaScript Typed Array with the given number of elements
@param ctx          The execution context to use.
@param arrayType    A value of type JSTypedArrayType identifying the type of array you want to create
@param numElements  The number of elements for the array.
@result             A JSObjectRef that is a Typed Array or NULL if there was an error
*/
JSObjectRef JSObjectMakeTypedArrayEJ(JSContextRef ctx, _JSTypedArrayType arrayType, size_t numElements);

/*!
@function
@abstract           Returns a copy of the Typed Array's data
@param ctx          The execution context to use.
@param value        The JSObject whose Typed Array data you want to obtain.
@result             A copy of the Typed Array's data or NULL if the JSObject is not a Typed Array
*/
bool JSObjectGetTypedArrayData(JSContextRef ctx, JSObjectRef object, char*& data, int& len);
bool JSObjectGetTypedArrayDataNoAlloc(JSContextRef ctx, JSObjectRef object, char* data, int len);
/*!
@function
@abstract           Replaces a Typed Array's data
@param ctx          The execution context to use.
@param value        The JSObject whose Typed Array data you want to replace
*/
void JSObjectSetTypedArrayData(JSContextRef ctx, JSObjectRef object, const uint8_t *data, size_t length);
}
#endif

