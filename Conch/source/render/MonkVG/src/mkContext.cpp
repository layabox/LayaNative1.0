/*
 *  mkContext.cpp
 *  MonkVG-XCode
 *
 *  Created by Micah Pearlman on 2/22/09.
 *  Copyright 2009 Monk Games. All rights reserved.
 *
 */
#include "mkContext.h"
#include "glPath.h"
#include "glContext.h"
#include "opengl/glContext.h"
#include "laya/layaContext.h"

using namespace MonkVG;

void* vgCreateContextMNK(VGint width, VGint height, VGRenderingBackendTypeMNK backend,
    OnTrianglesData onTrisData,
    OnTriangleStripsData onTriStripsData, void* pUserData)
{
    IContext* pCtx = nullptr;
    MK_LOG("Creating context %d, %d, %d", width, height, (int)backend);
    switch (backend) {
    case VG_RENDERING_BACKEND_TYPE_LAYA:
        pCtx = new MonkVG::layaContext(onTrisData, onTriStripsData, pUserData);
        break;
    case VG_RENDERING_BACKEND_TYPE_OPENGLES20:
        pCtx = new MonkVG::OpenGLContext();
        break;
    default:
        assert(0);
    }
    pCtx->setRenderingBackendType(backend);

    pCtx->setWidth(width);
    pCtx->setHeight(height);
    pCtx->Initialize();

    return pCtx;
}

namespace MonkVG {

    IContext::IContext()
        : _error(VG_NO_ERROR)
        , _width(0)
        , _height(0)
        , _stroke_line_width(2.0f)
        , _stroke_paint(0)
        , _fill_paint(0)
        , _active_matrix(&_path_user_to_surface)
        , _fill_rule(VG_EVEN_ODD)
        , _renderingQuality(VG_RENDERING_QUALITY_BETTER)
        , _tessellationIterations(16)
        , _matrixMode(VG_MATRIX_PATH_USER_TO_SURFACE)
        , _currentBatch(0)
        , _imageMode(VG_DRAW_IMAGE_NORMAL)
    {
        _path_user_to_surface.setIdentity();
        _glyph_user_to_surface.setIdentity();
        _image_user_to_surface.setIdentity();
        _active_matrix->setIdentity();
        _glyph_origin[0] = _glyph_origin[1] = 0;

        setImageMode(_imageMode);
    }

    //// parameters ////
    void IContext::set(VGuint type, VGfloat f) {
        switch (type) {
        case VG_STROKE_LINE_WIDTH:
            setStrokeLineWidth(f);
            break;
        default:
            setError(VG_ILLEGAL_ARGUMENT_ERROR);
            break;
        }
    }

    void IContext::set(VGuint type, const VGfloat * fv) {
        switch (type) {
        case VG_CLEAR_COLOR:
            setClearColor(fv);
            break;
        case VG_GLYPH_ORIGIN:
            setGlyphOrigin(fv);
            break;

        default:
            setError(VG_ILLEGAL_ARGUMENT_ERROR);
            break;
        }
    }

    void IContext::set(VGuint type, VGint i) {

        switch (type) {
        case VG_MATRIX_MODE:
            setMatrixMode((VGMatrixMode)i);
            break;
        case VG_FILL_RULE:
            setFillRule((VGFillRule)i);
            break;
        case VG_TESSELLATION_ITERATIONS_MNK:
            setTessellationIterations(i);
            break;
        case VG_IMAGE_MODE:
            setImageMode((VGImageMode)i);
            break;
        default:
            break;
        }

    }
    void IContext::get(VGuint type, VGfloat &f) const {
        switch (type) {
        case VG_STROKE_LINE_WIDTH:
            f = getStrokeLineWidth();
            break;
        default:
            //setError(VG_ILLEGAL_ARGUMENT_ERROR);
            break;
        }

    }

    void IContext::get(VGuint type, VGfloat *fv) const {
        switch (type) {
        case VG_CLEAR_COLOR:
            getClearColor(fv);
            break;
        case VG_GLYPH_ORIGIN:
            getGlyphOrigin(fv);
            break;

        default:
            //setError(VG_ILLEGAL_ARGUMENT_ERROR);
            break;
        }

    }
    void IContext::get(VGuint type, VGint& i) const {
        i = -1;

        switch (type) {
        case VG_MATRIX_MODE:
            i = getMatrixMode();
            break;
        case VG_FILL_RULE:
            i = getFillRule();
            break;
        case VG_TESSELLATION_ITERATIONS_MNK:
            i = getTessellationIterations();
            break;
        case VG_IMAGE_MODE:
            i = getImageMode();
            break;
        case VG_SURFACE_WIDTH_MNK:
            i = getWidth();
            break;
        case VG_SURFACE_HEIGHT_MNK:
            i = getHeight();
            break;

        default:
            break;
        }


    }
}

