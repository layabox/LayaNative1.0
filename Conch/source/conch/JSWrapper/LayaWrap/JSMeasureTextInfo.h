/**
@file			JSMeasureTextInfo.h
@brief			
@author			wyw
@version		1.0
@date			2013_11_12
*/

#ifndef __JSMeasureTextInfo_H__
#define __JSMeasureTextInfo_H__

//包含头文件
#include <stdio.h>
#include <string>
#include <map>
#include "../JSInterface/JSInterface.h"
#include "../JSInterface/jsobjbase.h"
#include <resource/text/JCFontInfo.h>

namespace laya
{
    class JSMeasureTextInfo:public JsObjBase, public JSObjNode
    {
    public:
	    static JsObjClassInfo JSCLSINFO;
	    static void exportJS();

	    JSMeasureTextInfo();

	    ~JSMeasureTextInfo();

	    void measureText( JCFontInfo* pFontInfo,std::string p_sBuffer );

	    int GetWidth();
	    int GetHeight();

    public:

	    int					m_nWidth;
	    int					m_nHeight;

    };
}

#endif //__JSRadialGradient_H__

//-----------------------------END FILE--------------------------------