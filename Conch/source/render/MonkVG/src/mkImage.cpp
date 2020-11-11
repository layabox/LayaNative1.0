//
//  mkImage.cpp
//  MonkVG-iOS
//
//  Created by Micah Pearlman on 6/28/11.
//  Copyright 2011 Zero Vision. All rights reserved.
//

#include "mkImage.h"
#include "mkContext.h"

namespace MonkVG {	// Internal Implementation
	IImage::IImage( VGImageFormat format,
		   VGint width, VGint height,
		   VGbitfield allowedQuality )
	:	BaseObject()
	,	_format( format )
	,	_width( width )
	,	_height( height )
	,	_allowedQuality( allowedQuality )
	,	_parent( 0 )
	{
		_s[0] = _t[0] = 0;
		_s[1] = _t[1] = 1;
	}
	
	IImage::IImage( IImage& other ) 
	:	BaseObject()
	,	_format( other._format )
	,	_width( other._width )
	,	_height( other._height )
	,	_allowedQuality( other._allowedQuality )
	,	_parent( &other )
	{
		_s[0] = other._s[0]; 
		_s[1] = other._s[1]; 
		_t[0] = other._t[0]; 
		_t[1] = other._t[1]; 

	}

	VGint IImage::getParameteri( const VGint p ) const {
		switch (p) {
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				return -1;	//todo: set error
				break;
		}
	}
	
	VGfloat IImage::getParameterf( const VGint p ) const {
		switch (p) {
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				return -1;	//todo: set error
				break;
		}
	}
	
	void IImage::getParameterfv( const VGint p, VGfloat *fv ) const {
		switch (p) {
				
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
		
	}
	
	void IImage::setParameter( const VGint p, const VGint v ) {
		switch (p) {
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
	}
	
	void IImage::setParameter( const VGint p, const VGfloat v ) 
	{
		switch (p) {
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
	}
	
	void IImage::setParameter( const VGint p, const VGfloat* fv, const VGint cnt ) {
		switch (p) {
				
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
	}
	
	
	
}


