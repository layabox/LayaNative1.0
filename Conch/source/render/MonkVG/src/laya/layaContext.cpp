
#include "layaContext.h"
#if __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
#include "layaPaint.h"
#include "layaPath.h"

using namespace laya;

namespace MonkVG {

    layaContext::layaContext(OnTrianglesData onTrisData, OnTriangleStripsData onTriStripsData, void* pUserData) {
        m_pOnTrisData = onTrisData;
        m_pOnTriStripsData = onTriStripsData;
        m_pUserData = pUserData;
    }

    layaContext::~layaContext() {
        if (_fill_paint){
            destroyPaint(_fill_paint);
            _fill_paint = nullptr;
        }
        if (_stroke_paint){
            destroyPaint(_stroke_paint);
            _stroke_paint = nullptr;
        }
    }

    void layaContext::checkGLError() {
        int err = glGetError();
        const char* RVAL = "GL_UNKNOWN_ERROR";
        switch (err){
        case GL_NO_ERROR:
            RVAL = "GL_NO_ERROR";
            break;
        case GL_INVALID_ENUM:
            RVAL = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            RVAL = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            RVAL = "GL_INVALID_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            RVAL = "GL_OUT_OF_MEMORY";
            break;
        default:
            break;
        }

        if (err != GL_NO_ERROR) {
            printf("GL_ERROR: %s\n", RVAL);
            MK_ASSERT(0);
        }
    }

    bool layaContext::Initialize() {

        return true;
    }

    void layaContext::resize() {
    }


    bool layaContext::Terminate() {
        return true;
    }


    void layaContext::beginRender() {
    }
    void layaContext::endRender() {
    }

    void layaContext::render(layaPath* pPath, VGbitfield paintModes) {
        void* pData = nullptr;
        //fill
        if (paintModes & VG_FILL_PATH) {
            int fillDataLen = 0;
            pData = pPath->getFillData(fillDataLen);
            if (m_pOnTrisData) {
                m_pOnTrisData(pData, fillDataLen, m_pUserData);
            }
        }
        //stroke
        if (paintModes & VG_STROKE_PATH) {
            int strokeDataLen = 0;
            pData = pPath->getStrokeData(strokeDataLen);
            if (m_pOnTriStripsData) {
                m_pOnTriStripsData(pData, strokeDataLen, m_pUserData);
            }
        }
    }

    /// factories

    IPath* layaContext::createPath(VGint pathFormat, VGPathDatatype datatype, VGfloat scale, VGfloat bias, VGint segmentCapacityHint, VGint coordCapacityHint, VGbitfield capabilities) {
        layaPath* path = new layaPath(pathFormat, datatype, scale, bias, segmentCapacityHint, coordCapacityHint, capabilities &= VG_PATH_CAPABILITY_ALL);
        path->m_pCtx = this;
        return (IPath*)path;
    }

    void layaContext::destroyPath(IPath* path) {
            delete (layaPath*)path;
    }

    void layaContext::destroyPaint(IPaint* paint) {
        delete (layaPaint*)paint;
    }

    IPaint* layaContext::createPaint() {
        layaPaint* paint = new layaPaint();
        return (IPaint*)paint;
    }

    IBatch* layaContext::createBatch() {
        //OpenGLBatch *batch = new OpenGLBatch();
        //if (batch == 0)
        //    SetError(VG_OUT_OF_MEMORY_ERROR);
        //return (IBatch*)batch;
        return nullptr;
    }

    void layaContext::destroyBatch(IBatch* batch) {
        //if (batch) {
        //    delete batch;
        //}
    }

    IImage* layaContext::createImage(VGImageFormat format,
        VGint width, VGint height,
        VGbitfield allowedQuality) {
        //return new OpenGLImage(format, width, height, allowedQuality);
        return nullptr;
    }
    void layaContext::destroyImage(IImage* image) {
        if (image) {
            delete image;
        }
    }

    IFont* layaContext::createFont() {
        //return new OpenGLFont();
        return nullptr;
    }
    void layaContext::destroyFont(IFont* font) {
        if (font) {
            delete font;
        }
    }



    /// state 
    void layaContext::setStrokePaint(IPaint* paint) {
        if (paint != _stroke_paint) {
            IContext::setStrokePaint(paint);
            layaPaint* glPaint = (layaPaint*)_stroke_paint;
            //glPaint->setGLState();
            if (glPaint)
                glPaint->setIsDirty(true);
        }
    }

    void layaContext::setFillPaint(IPaint* paint) {
        if (paint != _fill_paint) {
            IContext::setFillPaint(paint);
            layaPaint* glPaint = (layaPaint*)_fill_paint;
            //glPaint->setGLState();
            if (glPaint)
                glPaint->setIsDirty(true);
        }

    }


    void layaContext::stroke() {
        if (_stroke_paint) {
            layaPaint* glPaint = (layaPaint*)_stroke_paint;
            //glPaint->setGLState();
            glPaint->setIsDirty(false);
            // set the fill paint to dirty
            if (_fill_paint) {
                glPaint = (layaPaint*)_fill_paint;
                glPaint->setIsDirty(true);
            }
        }
    }

