#pragma once

class Point
{
public:
	float x;
	float y;

	bool marked = false;

	Point() : x(0), y(0) {}
	Point(float x, float y) : x(x), y(y){}
};