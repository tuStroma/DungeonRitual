#pragma once
#pragma once
#include "Shape.h"
#include <numbers>

namespace geometry
{
	class CircularSector : public Shape
	{
	private:
		double radious = 0;
		double beginning = 0;
		double end = 1;

		Point AngleToPoint(double angle)
		{
			double rad_angle = angle * 2 * std::numbers::pi;
			double x = sin(rad_angle) * radious;
			double y = cos(rad_angle) * radious;
			return Point(x, y);
		}

	public:
		CircularSector() {}
		CircularSector(Point position, double radious, double beginning, double end)
			: Shape(position), radious(radious), beginning(beginning), end(end)
		{}

		// Getters
		double Radious() { return radious; }
		Point SectorBegin()
		{
			return AngleToPoint(beginning);
		}
		Point SectorEnd()
		{
			return AngleToPoint(end);
		}
	};
} // geometry