namespace laya {
    MkVGContext::MkVGContext(VGint width, VGint height, VGRenderingBackendTypeMNK backend,
        OnTrianglesData onTrisData,
        OnTriangleStripsData onTriStripsData, void* pUserData) {
        m_pCtx = (IContext*)vgCreateContextMNK(width, height, backend, onTrisData, onTriStripsData, pUserData);
    }

    MkVGContext::~MkVGContext() {
        m_pCtx->Terminate();
        if (m_pCtx){
            delete m_pCtx;
            m_pCtx = nullptr;
        }
    }

    void MkVGContext::ResizeSurfaceMNK(VGint width, VGint height)
    {
        m_pCtx->setWidth(width);
        m_pCtx->setHeight(height);
        m_pCtx->resize();

    }

    void  MkVGContext::Setf(VGuint type, VGfloat value)  {
        m_pCtx->set(type, value);
    }

    void  MkVGContext::Seti(VGuint type, VGint value)  {
        m_pCtx->set(type, value);
    }

    void  MkVGContext::Setfv(VGuint type, VGint count,
        const VGfloat * values)  {
        m_pCtx->set(type, values);
    }
    void  MkVGContext::Setiv(VGuint type, VGint count,
        const VGint * values)  {
    }

    VGfloat  MkVGContext::Getf(VGuint type)  {
        VGfloat ret = -1;
        m_pCtx->get(type, ret);
        return ret;
    }

    VGint  MkVGContext::Geti(VGuint type)  {
        VGint ret = -1;
        m_pCtx->get(type, ret);
        return ret;
    }

    VGint  MkVGContext::GetVectorSize(VGuint type)  {
        return -1;
    }

    void  MkVGContext::Getfv(VGuint type, VGint count, VGfloat * values)  {

    }

    void  MkVGContext::Getiv(VGuint type, VGint count, VGint * values)  {

    }

    /* Masking and Clearing */
    void  MkVGContext::Clear(VGint x, VGint y, VGint width, VGint height)  {
        m_pCtx->clear(x, y, width, height);
    }

    void  MkVGContext::Mask(VGHandle mask, VGMaskOperation operation, VGint x, VGint y,
        VGint width, VGint height)  {

    }


    /* Finish and Flush */
    void  MkVGContext::Finish(void)  {
        //glFinish();
    }
    void  MkVGContext::Flush(void)  {
        //glFlush();
    }

    /*--------------------------------------------------
    * Returns the oldest error pending on the current
    * context and clears its error code
    *--------------------------------------------------*/

    VGErrorCode MkVGContext::GetError(void)
    {
        return m_pCtx->getError();
    }

    void MkVGContext::SetError(const VGErrorCode e) {
        m_pCtx->setError(e);
    }

    VGPaint MkVGContext::CreatePaint(void) {
        return (VGPaint)m_pCtx->createPaint();
    }

    void MkVGContext::DestroyPaint(VGPaint paint) {
        m_pCtx->destroyPaint((IPaint*)paint);
    }

    void MkVGContext::SetPaint(VGPaint paint, VGbitfield paintModes) {
        if (paint != VG_INVALID_HANDLE && ((IPaint*)paint)->getType() != BaseObject::kPaintType) {
            m_pCtx->setError(VG_BAD_HANDLE_ERROR);
            return;
        }
        if (!paintModes || paintModes & ~(VG_FILL_PATH | VG_STROKE_PATH)) {
            m_pCtx->setError(VG_BAD_HANDLE_ERROR);
            return;
        }

        // Set stroke / fill 
        if (paintModes & VG_STROKE_PATH)
            m_pCtx->setStrokePaint((IPaint*)paint);
        if (paintModes & VG_FILL_PATH)
            m_pCtx->setFillPaint((IPaint*)paint);
    }

    VGPaint MkVGContext::GetPaint(VGPaintMode paintModes) {
        if (paintModes & VG_STROKE_PATH)
            return (VGPaint)m_pCtx->getStrokePaint();
        if (paintModes & VG_FILL_PATH)
            return (VGPaint)m_pCtx->getFillPaint();
        m_pCtx->setError(VG_ILLEGAL_ARGUMENT_ERROR);
        return VG_INVALID_HANDLE;
    }


    void MkVGContext::PaintPattern(VGPaint paint, VGImage pattern)
    {
        // todo	((SHPaint*)paint)->pattern = pattern;
    }


