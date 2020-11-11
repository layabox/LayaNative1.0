/**
@file			JSTextMemoryCanvas.h
@brief			
@author			James
@version		1.0
@date			2016_5_17
*/

#ifndef __JSTextMemoryCanvas_H__
#define __JSTextMemoryCanvas_H__

#include <vector>
#include "../JSInterface/JSInterface.h"
#include "../JSInterface/jsobjbase.h"
#include <Text/JCTextMemoryCanvas.h>

namespace laya
{
    class JSTextMemoryCanvas :public JsObjBase
	{
    public:

		static JsObjClassInfo JSCLSINFO;

		void exportJS();	//全局的对象，需要生成一个实例，所以不用静态函数

        JSTextMemoryCanvas();

		~JSTextMemoryCanvas();

	public:
		const char* getFillStyle();
		void setFillStyle( const char* sColor );
		void setFontInfo(const char* sFontInfo);
		const char* getFontInfo();
		void setTextBaseline(const char* baseline);
		const char *getTextBaseline();
		bool isTextCanvas();
		JsValue measureText(const char* sText);
		void fillText(const char* sText, int x, int y);
		void setTargetTextureID(int nTextureID);
        void setSize( int w,int h );
        void setWidth( int w);
        int getWidth();
        void setHeight( int h );
        int getHeight();
        int getImageID();
    public:
		JCTextMemoryCanvas*	m_pTextMemoryCanvas;
        int                 m_nWidth;
        int                 m_nHeight;
	};
}
//------------------------------------------------------------------------------


#endif //__JSTextMemoryCanvas_H__

//-----------------------------END FILE--------------------------------