#include "trapezoid.h"
Trapezoid::Trapezoid(float a, float b, float c, float d)
{
	points.resize(4);

	points[0] = Point(a, 0);
	points[1] = Point(b, 1);
	points[2] = Point(c, 1);
	points[3] = Point(d, 0);

	bonds.resize(3);
	bonds[0] = Bond(&points[0], &points[1]);
	bonds[1] = Bond(&points[1], &points[2]);
	bonds[2] = Bond(&points[2], &points[3]);
}

Point* Trapezoid::getPoint(int index)
{
	return &points[index];
}

Bond* Trapezoid::getBond(int index)
{
	return &bonds[index];
}

float Trapezoid::getY(float x)
{
	for (int i = 0; i < 3; i++) 
	{
		if (bonds[i].isInRangeX(x)) 
		{
			return bonds[i].getY(x); 
		}
	}

	return 0;
}

void Trapezoid::clipTrapezoid(float y)
{
	bonds[0].clipByY(y, true);
	bonds[2].clipByY(y, true);

	bonds[1].updateEquation();
}

bool Trapezoid::isIn(Point& p)
{
	for(int i = 0; i < 3; i++) 
	{
		if(bonds[i].isInRangeX(p.x)) { if(p.y < bonds[i].getY(p.x)) { return true; } }
	}

	return false;
}

bool Trapezoid::isInDefinitionRange(Point& p)
{
	return p.x >= points[0].x && p.x <= points[3].x;
}