    VGPath MkVGContext::CreatePath(VGint pathFormat, VGPathDatatype datatype, VGfloat scale, VGfloat bias, VGint segmentCapacityHint, VGint coordCapacityHint, VGbitfield capabilities) {
        IPath* path = m_pCtx->createPath(pathFormat, datatype, scale, bias, segmentCapacityHint, coordCapacityHint, capabilities &= VG_PATH_CAPABILITY_ALL);

        return (VGPath)path;
    }

    void MkVGContext::DestroyPath(VGPath path) {
        if (path) {
            m_pCtx->destroyPath((IPath*)path);
            path = VG_INVALID_HANDLE;
        }
    }

     void MkVGContext::AppendPathData(VGPath dstPath, VGint numSegments, const VGubyte * pathSegments, const void * pathData) {
        IPath* path = (IPath*)dstPath;
        path->appendData(numSegments, pathSegments, pathData);
    }

     void MkVGContext::DrawPath(VGPath path, VGbitfield paintModes) {
        if (path == VG_INVALID_HANDLE) {
            m_pCtx->setError(VG_BAD_HANDLE_ERROR);
            return;
        }

        // force image matrix mode
        if (m_pCtx->getMatrixMode() != VG_MATRIX_PATH_USER_TO_SURFACE) {
            m_pCtx->setMatrixMode(VG_MATRIX_PATH_USER_TO_SURFACE);
        }

        IPath* p = (IPath*)path;


        p->draw(paintModes);
    }

     void MkVGContext::ClearPath(VGPath path, VGbitfield capabilities)  {
        if (path == VG_INVALID_HANDLE) {
            m_pCtx->setError(VG_BAD_HANDLE_ERROR);
            return;
        }

        IPath* p = (IPath*)path;
        p->clear(capabilities);

    }

     void MkVGContext::TransformPath(VGPath dstPath, VGPath srcPath)  {
        IPath* dp = (IPath*)dstPath;
        dp->copy(*(IPath*)srcPath, m_pCtx->getPathUserToSurface());
    }

     void MkVGContext::PathBounds(VGPath path,
        VGfloat * minX, VGfloat * minY,
        VGfloat * width, VGfloat * height)  {

        IPath* p = (IPath*)path;
        p->buildFillIfDirty();	// NOTE: according to the OpenVG specs we only care about the fill bounds, NOT the fill + stroke
        *minX = p->getMinX();
        *minY = p->getMinY();
        *width = p->getWidth();
        *height = p->getHeight();

    }

     void MkVGContext::PathTransformedBounds(VGPath path,
        VGfloat * minX, VGfloat * minY,
        VGfloat * width, VGfloat * height)  {
        IPath* p = (IPath*)path;
        p->buildFillIfDirty();	// NOTE: according to the OpenVG specs we only care about the fill bounds, NOT the fill + stroke
        float x = p->getMinX();
        float y = p->getMinX();
        float w = p->getWidth();
        float h = p->getHeight();

        float p0[2];
        p0[0] = x;
        p0[1] = y;
        float p1[2];
        p1[0] = x + w;
        p1[1] = y;
        float p2[2];
        p2[0] = x + w;
        p2[1] = y + h;
        float p3[2];
        p3[0] = x;
        p3[1] = y + h;

        const Matrix33 & m = m_pCtx->getPathUserToSurface();

        float t0[2];
        affineTransform(t0, m, p0);
        float t1[2];
        affineTransform(t1, m, p1);
        float t2[2];
        affineTransform(t2, m, p2);
        float t3[2];
        affineTransform(t3, m, p3);

        *minX = std::min(std::min(std::min(t0[0], t1[0]), t2[0]), t3[0]);
        *width = std::max(std::max(std::max(t0[0], t1[0]), t2[0]), t3[0]) - *minX;
        *minY = std::min(std::min(std::min(t0[1], t1[1]), t2[1]), t3[1]);
        *height = std::max(std::max(std::max(t0[1], t1[1]), t2[1]), t3[1]) - *minY;
    }

     VGBatchMNK MkVGContext::CreateBatchMNK() {
         return (VGBatchMNK)m_pCtx->createBatch();
     }

     void MkVGContext::DestroyBatchMNK(VGBatchMNK batch) {
         m_pCtx->destroyBatch((IBatch*)batch);
     }
     void MkVGContext::BeginBatchMNK(VGBatchMNK batch) {
         m_pCtx->startBatch((IBatch*)batch);
     }
     void MkVGContext::EndBatchMNK(VGBatchMNK batch) {
         m_pCtx->endBatch((IBatch*)batch);
     }
     void MkVGContext::DrawBatchMNK(VGBatchMNK batch) {
         ((IBatch*)batch)->draw();
     }
     void MkVGContext::DumpBatchMNK(VGBatchMNK batch, void **vertices, size_t * size) {
         m_pCtx->dumpBatch((IBatch *)batch, vertices, size);
     }


