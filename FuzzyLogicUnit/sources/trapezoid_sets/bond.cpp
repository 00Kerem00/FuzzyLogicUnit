#include "bond.h"

Bond::Bond(Point* p1, Point* p2)
{
	if (p1->x <= p2->x) 
	{
		this->p1 = p1;
		this->p2 = p2;
	}
	else
	{
		this->p1 = p2;
		this->p2 = p1;
	}

	updateEquation();
}

float Bond::getX(float y)
{
	if (slope == 0) { return p2->x; }
	return (y - offset) / slope;
}
float Bond::getY(float x)
{
	return slope * x + offset;
}

bool Bond::isInRangeX(float x)
{
	return x >= minX && x <= maxX;
}
bool Bond::isInRangeY(float y)
{
	return isInRangeX(getX(y));
}

Point* Bond::getIntersection(Bond* other)
{
	float x = (other->offset - offset) / (slope - other->slope);
	float y = getY(x);

	if (x != x || x < minX || x < other->minX || x > maxX || x > other->maxX) { return nullptr; }
	return new Point(x, y);
}
Point Bond::getIntersectionOfEquations(Bond* other)
{
	float x = (offset - other->offset) / (slope - other->slope);
	float y = getY(x);

	return Point(x, y);
}

void Bond::updateEquation()
{
	minX = p1->x;
	maxX = p2->x;

	if (minX == maxX) { slope = 0; offset = p1->y; return; }

	slope = (p2->y - p1->y) / (maxX - minX);
	offset = p1->y - slope * p1->x;

	int a = 0;
}
void Bond::clipByY(float y, bool fromTopSide)
{
	Point newPoint = Point(getX(y), y);
	if (fromTopSide)
	{
		if (p1->y < p2->y)
		{
			*p2 = newPoint;
		}
		else
		{
			*p1 = newPoint;
		}
	}
	else 
	{
		if (p1->y < p2->y)
		{
			*p1 = newPoint;
		}
		else
		{
			*p2 = newPoint;
		}
	}

	updateEquation();
}