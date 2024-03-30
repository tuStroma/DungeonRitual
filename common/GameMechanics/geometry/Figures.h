#pragma once
#include <cmath>

#include "Segment.h"

class Line
{
private:
	double a = 0;
	double b = 0;
	double c = 0;
public:
	Line(Segment& s)
	{
		if (s.Vector().X() == 0)
		{
			a = 1;
			b = 0;
			c = -s.Position().X();
		}
		else
		{
			a = -s.Vector().Y() / s.Vector().X();
			b = 1;
			c = -a * s.Position().X() - s.Position().Y();
		}
	}

	double A() { return a; }
	double B() { return b; }
	double C() { return c; }

	double Y(double x)
	{
		// Vertical line
		if (b == 0)
		{
			if (x == -c)
				return INFINITY;
			else
				return NAN;
		}

		return -a * x - c;
	}

	Point Intersect(Line& l)
	{
		// Parallel
		if (a == l.A() && b == l.B())
		{
			if (c == l.C())
				return Point(INFINITY, INFINITY);
			return Point(NAN, NAN);
		}

		// One is vertical
		if (b != l.B())
		{
			Line* vertical = b == 0 ? this : &l;
			Line* regular = b == 0 ? &l : this;
			return Point(-vertical->C(), regular->Y(-vertical->C()));
		}

		double x = (l.C() - c) / (a - l.A());
		double y = Y(x);
		return Point(x, y);
	}
};