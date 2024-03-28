#include "Point.h"

Point::Point(double x, double y)
	:x(x), y(y)
{}

void Point::Move(Point p)
{
	this->x += p.X();
	this->y += p.Y();
}

bool Point::operator==(const Point& p)
{
	return this->x == p.x && this->y == p.y;
}
