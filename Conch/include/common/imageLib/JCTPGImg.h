/**
@file			JCTPGImg.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#ifndef __JCTPGImg_H__
#define __JCTPGImg_H__
#ifdef TPG
#include <memory>
#include <functional>
#include "../3DRS/JCGL.h"
namespace laya
{
    class BitmapData;
    bool isTpg(const char* pData, int nLength);
    bool LoadTPGFromMem(BitmapData* pBitmapData, unsigned  char * data, int nLength);
}
#endif
//------------------------------------------------------------------------------
#endif //__JCTPGImg_H__
//-----------------------------END FILE--------------------------------
