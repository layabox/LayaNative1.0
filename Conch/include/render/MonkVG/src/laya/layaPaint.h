/**
@file			layaPaint.h
@brief
@author			guo
@version		1.0
@date			2016_6_6
*/

#ifndef __Laya_Paint_h__
#define __Laya_Paint_h__

#include "mkPaint.h"

/**
 * Paint 会提供一个颜色，或者一个位图作为笔刷。
*/
namespace MonkVG {
	
	
	class layaPaint : public IPaint {
	public:
        layaPaint();
		virtual ~layaPaint();
		void setGLState();
		void buildLinearGradientImage( VGfloat pathWidth, VGfloat pathHeight );
		void buildRadialGradientImage( VGfloat pathWidth, VGfloat pathHeight );
		void buildLinear2x3GradientImage( VGfloat pathWidth, VGfloat pathHeight );
		void buildRadial2x3GradientImage( VGfloat pathWidth, VGfloat pathHeight );
		void buildGradientImage( VGfloat pathWidth, VGfloat pathHeight );
		virtual bool isDirty() { return _isDirty; }
		virtual void setIsDirty( bool b ) { _isDirty = b; }
		
		//OpenGLImage* getGradientImage() { return (OpenGLImage*)_gradientImage; }
		
	private:
		bool		_isDirty;
		VGImage		_gradientImage;
		
		void calcStops(float ** stop0, float ** stop1, float g);
		void lerpColor(float * dst, float * stop0, float * stop1, float g);
		
	};
}
#endif // __qzPaint_h__
