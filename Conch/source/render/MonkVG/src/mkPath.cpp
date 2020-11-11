/*
 *  mkPath.cpp
 *  MonkVG-XCode
 *
 *  Created by Micah Pearlman on 2/22/09.
 *  Copyright 2009 Monk Games. All rights reserved.
 *
 */

#include "mkPath.h"
#include "mkContext.h"
#include <cassert>
#include <algorithm>

using std::min;
using std::max;

namespace MonkVG {	// Internal Implementation
	int32_t IPath::segmentToNumCoordinates(VGPathSegment segment)
	{
		static const int32_t coords[13] = {0,2,2,1,1,4,6,2,4,5,5,5,5};
		return coords[(int32_t)segment >> 1];
	}
	
	void IPath::appendData( const VGint numSegments, const VGubyte * pathSegments, const void * pathData ) 
	{
        if (_datatype != VG_PATH_DATATYPE_F) {
            assert(!"unsupported path data type");
            return;
        }

		int numCoords = 0;
		for( int i = 0; i < numSegments; i++ ) {
			_segments.push_back(pathSegments[i] );
            if (pathSegments[i]==VG_LINE_DATA) {
                float* pfDt = (float*)pathData;
                int nVertNum = *(int*)(pfDt + numCoords);
                numCoords += 1; //长度本身。
                numCoords += nVertNum*2;
            }
            else {
                numCoords += segmentToNumCoordinates(static_cast<VGPathSegment>(pathSegments[i]));
            }
		}
		
		_numSegments += numSegments;
		_numCoords += numCoords;
		
		for( int i = 0; i < numCoords; i++ ) {
    		_fcoords->push_back( *(((VGfloat*)(pathData)) + i) );
		}
		
		setIsDirty( true );
	}
	
	void IPath::copy( const IPath& src, const Matrix33& transform ) {
		// TODO: transform!!!
		// BUGBUG
		setNumCoords( src.getNumCoords() );
		setNumSegments( src.getNumSegments() );
		_segments = src._segments;
		*_fcoords = *src._fcoords;
	}
	
	void IPath::clear( VGbitfield caps ) {
	
		_segments.clear();
		_numSegments = 0;
		_numCoords = 0;

		switch (_datatype) {
			case VG_PATH_DATATYPE_F:
				_fcoords->clear();
				break;
			default:
				// error
				assert( !"unsupported path data type" );
				break;
		}
		
	}
	
	VGint IPath::getParameteri( const VGint p ) const {
		switch (p) {
			case VG_PATH_FORMAT:
				return getFormat();
			case VG_PATH_DATATYPE:
				return getDataType();
			case VG_PATH_NUM_SEGMENTS:
				return getNumSegments();
			case VG_PATH_NUM_COORDS:
				return getNumCoords();
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				return -1;	
				break;
		}
	}
	
	VGfloat IPath::getParameterf( const VGint p ) const {
		switch (p) {
			case VG_PATH_SCALE:
				return getScale();
				break;
			case VG_PATH_BIAS:
				return getBias();
				break;
			default:
				//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
				return -1;	
				break;
		}
	}
	
	void IPath::getParameterfv( const VGint p, VGfloat *fv ) const {
		//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
	}

	
	void IPath::setParameter( const VGint p, const VGint v ) {
		switch (p) {
			case VG_PATH_FORMAT:
				setFormat( v );
				break;
			case VG_PATH_DATATYPE:
				setDataType( static_cast<VGPathDatatype>( v ) );
				break;
			case VG_PATH_NUM_SEGMENTS:
				setNumSegments( v );
				break;
			case VG_PATH_NUM_COORDS:
				setNumCoords( v );
				break;
			default:
				break;
		}
	}
	
	void IPath::setParameter( const VGint p, const VGfloat v ) 
	{
		switch (p) {
			case VG_PATH_SCALE:
				setScale( v );
				break;
			case VG_PATH_BIAS:
				setBias( v );
				break;
			default:
				break;
		}
	}
	
	void IPath::setParameter( const VGint p, const VGfloat* fv, const VGint cnt ) {
		//SetError( VG_ILLEGAL_ARGUMENT_ERROR );
	}
	
	
}




