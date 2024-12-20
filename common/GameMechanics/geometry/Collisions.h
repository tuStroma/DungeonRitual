#pragma once
#include "Geometry.h"

#include "Rectangle.h"
#include "Segment.h"
#include "Circle.h"
#include "CircularSector.h"
#include "Figures.h"

// Detect and resolve collisions of 2 Shapes
namespace geometry
{
	namespace collisions
	{
		namespace helpers
		{
			inline bool CollinearSegmentsOverlap(Segment& s1, Segment& s2)
			{
				Point a_start = s1.Position();
				Point a_end = s1.EndPoint();
				double a_left = fmin(a_start.X(), a_end.X());
				double a_right = fmax(a_start.X(), a_end.X());
				double a_down = fmin(a_start.Y(), a_end.Y());
				double a_up = fmax(a_start.Y(), a_end.Y());

				Point b_start = s1.Position();
				Point b_end = s1.EndPoint();
				double b_left = fmin(b_start.X(), b_end.X());
				double b_right = fmax(b_start.X(), b_end.X());
				double b_down = fmin(b_start.Y(), b_end.Y());
				double b_up = fmax(b_start.Y(), b_end.Y());

				if (a_left > b_right || a_right < b_left ||
					a_down > b_up || a_up < b_down)
					return false;

				return true;
			}
		}

		inline bool RectangleToRectangle(Rectangle& r1, Rectangle& r2)
		{
			double r1_x0 = r1.Position().X() - r1.Width() / 2;
			double r1_x1 = r1.Position().X() + r1.Width() / 2;
			double r1_y0 = r1.Position().Y() - r1.Height() / 2;
			double r1_y1 = r1.Position().Y() + r1.Height() / 2;

			double r2_x0 = r2.Position().X() - r2.Width() / 2;
			double r2_x1 = r2.Position().X() + r2.Width() / 2;
			double r2_y0 = r2.Position().Y() - r2.Height() / 2;
			double r2_y1 = r2.Position().Y() + r2.Height() / 2;

			return !(r1_x0 >= r2_x1 || r1_x1 <= r2_x0 || r1_y0 >= r2_y1 || r1_y1 <= r2_y0);
		}

		inline bool SegmentToSegment(Segment& s1, Segment& s2)
		{
			Line a = Line(s1);
			Line b = Line(s2);

			Point intersection = a.Intersect(b);

			if (isnan(intersection.X()))
				return false;

			//if (intersection == s1.Position() || intersection == s1.EndPoint() ||
			//	intersection == s2.Position() || intersection == s2.EndPoint())
			//	return false;

			if (isinf(intersection.X()))
				return helpers::CollinearSegmentsOverlap(s1, s2);

			return s1.Contains(intersection) && s2.Contains(intersection);
		}

		inline bool CircleToCircle(Circle& c1, Circle& c2)
		{
			double distance = Distance(c1.Position(), c2.Position());
			double collision_distance = c1.Radious() + c2.Radious();

			return distance < collision_distance;
		}

		inline bool PointToCircle(Point& p, Circle& c)
		{
			double distance = Distance(p, c.Position());

			return distance < c.Radious();
		}

		inline bool SectorToSegment(CircularSector& cs, Segment& s)
		{
			Segment sector_begin(cs.Position(), cs.SectorBeginVector());
			Segment sector_end(cs.Position(), cs.SectorEndVector());

			if (SegmentToSegment(sector_begin, s) || SegmentToSegment(sector_end, s))
				return true;

			// If closest point inside sector return true
			Point closest = ClosestPoint(cs.Position(), s);
			
			return cs.IsInside(closest);
		}

		inline bool SectorToRectangle(CircularSector& cs, Rectangle& r)
		{
			Point left_down = Point(r.Left(), r.Down());
			Point right_up = Point(r.Right(), r.Up());

			Segment left(left_down, Point(0, r.Height()));
			Segment down(left_down, Point(r.Width(), 0));
			Segment right(right_up, Point(0, -r.Height()));
			Segment up(right_up, Point(-r.Width(), 0));

			return	SectorToSegment(cs, left) ||
					SectorToSegment(cs, right) ||
					SectorToSegment(cs, up) ||
					SectorToSegment(cs, down);
		}

		// Resolve collision to touch two shapes.
		// The return value is a point lying on the contact surface.
		namespace contact
		{
			namespace
			{
				// Return time of collision
				double PointToPoint(double a, double b, double a_v)
				{
					if (a == b)
						return 0;

					if (a_v == 0)
						return INFINITY;

					return (b - a) / a_v;
				}

