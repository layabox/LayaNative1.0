
#ifndef _LAYA_GEOM_POINT_H__
#define _LAYA_GEOM_POINT_H__

#include <string>

namespace laya
{
	class Point 
	{
	public:
		//测量两点之间的距离
		static float distance(Point* pt1, Point* pt2);
		static Point* interpolate(Point* pt1,Point* pt2,float f);
		static Point* polar(float len,float angle);

	public:
		Point (float x = 0,float y = 0);
		~Point();
		float getLength();
		Point* add(Point* v);
		Point* clone();
		void copyFrom(Point* sourcePoint);
		bool equals(Point* toCompare);
		void normalize(float thickness);
		void offset(float dx,float dy);
		
		void setTo(float xa,float ya);
		Point* subtract(Point* v);
		std::string toString ();

	public:
		float x;			
		float y;
	};
}

#endif