/**
 @file			JSLog.h
 @brief
 @author		wyw
 @version		1.0
 @date			2013_11_12
 */

#ifndef __JCMarcos_H__
#define __JCMarcos_H__
#include <stdio.h>
#include <stdint.h>

#define LAYA_HOST_IS_BIG_ENDIAN (bool)(*(unsigned short *)"\0\xff" < 0x100) 

#define LAYA_SWAP32(i)  ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#define LAYA_SWAP16(i)  ((i & 0x00ff) << 8 | (i &0xff00) >> 8)   
#define LAYA_SWAP_INT32_LITTLE_TO_HOST(i) ((LAYA_HOST_IS_BIG_ENDIAN == true)? LAYA_SWAP32(i) : (i) )
#define LAYA_SWAP_INT16_LITTLE_TO_HOST(i) ((LAYA_HOST_IS_BIG_ENDIAN == true)? LAYA_SWAP16(i) : (i) )
#define LAYA_SWAP_INT32_BIG_TO_HOST(i)    ((LAYA_HOST_IS_BIG_ENDIAN == true)? (i) : LAYA_SWAP32(i) )
#define LAYA_SWAP_INT16_BIG_TO_HOST(i)    ((LAYA_HOST_IS_BIG_ENDIAN == true)? (i):  LAYA_SWAP16(i) )

#endif //__JCMarcos_H__


//-----------------------------END FILE--------------------------------