     VGFont MkVGContext::CreateFont(VGint glyphCapacityHint) {
         return (VGFont)m_pCtx->createFont();
     }
     void MkVGContext::DestroyFont(VGFont font) {
         m_pCtx->destroyFont((IFont*)font);
     }
     void MkVGContext::SetGlyphToPath(VGFont font,
         VGuint glyphIndex,
         VGPath path,
         VGboolean isHinted,
         VGfloat glyphOrigin[2],
         VGfloat escapement[2]) {
         if (font == VG_INVALID_HANDLE) {
             m_pCtx->setError(VG_BAD_HANDLE_ERROR);
             return;
         }

         IFont* f = (IFont*)font;
         f->addGlyphPath(glyphIndex, (IPath*)path, glyphOrigin, escapement);

     }
     void MkVGContext::SetGlyphToImage(VGFont font,
         VGuint glyphIndex,
         VGImage image,
         VGfloat glyphOrigin[2],
         VGfloat escapement[2]) {
         if (font == VG_INVALID_HANDLE) {
             m_pCtx->setError(VG_BAD_HANDLE_ERROR);
             return;
         }

         IFont* f = (IFont*)font;
         f->addGlyphImage(glyphIndex, (IImage*)image, glyphOrigin, escapement);

     }
     void MkVGContext::ClearGlyph(VGFont font, VGuint glyphIndex) {

     }
     void MkVGContext::DrawGlyph(VGFont font,
         VGuint glyphIndex,
         VGbitfield paintModes,
         VGboolean allowAutoHinting) {
         if (font == VG_INVALID_HANDLE) {
             m_pCtx->setError(VG_BAD_HANDLE_ERROR);
             return;
         }

         // force glyph matrix mode
         if (m_pCtx->getMatrixMode() != VG_MATRIX_GLYPH_USER_TO_SURFACE) {
             m_pCtx->setMatrixMode(VG_MATRIX_GLYPH_USER_TO_SURFACE);
         }
         IFont* f = (IFont*)font;
         f->drawGlyph(glyphIndex, 0, 0, paintModes);


     }
     void MkVGContext::DrawGlyphs(VGFont font,
         VGint glyphCount,
         VGuint *glyphIndices,
         VGfloat *adjustments_x,
         VGfloat *adjustments_y,
         VGbitfield paintModes,
         VGboolean allowAutoHinting) {

         if (font == VG_INVALID_HANDLE) {
             m_pCtx->setError(VG_BAD_HANDLE_ERROR);
             return;
         }

         IFont* f = (IFont*)font;

         // force glyph matrix mode
         if (m_pCtx->getMatrixMode() != VG_MATRIX_GLYPH_USER_TO_SURFACE) {
             m_pCtx->setMatrixMode(VG_MATRIX_GLYPH_USER_TO_SURFACE);
         }

         for (int i = 0; i < glyphCount; i++) {
             VGfloat ax = 0, ay = 0;
             if (adjustments_x) {
                 ax = adjustments_x[i];
             }
             if (adjustments_y) {
                 ay = adjustments_y[i];
             }

             f->drawGlyph(glyphIndices[i], ax, ay, paintModes);
         }
     }

