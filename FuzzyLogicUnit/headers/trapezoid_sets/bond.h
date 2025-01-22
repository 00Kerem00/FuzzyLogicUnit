#pragma once
#include "point.h"

class Bond 
{
public:
	Point* p1;
	Point* p2;

	float slope;
	float offset;
	float minX;
	float maxX;

	Bond() : p1(nullptr), p2(nullptr), slope(0), offset(0), minX(0), maxX(0) {}
	Bond(Point* p1, Point* p2);

	float getX(float y);
	float getY(float x);
	bool isInRangeX(float x);
	bool isInRangeY(float y);

	Point* getIntersection(Bond* other);				// if there is no intersection, returns nullptr.
	Point getIntersectionOfEquations(Bond* other);
	void updateEquation();
	void clipByY(float y, bool fromTopSide);
};