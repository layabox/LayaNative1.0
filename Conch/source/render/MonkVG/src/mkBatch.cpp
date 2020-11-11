//
//  mkBatch.cpp
//  MonkVG-iOS
//
//  Created by Micah Pearlman on 6/27/11.
//  Copyright 2011 Zero Vision. All rights reserved.
//

#include "mkBatch.h"
#include "mkContext.h"

namespace MonkVG {	// Internal Implementation
	VGint IBatch::getParameteri( const VGint p ) const {
		switch (p) {
			default:
                m_pCtx->setError( VG_ILLEGAL_ARGUMENT_ERROR );
				return -1;	//todo: set error
				break;
		}
	}
	
	VGfloat IBatch::getParameterf( const VGint p ) const {
		switch (p) {
			default:
                m_pCtx->setError( VG_ILLEGAL_ARGUMENT_ERROR );
				return -1;	//todo: set error
				break;
		}
	}
	
	void IBatch::getParameterfv( const VGint p, VGfloat *fv ) const {
		switch (p) {
				
			default:
                m_pCtx->setError( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
		
	}
	
	void IBatch::setParameter( const VGint p, const VGint v ) {
		switch (p) {
			default:
                m_pCtx->setError( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
	}
	
	void IBatch::setParameter( const VGint p, const VGfloat v ) 
	{
		switch (p) {
			default:
                m_pCtx->setError( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
	}
	
	void IBatch::setParameter( const VGint p, const VGfloat* fv, const VGint cnt ) {
		switch (p) {
				
			default:
                m_pCtx->setError( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
	}
	
	
	
}