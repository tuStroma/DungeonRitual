#pragma once
#include "Shape.h"
class Segment : public Shape
{
private:
	Point vector;

public:
	Segment() {}
	Segment(Point position, Point vector)
		: Shape(position), vector(vector)
	{}

	// Getters
	Point Vector() { return vector; }
	Point EndPoint()
	{
		return Point(position.X() + vector.X(), position.Y() + vector.Y());
	}

	Point LeftPoint()	{ return vector.X() > 0 ? Position() : EndPoint(); }
	Point RightPoint()	{ return vector.X() > 0 ? EndPoint() : Position(); }
	Point LowerPoint()	{ return vector.Y() > 0 ? Position() : EndPoint(); }
	Point UpperPoint()	{ return vector.Y() > 0 ? EndPoint() : Position(); }

	// Geometry
	bool Contains(Point p) // Assumes that p lies on the same line as segment
	{
		Point end = EndPoint();

		double left = fmin(position.X(), end.X());
		double right = fmax(position.X(), end.X());

		double down = fmin(position.Y(), end.Y());
		double up = fmax(position.Y(), end.Y());

		return p.X() >= left && p.X() <= right && p.Y() >= down && p.Y() <= up;
	}
};