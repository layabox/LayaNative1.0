/**
@file			layaContext.h
@brief
@author			guo
@version		1.0
@date			2016_6_6
*/


#ifndef __LAYA_CONTEXT__H__
#define __LAYA_CONTEXT__H__

#include "mkContext.h"
namespace laya {
    class JCVectorGraphDynamicMesh;
}

namespace MonkVG {
    class layaPath;
	class layaContext : public IContext {
	public:
	
        layaContext(OnTrianglesData onTrisData, OnTriangleStripsData onTriStripsData,void* pUserData);
        ~layaContext();

		virtual bool Initialize();
		virtual bool Terminate();
		
		//// factories ////
		virtual IPath* createPath( VGint pathFormat, VGPathDatatype datatype, VGfloat scale, VGfloat bias, VGint segmentCapacityHint, VGint coordCapacityHint, VGbitfield capabilities );
		virtual void destroyPath( IPath* path );	
		virtual void destroyPaint( IPaint* paint );
		virtual IPaint* createPaint();
		virtual IImage* createImage( VGImageFormat format,
									VGint width, VGint height,
									VGbitfield allowedQuality );
		virtual void destroyImage( IImage* image );
		virtual IBatch* createBatch();
		virtual void destroyBatch( IBatch* batch );
		virtual IFont* createFont();
		virtual void destroyFont( IFont* font );


		/// paint overrides
		virtual void setStrokePaint( IPaint* paint );
		virtual void setFillPaint( IPaint* paint );

		//// platform specific execution of stroke and fill ////
		virtual void stroke();
		virtual void fill();
		
		//// platform specific execution of Masking and Clearing ////
		virtual void clear(VGint x, VGint y, VGint width, VGint height);
		
		//// platform specific implementation of transform ////
		virtual void setIdentity();
		virtual void transform( VGfloat* t );
		virtual void scale( VGfloat sx, VGfloat sy );
		virtual void translate( VGfloat x, VGfloat y );
		virtual void rotate( VGfloat angle );
		virtual void setTransform( const VGfloat* t ) ;
		virtual void multiply( const VGfloat* t );
		virtual void setMatrixMode( VGMatrixMode mode ) {
			IContext::setMatrixMode( mode );
			loadGLMatrix();
		}
		void loadGLMatrix();

		void beginRender();
        void render(layaPath* pPath, VGbitfield paintModes);
		void endRender();
		
		
		virtual void resize();
		
		
		static void checkGLError();
		
		/// batch drawing
		virtual void startBatch( IBatch* batch );
        virtual void dumpBatch( IBatch* batch, void **vertices, size_t *size );
		virtual void endBatch( IBatch* batch );
		
		/// image
		virtual void setImageMode( VGImageMode im );
        
        
	private:
        OnTrianglesData         m_pOnTrisData;
        OnTriangleStripsData    m_pOnTriStripsData;
        void*                   m_pUserData;
	};
}

#endif // __qlContext_h__
