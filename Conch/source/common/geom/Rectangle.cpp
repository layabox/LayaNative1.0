#include <stdio.h>
#include <algorithm>
#include "Rectangle.h"
#include "Point.h"
#include "Matrix32.h"
#include "RectGeometry.h"

namespace laya
{
    RectGeometry s_pTempGeo;
	Rectangle::Rectangle(float x,float y,float width,float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	
	Rectangle::~Rectangle()
	{

	}
	void Rectangle::intersection(Rectangle* resaultRect,Rectangle* srcRect,Rectangle* toIntersect)
	{
		float minr = std::min( srcRect->x + srcRect->width, toIntersect->getRight());
		float minb = std::min( srcRect->y + srcRect->height, toIntersect->getBottom());			
		float maxx = std::max( srcRect->x, toIntersect->x);
		float maxy = std::max( srcRect->y, toIntersect->y);
		resaultRect->x = maxx;
		resaultRect->y = maxy;
		resaultRect->width = minr-maxx;
		resaultRect->height = minb-maxy;
	}
	float Rectangle::getBottom ()
	{
		return height+y;
	}

	void Rectangle::setBottom(float value)
	{
		height = value-y;
	}

	void Rectangle::setBottomRight(Point* value)
	{
		setBottom( value->x );
		setRight( value->y );
	}

	float Rectangle::getLeft()
	{
		return x;
	}

	void Rectangle::setLeft(float value)
	{
		x = value;
	}

	float Rectangle::getRight()
	{
		return x+width;
	}

	void Rectangle::setRight(float value)
	{
		width = value-x;
	}

	void Rectangle::setSize(Point* value)
	{
		width = value->x;
		height = value->y;			
	}

	float Rectangle::getTop()
	{
		return y;
	}

	void Rectangle::setTop(float value)
	{
		y = value;
	}

	void Rectangle::setTopLeft(Point* value)
	{
		y = value->x;
		x = value->y;
	}

	Rectangle* Rectangle::clone()
	{
		return new Rectangle(x,y,width,height);
	}

	bool Rectangle::contains(float _x,float _y)
	{
		return _x>x && _x<(width+x) && _y>y && _y<(y+height);
	}

	bool Rectangle::containsPoint(Point* point)
	{
		return contains(point->x, point->y);
	}

	bool Rectangle::containsRect(Rectangle* rect)
	{
		return contains(rect->x, rect->y) && contains(rect->x + rect->width, rect->y + rect->height);
	}

	void Rectangle::copy(Rectangle* p_pOther)
	{
		x = p_pOther->x;
		y = p_pOther->y;
		width = p_pOther->width;
		height = p_pOther->height;
	}
	void Rectangle::copyFrom(Rectangle* sourceRect)
	{
		x = sourceRect->x;
		y = sourceRect->y;
		width = sourceRect->width;
		height = sourceRect->height;
	}

	bool Rectangle::equals(Rectangle* toCompare)
	{
		return x == toCompare->x && y == toCompare->y && width == toCompare->width && height == toCompare->height;
	}

	void Rectangle::inflate(float dx,float dy)
	{
		x += dx;
		y += dy;
		width += dx;
		height += dy;
	}

	void Rectangle::inflatePoint(Point* point)
	{
		inflate(point->x, point->y);
	}

	Rectangle* Rectangle::intersection(Rectangle* toIntersect)
	{
		float minr = std::min(x + width, toIntersect->getRight());
		float minb = std::min(y + height, toIntersect->getBottom());		
		float maxx = std::max(x, toIntersect->x);
		float maxy = std::max(y, toIntersect->y);
		return new Rectangle(maxx,maxy,minr-maxx,minb-maxy);
	}

	void Rectangle::intersectionThis(Rectangle* toIntersect)
	{			
		float minr = std::min(x + width, toIntersect->getRight());
		float minb = std::min(y + height, toIntersect->getBottom());
		x = std::max(x, toIntersect->x);
		y = std::max(y, toIntersect->y);
		width = minr - x;
		height = minb - y;
	}

	bool Rectangle::intersects(Rectangle* toIntersect)
	{
		float minr = std::min(x + width, toIntersect->getRight());
		float minb = std::min(y + height, toIntersect->getBottom());
		float maxx = std::max(x, toIntersect->x);
		float maxy = std::max(y, toIntersect->y);
        /****************LayaAir中Width是每帧都需要获取*************************/
        if (toIntersect->width == 0 || toIntersect->height == 0)
        {
            return toIntersect->x<(x + width) && toIntersect->y < (y + height);
        }
        /*********************************************************************/
		return minr > maxx && minb > maxy;
	}

	bool Rectangle::isEmpty()
	{
		return width<1 || height<1;
	}

	void Rectangle::offset(float dx,float dy)
	{
		this->x += dx;
		this->y += dy;
	}

	void Rectangle::offsetPoint(Point* point)
	{
		this->x += point->x;
		this->y += point->y;
	}

	void Rectangle::setEmpty()
	{
		width = height = 0;
	}

	void Rectangle::setTo(float xa,float ya,float widtha,float heighta)
	{
		this->x = xa;
		this->y = ya;
		this->width = widtha;
		this->height = heighta;
	}

	//std::string toString()
	//{
	//	// return "(x=" + this.x + ", y=" + this.y + ", w=" + this.width + ", h=" + this.height + ")";
	//	return "";
	//}

	Rectangle* Rectangle::unionRectangle(Rectangle* toUnion)
	{
		float maxr = std::max(x + width, toUnion->getRight());
		float maxb = std::max(y + height, toUnion->getBottom());
		float minx = std::min(x, toUnion->x);
		float miny = std::min(y, toUnion->y);
		return new Rectangle(minx,miny,maxr-minx,maxb-miny);
	}
    //TODO 效率不好
    void Rectangle::applyMatrix( Matrix32* p_pMatrix)
    {
        if (p_pMatrix->bOnlyTrans)
        {
            x += p_pMatrix->m[4];
            y += p_pMatrix->m[5];
        }
        else if (p_pMatrix->bHasRotate)
        {
            RectGeometry* pGeo1 = &s_pTempGeo;
            pGeo1->createForClip(x, y, width, height);
            pGeo1->applyMatrix(p_pMatrix->m[0], p_pMatrix->m[1], p_pMatrix->m[2], p_pMatrix->m[3], p_pMatrix->m[4], p_pMatrix->m[5]);
            float minx = std::min(std::min(std::min(pGeo1->m_pVertexRect[0].x, pGeo1->m_pVertexRect[1].x), pGeo1->m_pVertexRect[2].x), pGeo1->m_pVertexRect[3].x);
            float maxx = std::max(std::max(std::max(pGeo1->m_pVertexRect[0].x, pGeo1->m_pVertexRect[1].x), pGeo1->m_pVertexRect[2].x), pGeo1->m_pVertexRect[3].x);
            float miny = std::min(std::min(std::min(pGeo1->m_pVertexRect[0].y, pGeo1->m_pVertexRect[1].y), pGeo1->m_pVertexRect[2].y), pGeo1->m_pVertexRect[3].y);
            float maxy = std::max(std::max(std::max(pGeo1->m_pVertexRect[0].y, pGeo1->m_pVertexRect[1].y), pGeo1->m_pVertexRect[2].y), pGeo1->m_pVertexRect[3].y);
            x = minx;
            y = miny;
            width = maxx - minx;
            height = maxy - miny;
        }
        else
        {
            RectGeometry* pGeo = &s_pTempGeo;
            pGeo->createForClip(x, y, width, height);
            pGeo->applyMatrix(p_pMatrix->m[0], p_pMatrix->m[1], p_pMatrix->m[2], p_pMatrix->m[3], p_pMatrix->m[4], p_pMatrix->m[5]);
            pGeo->Normalize();
            x = pGeo->m_nX;
            y = pGeo->m_nY;
            width = pGeo->m_nWidth;
            height = pGeo->m_nHeight;
        }
    }
}
