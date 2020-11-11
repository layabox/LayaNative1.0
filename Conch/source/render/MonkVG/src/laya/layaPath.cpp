
#include "layaPath.h"
#include "../mkContext.h"
#include "layaContext.h"
#include "layaPaint.h"
#include <3DRS/JCRenderer.h>
#include <3DRS/JCRenderGroupData.h>

namespace MonkVG {
    // Given: Points (x0, y0) and (x1, y1) 
    // Return: TRUE if a solution exists, FALSE otherwise 
    //	Circle centers are written to (cx0, cy0) and (cx1, cy1) 
    static VGboolean findUnitCircles(VGfloat x0, VGfloat y0, VGfloat x1, VGfloat y1,
        VGfloat *cx0, VGfloat *cy0, VGfloat *cx1, VGfloat *cy1) {

        // Compute differences and averages
        VGfloat dx = x0 - x1;
        VGfloat dy = y0 - y1;
        VGfloat xm = (x0 + x1) / 2;
        VGfloat ym = (y0 + y1) / 2;
        VGfloat dsq, disc, s, sdx, sdy;
        // Solve for intersecting unit circles 
        dsq = dx*dx + dy*dy;
        if (dsq == 0.0)
            return VG_FALSE; // Points are coincident 
        disc = 1.0f / dsq - 1.0f / 4.0f;
        if (disc < 0.0)
            return VG_FALSE; // Points are too far apart 

        s = sqrt(disc);
        sdx = s*dx;
        sdy = s*dy;
        *cx0 = xm + sdy;
        *cy0 = ym - sdx;
        *cx1 = xm - sdy;
        *cy1 = ym + sdx;

        return VG_TRUE;
    }

    //Given: 
    //Return: 
    //Ellipse parameters rh, rv, rot (in degrees), endpoints (x0, y0) and (x1, y1) TRUE if a solution exists, FALSE otherwise. Ellipse centers are written to (cx0, cy0) and (cx1, cy1)

    static VGboolean findEllipses(VGfloat rh, VGfloat rv, VGfloat rot,
        VGfloat x0, VGfloat y0, VGfloat x1, VGfloat y1,
        VGfloat *cx0, VGfloat *cy0, VGfloat *cx1, VGfloat *cy1) {
        VGfloat COS, SIN, x0p, y0p, x1p, y1p, pcx0, pcy0, pcx1, pcy1;
        // Convert rotation angle from degrees to radians 
        rot *= M_PI / 180.0f;
        // Pre-compute rotation matrix entries 
        COS = cos(rot);
        SIN = sin(rot);
        // Transform (x0, y0) and (x1, y1) into unit space 
        // using (inverse) rotate, followed by (inverse) scale	
        x0p = (x0*COS + y0*SIN) / rh;
        y0p = (-x0*SIN + y0*COS) / rv;
        x1p = (x1*COS + y1*SIN) / rh;
        y1p = (-x1*SIN + y1*COS) / rv;
        if (!findUnitCircles(x0p, y0p, x1p, y1p, &pcx0, &pcy0, &pcx1, &pcy1)) {
            return VG_FALSE;
        }
        // Transform back to original coordinate space 
        // using (forward) scale followed by (forward) rotate 
        pcx0 *= rh;
        pcy0 *= rv;
        pcx1 *= rh;
        pcy1 *= rv;
        *cx0 = pcx0*COS - pcy0*SIN;
        *cy0 = pcx0*SIN + pcy0*COS;
        *cx1 = pcx1*COS - pcy1*SIN;
        *cy1 = pcx1*SIN + pcy1*COS;

        return VG_TRUE;
    }


    static inline VGfloat calcCubicBezier1d(VGfloat x0, VGfloat x1, VGfloat x2, VGfloat x3, VGfloat t) {
        // see openvg 1.0 spec section 8.3.2 Cubic Bezier Curves
        VGfloat oneT = 1.0f - t;
        VGfloat x = x0 * (oneT * oneT * oneT)
            + 3.0f * x1 * (oneT * oneT) * t
            + 3.0f * x2 * oneT * (t * t)
            + x3 * (t * t * t);
        return x;
    }

    static inline VGfloat calcQuadBezier1d(VGfloat start, VGfloat control, VGfloat end, VGfloat time) {
        float inverseTime = 1.0f - time;
        return (powf(inverseTime, 2.0f) * start) + (2.0f * inverseTime * time * control) + (powf(time, 2.0f) * end);
    }

#ifndef M_PI 
#define M_PI 3.14159265358979323846 
#endif

    layaPath::~layaPath() {
        if (_fillTesseleator) {
            gluDeleteTess(_fillTesseleator);
            _fillTesseleator = 0;
        }
    }


