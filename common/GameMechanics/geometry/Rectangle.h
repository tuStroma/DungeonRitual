#pragma once
#include "Shape.h"
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
};