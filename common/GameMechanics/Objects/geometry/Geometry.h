#pragma once
#include <cmath>
#include <numbers>

#include "Point.h"
#include "Figures.h"

namespace geometry
{
	inline double Distance(Point a, Point b)
	{
		return sqrt(pow(a.X() - b.X(), 2) + pow(a.Y() - b.Y(), 2));
	}

	inline double Round(double value, double precision = 1.0)
	{
		return std::round(value / precision) * precision;
	}

	// Angle is a number in range <0; 1>
	// measured from top position clockwise
	inline Point AngleToPoint(double angle, double radious)
	{
		double rad_angle = angle * 2 * std::numbers::pi;
		double x = sin(rad_angle) * radious;
		double y = cos(rad_angle) * radious;
		return Point(x, y);
	}
	inline double PointToAngle(Point p)
	{
		if (p.X() == 0 && p.Y() == 0)
			return 0;

		double angle = atan(p.X() / p.Y()) / (2 * std::numbers::pi);

		if (p.X() < 0 && p.Y() < 0)
			return angle + 0.5;

		return angle;
	}

	inline Point ClosestPoint(Point p, Line l)
	{
		Line perpendicular = l.Perpendicular(p);
		Point closest = l.Intersect(perpendicular);
		return closest;
	}

	inline Point ClosestPoint(Point p, Segment s)
	{
		Point closest = ClosestPoint(p, Line(s));
		if (!s.isVertical())
		{
			if (closest.X() >= s.LeftPoint().X() && closest.X() <= s.RightPoint().X())
				return closest;
			else if (closest.X() < s.LeftPoint().X())
				return s.LeftPoint();
			else
				return s.RightPoint();
		}
		
		if (closest.Y() >= s.LowerPoint().Y() && closest.Y() <= s.UpperPoint().Y())
			return closest;
		else if (closest.Y() < s.LowerPoint().Y())
			return s.LowerPoint();
		else
			return s.UpperPoint();
	}
} // geometry