#pragma once
#include <numbers>
#include "Shape.h"
#include "Geometry.h"

namespace geometry
{
	class CircularSector : public Shape
	{
	private:
		double radious = 0;
		double beginning = 0;
		double end = 1;

	public:
		CircularSector() {}
		CircularSector(Point position, double radious, double beginning, double end)
			: Shape(position), radious(radious), beginning(beginning), end(end)
		{}

		// Getters
		double Radious() { return radious; }
		Point SectorBeginVector()
		{
			return AngleToPoint(beginning, radious);
		}
		Point SectorBegin()
		{
			return SectorBeginVector() + position;
		}
		Point SectorEndVector()
		{
			return AngleToPoint(end, radious);
		}
		Point SectorEnd()
		{
			return SectorEndVector() + position;
		}

		bool IsInside(Point p)
		{
			if (Distance(position, p) >= radious)
				return false;

			double angle = PointToAngle(p - position);

			if (beginning > end)
				return angle > beginning || angle < end;

			return angle > beginning && angle < end;
		}
	};
} // geometry