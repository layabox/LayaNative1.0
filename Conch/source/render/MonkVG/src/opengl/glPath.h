/*
 *  glPath.h
 *  MonkVG-OpenGL
 *
 *  Created by Micah Pearlman on 8/6/10.
 *  Copyright 2010 MP Engineering. All rights reserved.
 *
 */

#ifndef __glPath_h__
#define __glPath_h__

#include "mkPath.h"
#include "glPlatform.h"
#include <list>
#include <vector>
#include "glPaint.h"
#include <algorithm>

using std::min;
using std::max;
namespace OpenGLES {
    class OpenGLESContext;
};
namespace MonkVG {
    class IContext;
	class OpenGLPath : public IPath {
	public:
	
		OpenGLPath( VGint pathFormat, VGPathDatatype datatype, VGfloat scale, VGfloat bias, VGint segmentCapacityHint, VGint coordCapacityHint, VGbitfield capabilities ) 
			:	IPath( pathFormat, datatype, scale, bias, segmentCapacityHint, coordCapacityHint, capabilities )
			,	_fillTesseleator( 0 )
			,	_strokeVBO(-1)
			,	_fillVBO(-1)
			,	_fillPaintForPath( 0 )
			,	_strokePaintForPath( 0 )
		{

		}
		virtual ~OpenGLPath();
		
		
		virtual bool draw( VGbitfield paintModes );
		virtual void clear( VGbitfield caps );
		virtual void buildFillIfDirty();

	private:
		struct v2_t {
			GLfloat x, y;
		};
		
		struct v3_t {
			GLdouble x,y,z;
			v3_t() {}
	       		v3_t( GLdouble * v) : x(v[0]), y(v[1]), z(v[2]) {}
			v3_t(GLdouble ix, GLdouble iy, GLdouble iz) : x(ix), y(iy), z(iz) {}
			void print() const {
				printf("(%f, %f)\n", x, y);
			}
			
		};
		
		struct textured_vertex_t {
			GLfloat		v[2];
			GLfloat		uv[2];
		};
		
	private:
		
		GLUtesselator*		_fillTesseleator;
		vector<GLfloat>		_vertices;          //fill的时候需要的顶点信息。0,1=x,y, 2,3=x,y, ...
		vector<v2_t>		_strokeVertices;    //stroke的时候需要的顶点信息
		list<v3_t>			_tessVertices;      //用来tess的顶点，这个已经是三维的了，只是实际z都是0
		GLenum				_primType;
		GLuint				_fillVBO;
		GLuint				_strokeVBO;
		int					_numberFillVertices;
		int					_numberStrokeVertices;
		OpenGLPaint*		_fillPaintForPath;
		OpenGLPaint*		_strokePaintForPath;
		
		
	private:		// tesseleator callbacks
		static void tessBegin( GLenum type, GLvoid* user );
		static void tessEnd( GLvoid* user );
		static void tessVertex( GLvoid *vertex, GLvoid* user );
		static void tessCombine( GLdouble coords[3], void *data[4],
								GLfloat weight[4], void **outData,
								void *polygonData );
		static void tessError( GLenum errorCode );
		void endOfTesselation( VGbitfield paintModes );
    public:
        IContext*   m_pCtx = nullptr;
        OpenGLES::OpenGLESContext*  gl = nullptr;
    private:	// utility methods
		
		GLenum primType() {
			return _primType;
		}
		void setPrimType( GLenum t ) {
			_primType = t;
		}
		
		GLdouble* tessVerticesBackPtr() {
			return &(_tessVertices.back().x);
		} 
		
		void updateBounds(float x, float y) {
			_minX = min(_minX, x);
			_width = max(_width, x);
			_minY = min(_minY, y);
			_height = max(_height, y);
		}

		void addVertex( GLdouble* v ) {
			VGfloat x = (VGfloat)v[0];
			VGfloat y = (VGfloat)v[1];
			updateBounds(x, y);
			_vertices.push_back(x);
			_vertices.push_back(y);
		}

		GLdouble * addTessVertex( const v3_t & v ) {
			//updateBounds(v.x, v.y);
			_tessVertices.push_back( v );
			return tessVerticesBackPtr();
		}
		
		void buildFill();
		void buildStroke();
        /** 
         *  @brief 创建一段线段。
         *  @param[out] vertices 这个描述了当前的结果，这个函数会继续往里push顶点作为输出。会push 4个顶点。
         *  @param[out] 
         *  @return 
         */
		void buildFatLineSegment( vector<v2_t>& vertices, const v2_t& p0, const v2_t& p1, const float stroke_width );

	};
}

#endif // __glPath_h__