    bool layaPath::draw(VGbitfield paintModes) {
        if (paintModes == 0)
            return false;

        if (paintModes & VG_FILL_PATH) {	// build the fill polygons
            buildFillIfDirty();
        }

        if (paintModes & VG_STROKE_PATH && (_isStrokeDirty == true )) {
            buildStroke();
            _isStrokeDirty = false;
        }

        endOfTesselation(paintModes);

        //这里已经得到了几何数据，可以给laya渲染了。
        layaContext& glContext = (MonkVG::layaContext&)*m_pCtx;
        glContext.render(this, paintModes);
        _fillVertices.clear();
        _strokeVertices.clear();
        return true;
    }
    void layaPath::clear(VGbitfield caps) {
        IPath::clear(caps);
        _vertices.clear();
    }

    void layaPath::buildFatLineSegment(vector<laya::RectGeometry::VertexInfo>& vertices, const v2_t& p0, const v2_t& p1, const float stroke_width) {

        if ((p0.x == p1.x) && (p0.y == p1.y)) {
            return;
        }
        static VGfloat sColor[] = { 1.0f,1.0f,1.0f,1.0f };
        const VGfloat* pCol = sColor;
        if (_strokePaintForPath) {
            pCol = _strokePaintForPath->getPaintColor();
        }

        float dx = p1.y - p0.y;
        float dy = p0.x - p1.x;
        const float inv_mag = 1.0f / sqrtf(dx*dx + dy*dy);
        dx = dx * inv_mag;
        dy = dy * inv_mag;

        laya::RectGeometry::VertexInfo vo;
        vo.r = pCol[0];
        vo.g = pCol[1];
        vo.b = pCol[2];
        vo.a = 1.0f;
        //vo.u = pCol[0];
        //vo.v = pCol[1];
        //vo.w = pCol[2];
        const float radius = stroke_width * 0.5f;

        vo.x = p0.x + radius * dx;
        vo.y = p0.y + radius * dy;
        vertices.push_back(vo);

        vo.x = p0.x - radius * dx;
        vo.y = p0.y - radius * dy;
        vertices.push_back(vo);

        vo.x = p1.x + radius * dx;
        vo.y = p1.y + radius * dy;
        vertices.push_back(vo);

        vo.x = p1.x - radius * dx;
        vo.y = p1.y - radius * dy;
        vertices.push_back(vo);

    }

    void layaPath::buildFillIfDirty() {
        IPaint* currentFillPaint = m_pCtx->getFillPaint();
        if (currentFillPaint != _fillPaintForPath) {
            _fillPaintForPath = (layaPaint*)currentFillPaint;
            _isFillDirty = true;
        }
        // only build the fill if dirty or we are in batch build mode
        if (_isFillDirty || m_pCtx->currentBatch()) {
            buildFill();
        }
        _isFillDirty = false;
    }

