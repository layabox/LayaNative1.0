//
//  mkFont.cpp
//  MonkVG-iOS
//
//  Created by Micah Pearlman on 6/29/11.
//  Copyright 2011 Zero Vision. All rights reserved.
//

#include "mkFont.h"
#include "mkContext.h"

namespace MonkVG {	// Internal Implementation
	VGint IFont::getParameteri( const VGint p ) const {
		switch (p) {
			default:
                //m_pCtx->setError(( VG_ILLEGAL_ARGUMENT_ERROR );
				return -1;	//todo: set error
				break;
		}
	}
	
	VGfloat IFont::getParameterf( const VGint p ) const {
		switch (p) {
			default:
                //m_pCtx->setError(( VG_ILLEGAL_ARGUMENT_ERROR );
				return -1;	//todo: set error
				break;
		}
	}
	
	void IFont::getParameterfv( const VGint p, VGfloat *fv ) const {
		switch (p) {
			default:
                //m_pCtx->setError(( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
		
	}
	
	void IFont::setParameter( const VGint p, const VGint v ) {
		switch (p) {
			default:
                //m_pCtx->setError(( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
	}
	
	void IFont::setParameter( const VGint p, const VGfloat v ) {
		switch (p) {
			default:
                //m_pCtx->setError(( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
	}
	
	void IFont::setParameter( const VGint p, const VGfloat* fv, const VGint cnt ) {
		switch (p) {
			default:
                //m_pCtx->setError(( VG_ILLEGAL_ARGUMENT_ERROR );
				break;
		}
	}
	
	
	void IFont::addGlyphImage( VGuint index_, IImage* image_, VGfloat glyphOrigin_[2], VGfloat escapement_[2] ) {
		_glyphs[index_] = std::shared_ptr<IFont::GlyphImage>( new IFont::GlyphImage( index_, image_, glyphOrigin_, escapement_ ) );
	}
	void IFont::addGlyphPath( VGuint index_, IPath* path_, VGfloat glyphOrigin_[2], VGfloat escapement_[2] ) {
		_glyphs[index_] = std::shared_ptr<IFont::GlyphPath>( new IFont::GlyphPath( index_, path_, glyphOrigin_, escapement_ ));
	}
	
	void IFont::drawGlyph( VGuint index, VGfloat adj_x, VGfloat adj_y, VGbitfield paintModes ) {
		std::unordered_map<VGuint, std::shared_ptr<Glyph> >::iterator it =  _glyphs.find( index );
		if ( it != _glyphs.end() ) {
			std::shared_ptr<Glyph> glyph = it->second;
			VGfloat origin[2];
			m_pCtx->getGlyphOrigin( origin );
//			vgTranslate( origin[0], origin[1] );
			glyph->draw( paintModes, adj_x, adj_y );
			origin[0] += glyph->escapement[0];
			m_pCtx->setGlyphOrigin( origin );
		}
	}
	
	void IFont::GlyphImage::draw( VGbitfield paintModes, VGfloat adj_x, VGfloat adj_y ) {
		//image->drawSubRect( glyphOrigin[0], glyphOrigin[1], escapement[0], escapement[1], paintModes );
		//image->draw( );
		VGfloat origin[2];
		m_pCtx->getGlyphOrigin( origin );
		origin[0] += adj_x;
		origin[1] += adj_y;
		
		image->drawAtPoint( origin[0], origin[1], paintModes );
	}


	
}

