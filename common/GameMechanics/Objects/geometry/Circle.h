#pragma once
#include "Shape.h"

namespace geometry
{
	class Circle : public Shape
	{
	private:
		double radious = 0;

	public:
		Circle() {}
		Circle(Point position, double radious)
			: Shape(position), radious(radious)
		{}

		// Getters
		double Radious() { return radious; }
	};
} // geometry