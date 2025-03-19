#pragma once
#include "Shape.h"

namespace geometry
{
	class Rectangle : public Shape
	{
	private:
		double width = 0, height = 0;

	public:
		Rectangle() {}
		Rectangle(Point position, double width, double height)
			: Shape(position), width(width), height(height)
		{}

		// Getters
		double Width() { return width; }
		double Height() { return height; }

		double Left() { return position.X() - width / 2; }
		double Right() { return position.X() + width / 2; }
		double Down() { return position.Y() - height / 2; }
		double Up() { return position.Y() + height / 2; }
	};
} // geometry