    void layaPath::buildStroke() {
        _strokeVertices.clear();
        _strokeVertices.reserve(100);
        IPaint* curStrokePaint = m_pCtx->getStrokePaint();
        if (curStrokePaint != _strokePaintForPath) {
            _strokePaintForPath = (layaPaint*)curStrokePaint;
            _isStrokeDirty = true;
        }
        layaContext& glContext = (layaContext&)*m_pCtx;

        const VGfloat stroke_width = glContext.getStrokeLineWidth();

        //vector< VGfloat >::iterator coordsIter = _fcoords->begin();
        VGfloat* coordsIter = NULL;
        if (_fcoords->size()>0)
            coordsIter = &_fcoords->operator[](0);
        VGbyte segment = VG_CLOSE_PATH;
        v2_t coords = { 0,0 };
        v2_t prev = { 0,0 };
        v2_t closeTo = { 0,0 };
        for (vector< VGubyte >::iterator segmentIter = _segments.begin(); segmentIter != _segments.end(); segmentIter++) {
            segment = (*segmentIter);
            //int numCoords = segmentToNumCoordinates( static_cast<VGPathSegment>( segment ) );
            //segment = segment >> 1;


            //			VG_CLOSE_PATH                               = ( 0 << 1),
            //			VG_MOVE_TO                                  = ( 1 << 1),
            //			VG_LINE_TO                                  = ( 2 << 1),
            //			VG_HLINE_TO                                 = ( 3 << 1),
            //			VG_VLINE_TO                                 = ( 4 << 1),
            //			VG_QUAD_TO                                  = ( 5 << 1),
            //			VG_CUBIC_TO                                 = ( 6 << 1),
            //			VG_SQUAD_TO                                 = ( 7 << 1),
            //			VG_SCUBIC_TO                                = ( 8 << 1),
            //			VG_SCCWARC_TO                               = ( 9 << 1),
            //			VG_SCWARC_TO                                = (10 << 1),
            //			VG_LCCWARC_TO                               = (11 << 1),
            //			VG_LCWARC_TO                                = (12 << 1),

            // todo: deal with relative move
            bool isRelative = segment & VG_RELATIVE;
            switch (segment >> 1) {
            case (VG_CLOSE_PATH >> 1) :
            {
                buildFatLineSegment(_strokeVertices, coords, closeTo, stroke_width);
            } break;
            case (VG_MOVE_TO >> 1) :
            {
                prev.x = closeTo.x = coords.x = *coordsIter; coordsIter++;
                prev.y = closeTo.y = coords.y = *coordsIter; coordsIter++;

            } break;
            case (VG_LINE_TO >> 1) :
            {
                prev = coords;
                coords.x = *coordsIter; coordsIter++;
                coords.y = *coordsIter; coordsIter++;
                if (isRelative) {
                    coords.x += prev.x;
                    coords.y += prev.y;
                }

                buildFatLineSegment(_strokeVertices, prev, coords, stroke_width);


            } break;
            case (VG_HLINE_TO >> 1) :
            {
                prev = coords;
                coords.x = *coordsIter; coordsIter++;
                if (isRelative) {
                    coords.x += prev.x;
                }

                buildFatLineSegment(_strokeVertices, prev, coords, stroke_width);
            } break;
            case (VG_VLINE_TO >> 1) :
            {
                prev = coords;
                coords.y = *coordsIter; coordsIter++;
                if (isRelative) {
                    coords.y += prev.y;
                }

                buildFatLineSegment(_strokeVertices, prev, coords, stroke_width);

            } break;
            case (VG_SCUBIC_TO >> 1) :
            {
                prev = coords;
                VGfloat cp2x = *coordsIter; coordsIter++;
                VGfloat cp2y = *coordsIter; coordsIter++;
                VGfloat p3x = *coordsIter; coordsIter++;
                VGfloat p3y = *coordsIter; coordsIter++;


                if (isRelative) {
                    cp2x += prev.x;
                    cp2y += prev.y;
                    p3x += prev.x;
                    p3y += prev.y;
                }

                VGfloat cp1x = 2.0f * cp2x - p3x;
                VGfloat cp1y = 2.0f * cp2y - p3y;


                VGfloat increment = 1.0f / m_pCtx->getTessellationIterations();
                //printf("\tcubic: ");
                for (VGfloat t = increment; t < 1.0f + increment; t += increment) {
                    v2_t c;
                    c.x = calcCubicBezier1d(coords.x, cp1x, cp2x, p3x, t);
                    c.y = calcCubicBezier1d(coords.y, cp1y, cp2y, p3y, t);
                    buildFatLineSegment(_strokeVertices, prev, c, stroke_width);
                    prev = c;
                }
                coords.x = p3x;
                coords.y = p3y;

            }
                                     break;

            case (VG_QUAD_TO >> 1) :     // added by rhcad
            {
                prev = coords;
                VGfloat cpx = *coordsIter; coordsIter++;
                VGfloat cpy = *coordsIter; coordsIter++;
                VGfloat px = *coordsIter; coordsIter++;
                VGfloat py = *coordsIter; coordsIter++;

                if (isRelative) {
                    cpx += prev.x;
                    cpy += prev.y;
                    px += prev.x;
                    py += prev.y;
                }

                VGfloat increment = 1.0f / m_pCtx->getTessellationIterations();

                for (VGfloat t = increment; t < 1.0f + increment; t += increment) {
                    v2_t c;
                    c.x = calcQuadBezier1d(coords.x, cpx, px, t);
                    c.y = calcQuadBezier1d(coords.y, cpy, py, t);
                    buildFatLineSegment(_strokeVertices, prev, c, stroke_width);
                    prev = c;
                }
                coords.x = px;
                coords.y = py;

            } break;

            case (VG_CUBIC_TO >> 1) :	// todo
            {
                prev = coords;
                VGfloat cp1x = *coordsIter; coordsIter++;
                VGfloat cp1y = *coordsIter; coordsIter++;
                VGfloat cp2x = *coordsIter; coordsIter++;
                VGfloat cp2y = *coordsIter; coordsIter++;
                VGfloat p3x = *coordsIter; coordsIter++;
                VGfloat p3y = *coordsIter; coordsIter++;

                if (isRelative) {
                    cp1x += prev.x;
                    cp1y += prev.y;
                    cp2x += prev.x;
                    cp2y += prev.y;
                    p3x += prev.x;
                    p3y += prev.y;
                }


                VGfloat increment = 1.0f / m_pCtx->getTessellationIterations();

                for (VGfloat t = increment; t < 1.0f + increment; t += increment) {
                    v2_t c;
                    c.x = calcCubicBezier1d(coords.x, cp1x, cp2x, p3x, t);
                    c.y = calcCubicBezier1d(coords.y, cp1y, cp2y, p3y, t);
                    buildFatLineSegment(_strokeVertices, prev, c, stroke_width);
                    prev = c;
                }
                coords.x = p3x;
                coords.y = p3y;

            } break;
            case (VG_SCCWARC_TO >> 1) :
            case (VG_SCWARC_TO >> 1) :
            case (VG_LCCWARC_TO >> 1) :
            case (VG_LCWARC_TO >> 1) :

            {
                VGfloat rh = *coordsIter; coordsIter++;
                VGfloat rv = *coordsIter; coordsIter++;
                VGfloat rot = *coordsIter; coordsIter++;
                VGfloat cp1x = *coordsIter; coordsIter++;
                VGfloat cp1y = *coordsIter; coordsIter++;
                if (isRelative) {
                    cp1x += prev.x;
                    cp1y += prev.y;
                }


                // convert to Center Parameterization (see OpenVG Spec Apendix A)
                VGfloat cx0[2];
                VGfloat cx1[2];
                VGboolean success = findEllipses(rh, rv, rot,
                    coords.x, coords.y, cp1x, cp1y,
                    &cx0[0], &cx0[1], &cx1[0], &cx1[1]);

                if (success) {
                    // see: http://en.wikipedia.org/wiki/Ellipse#Ellipses_in_computer_graphics 
                    const int steps = m_pCtx->getTessellationIterations();
                    VGfloat beta = 0;	// angle. todo
                    VGfloat sinbeta = sinf(beta);
                    VGfloat cosbeta = cosf(beta);

                    // calculate the start and end angles
                    v2_t center;
                    center.x = cx0[0];//(cx0[0] + cx1[0])*0.5f;
                    center.y = cx0[1];//(cx0[1] + cx1[1])*0.5f;
                    v2_t norm[2];
                    norm[0].x = center.x - coords.x;
                    norm[0].y = center.y - coords.y;
                    VGfloat inverse_len = 1.0f / sqrtf((norm[0].x * norm[0].x) + (norm[0].y * norm[0].y));
                    norm[0].x *= inverse_len;
                    norm[0].y *= inverse_len;

                    norm[1].x = center.x - cp1x;
                    norm[1].y = center.y - cp1y;
                    inverse_len = 1.0f / sqrtf((norm[1].x * norm[1].x) + (norm[1].y * norm[1].y));
                    norm[1].x *= inverse_len;
                    norm[1].y *= inverse_len;
                    VGfloat startAngle = degrees(acosf(-norm[0].x));
                    VGfloat endAngle = degrees(acosf(-norm[1].x));
                    VGfloat cross = norm[0].x;
                    if (cross >= 0) {
                        startAngle = 360 - startAngle;
                        endAngle = 360 - endAngle;
                    }
                    if (startAngle > endAngle) {
                        VGfloat tmp = startAngle;
                        startAngle = endAngle;
                        endAngle = tmp;
                        startAngle = startAngle - 90;
                        endAngle = endAngle - 90;
                    }


                    prev = coords;
                    for (VGfloat g = startAngle; g < endAngle + (360 / steps); g += 360 / steps) {
                        v2_t c;

                        VGfloat alpha = g * (M_PI / 180.0f);
                        VGfloat sinalpha = sinf(alpha);
                        VGfloat cosalpha = cosf(alpha);
                        c.x = cx0[0] + (rh * cosalpha * cosbeta - rv * sinalpha * sinbeta);
                        c.y = cx0[1] + (rh * cosalpha * sinbeta + rv * sinalpha * cosbeta);
                        //printf( "(%f, %f)\n", c[0], c[1] );
                        buildFatLineSegment(_strokeVertices, prev, c, stroke_width);
                        prev = c;
                    }
                }

                coords.x = cp1x;
                coords.y = cp1y;

            } break;
            case (VG_LINE_DATA >> 1) :
            {
                int vertnum = *(int*)coordsIter; coordsIter++;
                prev = coords;
                for (int i = 0; i < vertnum; i++) {
                    coords.x = *coordsIter; coordsIter++;
                    coords.y = *coordsIter; coordsIter++;
                    buildFatLineSegment(_strokeVertices, prev, coords, stroke_width);
                    prev = coords;
                }
            }break;
            default:
                printf("unkwown command: %d\n", segment >> 1);
                break;
            }
        }	// foreach segment
    }

