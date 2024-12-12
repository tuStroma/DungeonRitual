#pragma once
#include <cmath>

namespace geometry
{
	class Point
	{
	private:
		double x = 0.0;
		double y = 0.0;

	public:
		Point() {}
		Point(double x, double y);

		// Getters
		double X() { return x; }
		double Y() { return y; }

		// Setters
		void X(double x) { this->x = x; }
		void Y(double y) { this->y = y; }

		void Set(Point p) { x = p.x; y = p.y; }
		void Move(Point p);

		bool operator==(const Point p);
		Point operator*(const double& m);
		Point& operator*=(const double& m);
		Point operator+(const Point& m);
		Point& operator+=(const Point& m);
		Point operator-(const Point& m);
		Point& operator-=(const Point& m);
	};
} // geometry