				double RangeToRange(double a0, double a1, double b0, double b1, double a_v)
				{
					// If ranges are intersecting from the beggining return 0 time
					if (!(a0 >= b1 || a1 <= b0))
						return 0;

					// Check direction of vector on point collision
					double left_collision = PointToPoint(a1, b0, a_v);
					left_collision = left_collision == 0 && a_v <= 0 ? INFINITY : left_collision;

					double right_collision = PointToPoint(a0, b1, a_v);
					right_collision = right_collision == 0 && a_v >= 0 ? INFINITY : right_collision;

					return fmin(left_collision, right_collision);
				}

				Point RectangleToRectangleConnection(Rectangle& r1, Rectangle& r2)
				{
					double r1_x0 = r1.Position().X() - r1.Width() / 2;
					double r1_x1 = r1.Position().X() + r1.Width() / 2;
					double r1_y0 = r1.Position().Y() - r1.Height() / 2;
					double r1_y1 = r1.Position().Y() + r1.Height() / 2;

					double r2_x0 = r2.Position().X() - r2.Width() / 2;
					double r2_x1 = r2.Position().X() + r2.Width() / 2;
					double r2_y0 = r2.Position().Y() - r2.Height() / 2;
					double r2_y1 = r2.Position().Y() + r2.Height() / 2;

					double left = fmax(r1_x0, r2_x0);
					double right = fmin(r1_x1, r2_x1);
					double down = fmax(r1_y0, r2_y0);
					double up = fmin(r1_y1, r2_y1);

					double connection_x = (left + right) / 2;
					double connection_y = (down + up) / 2;

					return Point(connection_x, connection_y);
				}
			}

			inline Point RectangleToRectangle(Rectangle& r1, Rectangle& r2)
			{
				// Return NAN contact point if there is no collision
				if (!collisions::RectangleToRectangle(r1, r2))
					return Point(NAN, NAN);


				double r1_x0 = r1.PreviousPosition().X() - r1.Width() / 2;
				double r1_x1 = r1.PreviousPosition().X() + r1.Width() / 2;
				double r1_y0 = r1.PreviousPosition().Y() - r1.Height() / 2;
				double r1_y1 = r1.PreviousPosition().Y() + r1.Height() / 2;

				double r2_x0 = r2.PreviousPosition().X() - r2.Width() / 2;
				double r2_x1 = r2.PreviousPosition().X() + r2.Width() / 2;
				double r2_y0 = r2.PreviousPosition().Y() - r2.Height() / 2;
				double r2_y1 = r2.PreviousPosition().Y() + r2.Height() / 2;

				// Calculate relative velocity on X and Y
				double r1_x_v = r1.Position().X() - r1.PreviousPosition().X();
				double r2_x_v = r2.Position().X() - r2.PreviousPosition().X();
				double x_dv = r1_x_v - r2_x_v;

				double r1_y_v = r1.Position().Y() - r1.PreviousPosition().Y();
				double r2_y_v = r2.Position().Y() - r2.PreviousPosition().Y();
				double y_dv = r1_y_v - r2_y_v;

				// Calculate collision times on X and Y ranges
				double x_collision_time = RangeToRange(r1_x0, r1_x1, r2_x0, r2_x1, x_dv);
				double y_collision_time = RangeToRange(r1_y0, r1_y1, r2_y0, r2_y1, y_dv);

				double collision_time = fmax(x_collision_time, y_collision_time);

				// Move objects to calculated positions
				// Slide on the surface
				double new_r1_x = r1.Position().X();
				double new_r1_y = r1.Position().Y();
				double new_r2_x = r2.Position().X();
				double new_r2_y = r2.Position().Y();

				// Connect shapes if not intersected in previous frame 
				if (r1_x0 >= r2_x1 || r1_x1 <= r2_x0)
				{
					new_r1_x = Round(r1.PreviousPosition().X() + collision_time * r1_x_v, 0.001);
					new_r2_x = Round(r2.PreviousPosition().X() + collision_time * r2_x_v, 0.001);
				}

				if (r1_y0 >= r2_y1 || r1_y1 <= r2_y0)
				{
					new_r1_y = Round(r1.PreviousPosition().Y() + collision_time * r1_y_v, 0.001);
					new_r2_y = Round(r2.PreviousPosition().Y() + collision_time * r2_y_v, 0.001);
				}

				r1.MoveTo(Point(new_r1_x, new_r1_y));

				r2.MoveTo(Point(new_r2_x, new_r2_y));

				// Calculate connection point
				return RectangleToRectangleConnection(r1, r2);
			}
		} // contact
	} // collisions
} // geometry
