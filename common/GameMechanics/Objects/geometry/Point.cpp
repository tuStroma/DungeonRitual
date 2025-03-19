#include "Point.h"

namespace geometry
{
	Point::Point(double x, double y)
		:x(x), y(y)
	{}

	void Point::Move(Point p)
	{
		this->x += p.X();
		this->y += p.Y();
	}

	bool Point::operator==(const Point p)
	{
		return this->x == p.x && this->y == p.y;
	}
	Point Point::operator*(const double& m)
	{
		return Point(this->x * m, this->y * m);
	}
	Point& Point::operator*=(const double& m)
	{
		this->x *= m;
		this->y *= m;
		return *this;
	}
	Point Point::operator+(const Point& p)
	{
		return Point(this->x + p.x, this->y + p.y);
	}
	Point& Point::operator+=(const Point& p)
	{
		this->x += p.x;
		this->y += p.y;
		return *this;
	}
	Point Point::operator-(const Point& p)
	{
		return Point(this->x - p.x, this->y - p.y);
	}
	Point& Point::operator-=(const Point& p)
	{
		this->x -= p.x;
		this->y -= p.y;
		return *this;
	}
}
