#pragma once
#include "bond.h"
#include <vector>

class Trapezoid
{
private:
	std::vector<Point> points;
	std::vector<Bond> bonds;

public:
	Trapezoid() {}
	Trapezoid(float a, float b, float c, float d);

	Point* getPoint(int index);
	Bond* getBond(int index);

	float getY(float x);
	void clipTrapezoid(float y);
	bool isIn(Point& p);
	bool isInDefinitionRange(Point& p);
};

class Triangle : public Trapezoid
{
	Triangle(float a, float b, float c) : Trapezoid(a, b, b, c){}
};