     VGImage MkVGContext::CreateImage(VGImageFormat format,
         VGint width, VGint height,
         VGbitfield allowedQuality) {
         return (VGImage)m_pCtx->createImage(format, width, height, allowedQuality);
     }
     void MkVGContext::DestroyImage(VGImage image) {
         m_pCtx->destroyImage((IImage*)image);
     }
     void MkVGContext::ClearImage(VGImage image,
         VGint x, VGint y, VGint width, VGint height) {

     }
     void MkVGContext::ImageSubData(VGImage image,
         const void * data, VGint dataStride,
         VGImageFormat dataFormat,
         VGint x, VGint y, VGint width, VGint height) {
         if (image == VG_INVALID_HANDLE) {
             m_pCtx->setError(VG_BAD_HANDLE_ERROR);
             return;
         }

         IImage* mkImage = (IImage*)image;
         mkImage->setSubData(data, dataStride, dataFormat, x, y, width, height);

     }
     void MkVGContext::GetImageSubData(VGImage image,
         void * data, VGint dataStride,
         VGImageFormat dataFormat,
         VGint x, VGint y,
         VGint width, VGint height) {

     }
     VGImage MkVGContext::ChildImage(VGImage parent,
         VGint x, VGint y, VGint width, VGint height) {

         if (parent == VG_INVALID_HANDLE) {
             m_pCtx->setError(VG_BAD_HANDLE_ERROR);
             return VG_INVALID_HANDLE;
         }

         IImage* p = (IImage*)parent;
         IImage* child = p->createChild(x, y, width, height);

         return (VGImage)child;

     }
     void MkVGContext::CopyImage(VGImage dst, VGint dx, VGint dy,
         VGImage src, VGint sx, VGint sy,
         VGint width, VGint height,
         VGboolean dither) {

     }
     void MkVGContext::DrawImage(VGImage image) {
         if (image == VG_INVALID_HANDLE) {
             m_pCtx->setError(VG_BAD_HANDLE_ERROR);
             return;
         }

         // force image matrix mode
         if (m_pCtx->getMatrixMode() != VG_MATRIX_IMAGE_USER_TO_SURFACE) {
             m_pCtx->setMatrixMode(VG_MATRIX_IMAGE_USER_TO_SURFACE);
         }

         IImage* mkImage = (IImage*)image;
         mkImage->draw();
     }
     void MkVGContext::SetPixels(VGint dx, VGint dy,
         VGImage src, VGint sx, VGint sy,
         VGint width, VGint height) {

     }
     void MkVGContext::WritePixels(const void * data, VGint dataStride,
         VGImageFormat dataFormat,
         VGint dx, VGint dy,
         VGint width, VGint height) {

     }
     void MkVGContext::GetPixels(VGImage dst, VGint dx, VGint dy,
         VGint sx, VGint sy,
         VGint width, VGint height) {

     }
     void MkVGContext::ReadPixels(void * data, VGint dataStride,
         VGImageFormat dataFormat,
         VGint sx, VGint sy,
         VGint width, VGint height) {

     }
     void MkVGContext::CopyPixels(VGint dx, VGint dy,
         VGint sx, VGint sy,
         VGint width, VGint height) {
     }

     void MkVGContext::LoadIdentity(void) {
         m_pCtx->setIdentity();
     }

     void MkVGContext::LoadMatrix(const VGfloat * m) {
         m_pCtx->setTransform(m);
     }

     void MkVGContext::GetMatrix(VGfloat * m) {
         m_pCtx->transform(m);
     }

     void MkVGContext::MultMatrix(const VGfloat * m) {
         m_pCtx->multiply(m);
     }

     void MkVGContext::Translate(VGfloat tx, VGfloat ty) {
         m_pCtx->translate(tx, ty);
     }

     void MkVGContext::Scale(VGfloat sx, VGfloat sy) {
         m_pCtx->scale(sx, sy);
     }

     //void Shear(VGfloat shx, VGfloat shy) VG_API_EXIT;

     void MkVGContext::Rotate(VGfloat angle) {
         m_pCtx->rotate(angle);
     }


     VGfloat MkVGContext::GetParameterf(VGHandle object, VGint paramType)
     {
         BaseObject *obj = (BaseObject*)object;
         return obj->getParameterf(paramType);
     }

     VGint MkVGContext::GetParameteri(VGHandle object, VGint paramType)
     {
         BaseObject *obj = (BaseObject*)object;
         return obj->getParameteri(paramType);
     }

     void MkVGContext::SetParameterf(VGHandle object, VGint paramType, VGfloat value)
     {
         BaseObject *obj = (BaseObject*)object;
         obj->setParameter(paramType, value);
     }

     void MkVGContext::SetParameterfv(VGHandle object,
         VGint paramType,
         VGint count, const VGfloat * values) {
         BaseObject *obj = (BaseObject*)object;
         obj->setParameter(paramType, values, count);
     }


     void MkVGContext::SetParameteri(VGHandle object, VGint paramType, VGint value)
     {
         BaseObject *obj = (BaseObject*)object;
         obj->setParameter(paramType, value);
     }

     VGint MkVGContext::GetParameterVectorSize(VGHandle object,
         VGint paramType)
     {
         return -1;
     }

     void MkVGContext::GetParameterfv(VGHandle object,
         VGint paramType,
         VGint count, VGfloat * values)
     {
         BaseObject* obj = (BaseObject*)object;
         obj->getParameterfv(paramType, values);
     }

     void MkVGContext::GetParameteriv(VGHandle object,
         VGint paramType,
         VGint count, VGint * values)
     {

     }
}

void vgAppendPathData(VGPath dstPath, VGint numSegments, const VGubyte * pathSegments, const void * pathData) {
    IPath* path = (IPath*)dstPath;
    path->appendData(numSegments, pathSegments, pathData);
}

