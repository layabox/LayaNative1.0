
#ifndef _LAYA_GEOM_RECTANGLE_H__
#define _LAYA_GEOM_RECTANGLE_H__

#include <stdio.h>
#include <string>

namespace laya{

	class Point;
    class Matrix32;
	class Rectangle 
	{
	public:
		Rectangle(float x = 0,float y = 0,float width = 0,float height = 0);
		virtual ~Rectangle();
		static void intersection(Rectangle* resaultRect,Rectangle* srcRect,Rectangle* toIntersect);
		void copy(Rectangle* p_pOther);
		float getBottom();
		void setBottom(float value);
		void setBottomRight(Point* value);
		float getLeft();
		void setLeft(float value);
		float getRight();
		void setRight(float value);
		void setSize(Point* value);
		float getTop();
		void setTop(float value);
		void setTopLeft(Point* value);
		Rectangle* clone();
		bool contains(float _x,float _y);
		bool containsPoint(Point* point);
		bool containsRect(Rectangle* rect);
		void copyFrom(Rectangle* sourceRect);
		bool equals(Rectangle* toCompare);
		void inflate(float dx,float dy);
		void inflatePoint(Point* point);
		Rectangle* intersection(Rectangle* toIntersect);
		void intersectionThis(Rectangle* toIntersect);
		bool intersects(Rectangle* toIntersect);
		bool isEmpty();
		void offset(float dx,float dy);
		void offsetPoint(Point* point);
		void setEmpty();
		void setTo(float xa,float ya,float widtha,float heighta);
		//std::string toString();
		Rectangle* unionRectangle(Rectangle* toUnion);
        //TODO效率不好
        void applyMatrix(Matrix32* p_pMatrix);
	public:
		float height;
		float width;
		float x;
		float y;
	};

}
#endif