    void layaContext::fill() {

        if (_fill_paint && _fill_paint->getPaintType() == VG_PAINT_TYPE_COLOR) {
            layaPaint* glPaint = (layaPaint*)_fill_paint;
            //glPaint->setGLState();
            //TODO 取出paint的颜色。设置到shader中。
            glPaint->setIsDirty(false);
            // set the stroke paint to dirty
            if (_stroke_paint) {
                glPaint = (layaPaint*)_stroke_paint;
                glPaint->setIsDirty(true);
            }
        }
    }

    void layaContext::startBatch(IBatch* batch) {
        //assert(_currentBatch == 0);	// can't have multiple batches going on at once
        //_currentBatch = batch;
    }
    void layaContext::dumpBatch(IBatch *batch, void **vertices, size_t *size) {
        //_currentBatch->dump(vertices, size);
    }
    void layaContext::endBatch(IBatch* batch) {
        //_currentBatch->finalize();
        //_currentBatch = 0;
    }


    void layaContext::clear(VGint x, VGint y, VGint width, VGint height) {
        // TODO:
    }

    void layaContext::loadGLMatrix() {
        Matrix33& active = *getActiveMatrix();
        //TODO 真正渲染的时候要用这个矩阵。
        GLfloat mat44[4][4];
        //for( int x = 0; x < 4; x++ )
        //	for( int y = 0; y < 4; y++ )
        //		mat44[x][y] = 0;
        //mat44[0][0] = 1.0f;
        //mat44[1][1] = 1.0f;
        //mat44[2][2] = 1.0f;
        //mat44[3][3]	= 1.0f;


        //		a, c, e,			// cos(a) -sin(a) tx
        //		b, d, f,			// sin(a) cos(a)  ty
        //		ff0, ff1, ff2;		// 0      0       1

        mat44[0][0] = active.a;	mat44[0][1] = active.b; mat44[0][2] = 0;    mat44[0][3] = 0;
        mat44[1][0] = active.c;	mat44[1][1] = active.d; mat44[1][2] = 0;    mat44[1][3] = 0;
        mat44[2][0] = 0;        mat44[2][1] = 0;        mat44[2][2] = 1.0f; mat44[2][3] = 0;
        mat44[3][0] = active.e;	mat44[3][1] = active.f; mat44[3][2] = 0.0f; mat44[3][3] = 1.0f;
        //gl()->glLoadMatrixf(&mat44[0][0]);
    }



    void layaContext::setIdentity() {
        Matrix33* active = getActiveMatrix();
        active->setIdentity();
        loadGLMatrix();
    }

    void layaContext::transform(VGfloat* t) {
        // a	b	0
        // c	d	0
        // tx	ty	1
        Matrix33* active = getActiveMatrix();
        for (int i = 0; i < 9; i++)
            t[i] = active->m[i];

    }

    void layaContext::setTransform(const VGfloat* t) {
        //对于layaContext这个其实可以不处理。因为点本身都已经先转换了，就剩下的颠倒在shader中做了
        return;
        //	OpenVG:
        //	sh	shx	tx
        //	shy	sy	ty
        //	0	0	1
        //
        // OpenGL
        // a	b	0
        // c	d	0
        // tx	ty	1

        Matrix33* active = getActiveMatrix();
        for (int i = 0; i < 9; i++)
            active->m[i] = t[i];
        loadGLMatrix();
    }


    void layaContext::multiply(const VGfloat* t) {
        Matrix33 m;
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                m.set(y, x, t[(y * 3) + x]);
            }
        }
        Matrix33* active = getActiveMatrix();
        active->postMultiply(m);
        loadGLMatrix();
    }

    void layaContext::scale(VGfloat sx, VGfloat sy) {
        Matrix33* active = getActiveMatrix();
        Matrix33 scale;
        scale.setIdentity();
        scale.setScale(sx, sy);
        Matrix33 tmp;
        Matrix33::multiply(tmp, scale, *active);
        active->copy(tmp);
        loadGLMatrix();
    }
    void layaContext::translate(VGfloat x, VGfloat y) {

        Matrix33* active = getActiveMatrix();
        Matrix33 translate;
        translate.setTranslate(x, y);
        Matrix33 tmp;
        tmp.setIdentity();
        Matrix33::multiply(tmp, translate, *active);
        active->copy(tmp);
        loadGLMatrix();
    }
    void layaContext::rotate(VGfloat angle) {
        Matrix33* active = getActiveMatrix();
        Matrix33 rotate;
        rotate.setRotation(radians(angle));
        Matrix33 tmp;
        tmp.setIdentity();
        Matrix33::multiply(tmp, rotate, *active);
        active->copy(tmp);
        loadGLMatrix();
    }

    void layaContext::setImageMode(VGImageMode im) {
        //IContext::setImageMode(im);
        //switch (im) {
        //case VG_DRAW_IMAGE_NORMAL:
        //    gl()->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        //    break;
        //case VG_DRAW_IMAGE_MULTIPLY:
        //    gl()->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        //    break;
        //case VG_DRAW_IMAGE_STENCIL:
        //    break;
        //default:
        //    break;
        //}
    }

}