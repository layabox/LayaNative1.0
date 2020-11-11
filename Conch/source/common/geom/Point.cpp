#include "Point.h"
#include <math.h>

namespace laya{
	//测量两点之间的距离
	float Point::distance(Point* pt1,Point* pt2)
	{
		return sqrt(pow((pt2->x - pt1->x), 2) + pow((pt2->y - pt1->y), 2));
	}

	Point* Point::polar(float len,float angle)
	{
		Point* result= new Point();
		result->x = len * cos(angle);
		result->y = len * sin(angle);
		return result;
	}

	Point* Point::interpolate(Point* pt1,Point* pt2,float f)
	{
		Point* result = new Point();
		result->x = (pt1->x + pt2->x) * f;
		result->y = (pt1->y + pt2->y) * f;
		return result;
	}

	Point::Point(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Point::~Point()
	{

	}

	float Point::getLength ()
	{ 
		return sqrtf(powf(this->x ,2) + powf(this->y, 2));
	}

	Point* Point::add (Point* v)
	{
		Point* result = new Point();
		result->x = this->x + v->x;
		result->y = this->y + v->y;
		return result;
	}

	Point* Point::clone ()
	{
		Point* result = new Point();
		result->x = this->x;
		result->y = this->y;
		return result; 
	}

	void Point::copyFrom(Point* sourcePoint)
	{
		this->x = sourcePoint->x;
		this->y = sourcePoint->y;
	}

	bool Point::equals(Point* toCompare)
	{
		if (toCompare == NULL)
			return false;

		if (toCompare->x == this->x && toCompare->y == this->y)
			return true;

		return false;
	}

	void Point::normalize (float thickness)
	{
		float rate = thickness / this->getLength();
		this->x = rate * this->x;
		this->y = rate * this->y;
	}


	void Point::offset(float dx,float dy)
	{
		this->x += dx;
		this->y += dy;
	}

	void Point::setTo(float xa,float ya)
	{
		this->x = xa;
		this->y = ya;
	}

	Point* Point::subtract (Point* v)
	{
		Point* result = new Point();
		result->x = this->x - v->x;
		result->y = this->y - v->y;
		return result;
	}

	std::string toString ()
	{
		//return "(x=" + this->x + ", y=" + this->y + ")";
		return "";
	}
}
