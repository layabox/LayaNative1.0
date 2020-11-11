/*
 *  mkPaint.cpp
 *  MonkVG-Quartz
 *
 *  Created by Micah Pearlman on 3/3/09.
 *  Copyright 2009 Monk Games. All rights reserved.
 *
 */

#include "mkPaint.h"
#include "mkContext.h"

namespace MonkVG {	// Internal Implementation
	VGint IPaint::getParameteri( const VGint p ) const {
		switch (p) {
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				return -1;	//todo: set error
				break;
		}
	}
	
	VGfloat IPaint::getParameterf( const VGint p ) const {
		switch (p) {
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				return -1;	//todo: set error
				break;
		}
	}
	
	void IPaint::getParameterfv( const VGint p, VGfloat *fv ) const {
		switch (p) {
			case VG_PAINT_COLOR:
				for( int i = 0; i < 4; i++ )
					fv[i] = _paintColor[i];
				break;
	
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
		
	}

	void IPaint::setParameter( const VGint p, const VGint v ) {
		switch (p) {
			case VG_PAINT_TYPE:
				setPaintType( (VGPaintType)v );
				break;
			case VG_PAINT_COLOR_RAMP_SPREAD_MODE:
				_colorRampSpreadMode = (VGColorRampSpreadMode)v;
				break;
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
	}
	
	void IPaint::setParameter( const VGint p, const VGfloat v ) 
	{
		switch (p) {
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
	}
	
	void IPaint::setParameter( const VGint p, const VGfloat* fv, const VGint cnt ) {
		switch (p) {
			case VG_PAINT_COLOR:
                _paintColor[0] = fv[0];
                _paintColor[1] = fv[1];
                _paintColor[2] = fv[2];
                _paintColor[3] = fv[3];
				break;

			case VG_PAINT_LINEAR_GRADIENT:
                _paintLinearGradient[0] = fv[0];
                _paintLinearGradient[1] = fv[1];
                _paintLinearGradient[2] = fv[2];
                _paintLinearGradient[3] = fv[3];
				break;
			case VG_PAINT_RADIAL_GRADIENT:
                _paintRadialGradient[0] = fv[0];
                _paintRadialGradient[1] = fv[1];
                _paintRadialGradient[2] = fv[2];
                _paintRadialGradient[3] = fv[3];
                _paintRadialGradient[4] = fv[4];
				break;
			case VG_PAINT_COLOR_RAMP_STOPS:
				for ( int j = 0; j < cnt/5; j++ ) {
					Stop_t stop;
					for ( int p = 0; p < 5; p++ ) {
						stop.a[p] = fv[(j * 5) + p];
					}
					_colorRampStops.push_back( stop );
				}
				break;
			case VG_PAINT_2x3_GRADIENT:
                _paint2x3Gradient[0] = fv[0];
                _paint2x3Gradient[1] = fv[1];
                _paint2x3Gradient[2] = fv[2];
                _paint2x3Gradient[3] = fv[3];
                _paint2x3Gradient[4] = fv[4];
                _paint2x3Gradient[5] = fv[5];
				break;
			
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
	}
	

	
}

