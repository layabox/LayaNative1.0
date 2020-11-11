
/**
@file			JCConchBridge.h
@brief
@author			James
@version		1.0
@date			2016_5_27
*/

#ifndef __JCConchBridge_H__
#define __JCConchBridge_H__

namespace laya
{
    /**
    * @brief
    */
    class JCConchBridge
    {
    public:
        static void getPixelsRenderToJS(unsigned char * pPixels, int nSize, int nID, int w, int h);
        static void toBaseRenderToJS(const char* base64, int nID);
    };
}
//------------------------------------------------------------------------------


#endif //__JCConchBridge_H__

 //-----------------------------END FILE--------------------------------