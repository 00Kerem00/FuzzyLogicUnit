#pragma once
#include "trapezoid.h"

class TrapezoidTemplate
{
public:
	float a;
	float b;
	float c;
	float d;

	TrapezoidTemplate() : a(0), b(0), c(0), d(0) {}
	TrapezoidTemplate(float a, float b, float c, float d) : a(a), b(b), c(c), d(d){}
	bool isDiscrete(TrapezoidTemplate& tt) { return (a > tt.a && a > tt.d) || (d < tt.d && d < tt.a); }
	Trapezoid createInstance();
};

class ConstantTriangle : TrapezoidTemplate{};