    void layaPath::buildFill() {
        _vertices.clear();

        // reset the bounds
        _minX = VG_MAX_FLOAT;
        _minY = VG_MAX_FLOAT;
        _width = -VG_MAX_FLOAT;
        _height = -VG_MAX_FLOAT;


        _fillTesseleator = gluNewTess();
        gluTessCallback(_fillTesseleator, GLU_TESS_BEGIN_DATA, (GLvoid(APIENTRY *) ()) &layaPath::tessBegin);
        gluTessCallback(_fillTesseleator, GLU_TESS_END_DATA, (GLvoid(APIENTRY *) ()) &layaPath::tessEnd);
        gluTessCallback(_fillTesseleator, GLU_TESS_VERTEX_DATA, (GLvoid(APIENTRY *) ()) &layaPath::tessVertex);
        gluTessCallback(_fillTesseleator, GLU_TESS_COMBINE_DATA, (GLvoid(APIENTRY *) ()) &layaPath::tessCombine);
        gluTessCallback(_fillTesseleator, GLU_TESS_ERROR, (GLvoid(APIENTRY *)())&layaPath::tessError);
        if (m_pCtx->getFillRule() == VG_EVEN_ODD) {
            gluTessProperty(_fillTesseleator, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
        }
        else if (m_pCtx->getFillRule() == VG_NON_ZERO) {
            gluTessProperty(_fillTesseleator, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
        }
        gluTessProperty(_fillTesseleator, GLU_TESS_TOLERANCE, 0.5f);

        gluTessBeginPolygon(_fillTesseleator, this);


        //vector< VGfloat >::iterator coordsIter = _fcoords->begin();
        VGfloat* coordsIter = NULL;
        if (_fcoords->size()>0)
            coordsIter = &_fcoords->operator[](0);

        VGbyte segment = VG_CLOSE_PATH;
        v3_t coords(0, 0, 0);
        v3_t prev(0, 0, 0);
        int num_contours = 0;

        for (vector< VGubyte >::iterator segmentIter = _segments.begin(); segmentIter != _segments.end(); segmentIter++) {
            segment = (*segmentIter);
            //int numCoords = segmentToNumCoordinates( static_cast<VGPathSegment>( segment ) );
            //segment = segment >> 1;


            //			VG_CLOSE_PATH                               = ( 0 << 1),
            //			VG_MOVE_TO                                  = ( 1 << 1),
            //			VG_LINE_TO                                  = ( 2 << 1),
            //			VG_HLINE_TO                                 = ( 3 << 1),
            //			VG_VLINE_TO                                 = ( 4 << 1),
            //			VG_QUAD_TO                                  = ( 5 << 1),
            //			VG_CUBIC_TO                                 = ( 6 << 1),
            //			VG_SQUAD_TO                                 = ( 7 << 1),
            //			VG_SCUBIC_TO                                = ( 8 << 1),
            //			VG_SCCWARC_TO                               = ( 9 << 1),
            //			VG_SCWARC_TO                                = (10 << 1),
            //			VG_LCCWARC_TO                               = (11 << 1),
            //			VG_LCWARC_TO                                = (12 << 1),

            // todo: deal with relative move
            bool isRelative = segment & VG_RELATIVE;
            switch (segment >> 1) {
            case (VG_CLOSE_PATH >> 1) :
            {
                if (num_contours) {
                    gluTessEndContour(_fillTesseleator);
                    num_contours--;
                }

            } break;
            case (VG_MOVE_TO >> 1) :
            {
                if (num_contours) {
                    gluTessEndContour(_fillTesseleator);
                    num_contours--;
                }

                gluTessBeginContour(_fillTesseleator);
                num_contours++;
                coords.x = *coordsIter; coordsIter++;
                coords.y = *coordsIter; coordsIter++;

                GLdouble * l = addTessVertex(coords);
                gluTessVertex(_fillTesseleator, l, l);

            } break;
            case (VG_LINE_TO >> 1) :
            {
                prev = coords;
                coords.x = *coordsIter; coordsIter++;
                coords.y = *coordsIter; coordsIter++;
                if (isRelative) {
                    coords.x += prev.x;
                    coords.y += prev.y;
                }

                GLdouble * l = addTessVertex(coords);
                gluTessVertex(_fillTesseleator, l, l);
            } break;
            case (VG_HLINE_TO >> 1) :
            {
                prev = coords;
                coords.x = *coordsIter; coordsIter++;
                if (isRelative) {
                    coords.x += prev.x;
                }

                GLdouble * l = addTessVertex(coords);
                gluTessVertex(_fillTesseleator, l, l);
            } break;
            case (VG_VLINE_TO >> 1) :
            {
                prev = coords;
                coords.y = *coordsIter; coordsIter++;
                if (isRelative) {
                    coords.y += prev.y;
                }

                GLdouble * l = addTessVertex(coords);
                gluTessVertex(_fillTesseleator, l, l);
            } break;
            case (VG_SCUBIC_TO >> 1) :
            {
                prev = coords;
                VGfloat cp2x = *coordsIter; coordsIter++;
                VGfloat cp2y = *coordsIter; coordsIter++;
                VGfloat p3x = *coordsIter; coordsIter++;
                VGfloat p3y = *coordsIter; coordsIter++;


                if (isRelative) {
                    cp2x += prev.x;
                    cp2y += prev.y;
                    p3x += prev.x;
                    p3y += prev.y;
                }

                VGfloat cp1x = 2.0f * cp2x - p3x;
                VGfloat cp1y = 2.0f * cp2y - p3y;


                VGfloat increment = 1.0f / m_pCtx->getTessellationIterations();
                //printf("\tcubic: ");
                for (VGfloat t = increment; t < 1.0f + increment; t += increment) {
                    v3_t c;
                    c.x = calcCubicBezier1d(coords.x, cp1x, cp2x, p3x, t);
                    c.y = calcCubicBezier1d(coords.y, cp1y, cp2y, p3y, t);
                    c.z = 0;
                    GLdouble * l = addTessVertex(c);
                    gluTessVertex(_fillTesseleator, l, l);
                    //	c.print();
                }
                //printf("\n");
                coords.x = p3x;
                coords.y = p3y;

            }
                                     break;
            case (VG_CUBIC_TO >> 1) :
            {
                prev = coords;
                VGfloat cp1x = *coordsIter; coordsIter++;
                VGfloat cp1y = *coordsIter; coordsIter++;
                VGfloat cp2x = *coordsIter; coordsIter++;
                VGfloat cp2y = *coordsIter; coordsIter++;
                VGfloat p3x = *coordsIter; coordsIter++;
                VGfloat p3y = *coordsIter; coordsIter++;

                if (isRelative) {
                    cp1x += prev.x;
                    cp1y += prev.y;
                    cp2x += prev.x;
                    cp2y += prev.y;
                    p3x += prev.x;
                    p3y += prev.y;
                }

                VGfloat increment = 1.0f / m_pCtx->getTessellationIterations();
                //printf("\tcubic: ");
                for (VGfloat t = increment; t < 1.0f + increment; t += increment) {
                    v3_t c;
                    c.x = calcCubicBezier1d(coords.x, cp1x, cp2x, p3x, t);
                    c.y = calcCubicBezier1d(coords.y, cp1y, cp2y, p3y, t);
                    c.z = 0;
                    GLdouble * l = addTessVertex(c);
                    gluTessVertex(_fillTesseleator, l, l);
                    //	c.print();
                }
                //printf("\n");
                coords.x = p3x;
                coords.y = p3y;

            } break;

            case (VG_QUAD_TO >> 1) :
            {
                prev = coords;
                VGfloat cpx = *coordsIter; coordsIter++;
                VGfloat cpy = *coordsIter; coordsIter++;
                VGfloat px = *coordsIter; coordsIter++;
                VGfloat py = *coordsIter; coordsIter++;

                if (isRelative) {
                    cpx += prev.x;
                    cpy += prev.y;
                    px += prev.x;
                    py += prev.y;
                }

                VGfloat increment = 1.0f / m_pCtx->getTessellationIterations();
                for (VGfloat t = increment; t < 1.0f + increment; t += increment) {
                    v3_t c;
                    c.x = calcQuadBezier1d(coords.x, cpx, px, t);
                    c.y = calcQuadBezier1d(coords.y, cpy, py, t);
                    c.z = 0;
                    GLdouble * l = addTessVertex(c);
                    gluTessVertex(_fillTesseleator, l, l);
                }

                coords.x = px;
                coords.y = py;

            } break;

            case (VG_SCCWARC_TO >> 1) :
            case (VG_SCWARC_TO >> 1) :
            case (VG_LCCWARC_TO >> 1) :
            case (VG_LCWARC_TO >> 1) :

            {
                VGfloat rh = *coordsIter; coordsIter++;
                VGfloat rv = *coordsIter; coordsIter++;
                VGfloat rot = *coordsIter; coordsIter++;
                VGfloat cp1x = *coordsIter; coordsIter++;
                VGfloat cp1y = *coordsIter; coordsIter++;
                if (isRelative) {
                    cp1x += prev.x;
                    cp1y += prev.y;
                }

                // convert to Center Parameterization (see OpenVG Spec Apendix A)
                VGfloat cx0[2];
                VGfloat cx1[2];
                VGboolean success = findEllipses(rh, rv, rot,
                    coords.x, coords.y, cp1x, cp1y,
                    &cx0[0], &cx0[1], &cx1[0], &cx1[1]);

                if (success) {
                    // see: http://en.wikipedia.org/wiki/Ellipse#Ellipses_in_computer_graphics 
                    const int steps = m_pCtx->getTessellationIterations();
                    VGfloat beta = 0;	// angle. todo
                    VGfloat sinbeta = sinf(beta);
                    VGfloat cosbeta = cosf(beta);

                    // calculate the start and end angles
                    v2_t center;
                    center.x = cx0[0];
                    center.y = cx0[1];
                    v2_t norm[2];
                    norm[0].x = center.x - coords.x;
                    norm[0].y = center.y - coords.y;
                    VGfloat inverse_len = 1.0f / sqrtf((norm[0].x * norm[0].x) + (norm[0].y * norm[0].y));
                    norm[0].x *= inverse_len;
                    norm[0].y *= inverse_len;

                    norm[1].x = center.x - cp1x;
                    norm[1].y = center.y - cp1y;
                    inverse_len = 1.0f / sqrtf((norm[1].x * norm[1].x) + (norm[1].y * norm[1].y));
                    norm[1].x *= inverse_len;
                    norm[1].y *= inverse_len;
                    VGfloat startAngle = degrees(acosf(-norm[0].x));
                    VGfloat endAngle = degrees(acosf(-norm[1].x));

                    VGfloat cross = norm[0].x;

                    if (cross >= 0) {
                        startAngle = 360 - startAngle;
                        endAngle = 360 - endAngle;
                    }
                    if (startAngle > endAngle) {
                        VGfloat tmp = startAngle;
                        startAngle = endAngle;
                        endAngle = tmp;
                        startAngle = startAngle - 90;
                        endAngle = endAngle - 90;
                    }
                    for (VGfloat g = startAngle; g < endAngle; g += 360 / steps) {
                        v3_t c;

                        VGfloat alpha = g * (M_PI / 180.0f);
                        VGfloat sinalpha = sinf(alpha);
                        VGfloat cosalpha = cosf(alpha);
                        c.x = cx0[0] + (rh * cosalpha * cosbeta - rv * sinalpha * sinbeta);
                        c.y = cx0[1] + (rh * cosalpha * sinbeta + rv * sinalpha * cosbeta);
                        c.z = 0;
                        GLdouble * l = addTessVertex(c);
                        gluTessVertex(_fillTesseleator, l, l);
                    }
                }

                coords.x = cp1x;
                coords.y = cp1y;

            } break;
            case (VG_LINE_DATA >> 1) :
            {
                int vertnum = *(int*)coordsIter; coordsIter++;
                for (int i = 0; i < vertnum; i++) {
                    coords.x = *coordsIter; coordsIter++;
                    coords.y = *coordsIter; coordsIter++;
                    GLdouble * l = addTessVertex(coords);
                    gluTessVertex(_fillTesseleator, l, l);
                }
            }break;
            default:
                printf("unkwown command\n");
                break;
            }
        }	// foreach segment

        if (num_contours) {
            gluTessEndContour(_fillTesseleator);
            num_contours--;
        }

        assert(num_contours == 0);

        gluTessEndPolygon(_fillTesseleator);

        gluDeleteTess(_fillTesseleator);
        _fillTesseleator = 0;

        // final calculation of the width and height
        _width = fabsf(_width - _minX);
        _height = fabsf(_height - _minY);
    }

    void layaPath::endOfTesselation(VGbitfield paintModes) {

        /// build fill vbo
        // TODO: BUGBUG: if in batch mode don't build the VBO!
        if (_vertices.size() > 0) {
            //vertices:x,y.
            _numberFillVertices = (int)_vertices.size() / 2;
            _fillVertices.resize(_numberFillVertices);
            if (_fillPaintForPath && _fillPaintForPath->getPaintType() == VG_PAINT_TYPE_COLOR) {
                IPaint* currentFillPaint = m_pCtx->getFillPaint();
                const VGfloat* pCol = currentFillPaint->getPaintColor();
                int i = 0; 
                for (vector<float>::const_iterator it = _vertices.begin(); it != _vertices.end(); it++) {
                    laya::RectGeometry::VertexInfo& curvert = _fillVertices[i];
                    i++;
                    curvert.x = *it; it++;
                    curvert.y = *it;
                    //curvert.u = pCol[0];
                    //curvert.v = pCol[1];
                    //curvert.w = pCol[2];
                    curvert.r = pCol[0];
                    curvert.g = pCol[1];
                    curvert.b = pCol[2];
                    curvert.a = 1.0f;
                }
            }
            else if (_fillPaintForPath && (_fillPaintForPath->getPaintType() == VG_PAINT_TYPE_LINEAR_GRADIENT || _fillPaintForPath->getPaintType() == VG_PAINT_TYPE_RADIAL_GRADIENT || _fillPaintForPath->getPaintType() == VG_PAINT_TYPE_RADIAL_2x3_GRADIENT || _fillPaintForPath->getPaintType() == VG_PAINT_TYPE_LINEAR_2x3_GRADIENT)) {
                int i = 0;
                for (vector<float>::const_iterator it = _vertices.begin(); it != _vertices.end(); it++) {
                    laya::RectGeometry::VertexInfo& curvert = _fillVertices[i];
                    i++;
                    curvert.x = *it; it++;
                    curvert.y = *it;
                    curvert.u = fabsf(curvert.x - _minX) / _width;
                    curvert.v = fabsf(curvert.y - _minY) / _height;
                    curvert.r = curvert.g = curvert.b = curvert.a = 1.0f;
                }

                // setup the paints linear gradient
                _fillPaintForPath->buildGradientImage(_width, _height);

            }

            _tessVertices.clear();
        }

        /// build stroke vbo 
        if (_strokeVertices.size() > 0) {
            //x,y, _strokeVertices.size()
            _numberStrokeVertices = (int)_strokeVertices.size();
        }

        //TODO Batch

        // clear out vertex buffer
        _vertices.clear();
    }

    static GLdouble startVertex_[2];
    static GLdouble lastVertex_[2];
    static int vertexCount_ = 0;

    //开始新的三角形组，可能是strip，fan或者三角形列表。这个可能会被调用多次。
    void layaPath::tessBegin(GLenum type, GLvoid* user) {
        layaPath* me = (layaPath*)user;
        me->setPrimType(type);
        vertexCount_ = 0;

        switch (type)
        {
        case GL_TRIANGLES:
            //printf( "begin(GL_TRIANGLES)\n" );
            break;
        case GL_TRIANGLE_FAN:
            //printf( "begin(GL_TRIANGLE_FAN)\n" );
            break;
        case GL_TRIANGLE_STRIP:
            //printf( "begin(GL_TRIANGLE_STRIP)\n" );
            break;
        case GL_LINE_LOOP:
            //printf( "begin(GL_LINE_LOOP)\n" );
            break;
        default:
            break;
        }

    }


    void layaPath::tessEnd(GLvoid* user) {
        //		OpenGLPath* me = (OpenGLPath*)user;
        //		me->endOfTesselation();

        //printf("end\n");
    }

    /**
    *  @brief  tess引擎增加顶点的时候的回调。这里会记录下来用作渲染。
    *          为了简单，最后都用 GL_TRIANGLES 来渲染，所以需要把triangleFan和triangleStrips展开。
    *  @param[in] vertex float[2]表示的顶点坐标。
    *  @param[in] user 用户自定义数据，这里是 this
    *  @return
    */
    void layaPath::tessVertex(GLvoid* vertex, GLvoid* user) {
        layaPath* me = (layaPath*)user;
        GLdouble* v = (GLdouble*)vertex;

        if (me->primType() == GL_TRIANGLE_FAN) {
            // break up fans and strips into triangles
            switch (vertexCount_) {
            case 0:
                startVertex_[0] = v[0];
                startVertex_[1] = v[1];
                break;
            case 1:
                lastVertex_[0] = v[0];
                lastVertex_[1] = v[1];
                break;

            default:
                me->addVertex(startVertex_);
                me->addVertex(lastVertex_);
                me->addVertex(v);
                lastVertex_[0] = v[0];
                lastVertex_[1] = v[1];
                break;
            }
        }
        else if (me->primType() == GL_TRIANGLES) {
            me->addVertex(v);
        }
        else if (me->primType() == GL_TRIANGLE_STRIP) {
            switch (vertexCount_) {
            case 0:
                me->addVertex(v);
                break;
            case 1:
                startVertex_[0] = v[0];
                startVertex_[1] = v[1];
                me->addVertex(v);
                break;
            case 2:
                lastVertex_[0] = v[0];
                lastVertex_[1] = v[1];
                me->addVertex(v);
                break;

            default:
                me->addVertex(startVertex_);
                me->addVertex(lastVertex_);
                me->addVertex(v);
                startVertex_[0] = lastVertex_[0];
                startVertex_[1] = lastVertex_[1];
                lastVertex_[0] = v[0];
                lastVertex_[1] = v[1];
                break;
            }
        }
        vertexCount_++;

        //printf("\tvert[%d]: %f, %f, %f\n", vertexCount_, v[0], v[1], v[2] );
    }
    void layaPath::tessCombine(GLdouble coords[3], void *data[4],
        GLfloat weight[4], void **outData,
        void *polygonData) {

        layaPath* me = (layaPath*)polygonData;
        *outData = me->addTessVertex(coords);

    }

    void layaPath::tessError(GLenum errorCode) {
        printf("tesselator error: [%d] %s\n", errorCode, gluErrorString(errorCode));
    }
}