#include "pch.h"
#include "../common/GameMechanics/geometry/Collisions.h"

namespace Collisions
{
	// Rectangle to Rectangle
	TEST(RectangleToRectangle, no_collision_on_no_intersection) {
			geometry::Rectangle r1(geometry::Point(1, 2), 2, 2);
			geometry::Rectangle r2(geometry::Point(4, 5), 2, 2);
			geometry::Rectangle r3(geometry::Point(1, 5), 2, 2);

			EXPECT_FALSE(geometry::collisions::RectangleToRectangle(r1, r2));
			EXPECT_FALSE(geometry::collisions::RectangleToRectangle(r1, r3));
			EXPECT_FALSE(geometry::collisions::RectangleToRectangle(r2, r3));
		}

	TEST(RectangleToRectangle, no_collision_on_corner_touch) {
			geometry::Rectangle r1(geometry::Point(0, 0), 2, 2);
			geometry::Rectangle r2(geometry::Point(2, 2), 2, 2);

			EXPECT_FALSE(geometry::collisions::RectangleToRectangle(r1, r2));
		}

	TEST(RectangleToRectangle, no_collision_on_side_touch) {
			geometry::Rectangle r1(geometry::Point(0, 0), 2, 2);
			geometry::Rectangle r2(geometry::Point(0, 2), 2, 2);
			geometry::Rectangle r3(geometry::Point(2, 0), 2, 2);

			EXPECT_FALSE(geometry::collisions::RectangleToRectangle(r1, r2));
			EXPECT_FALSE(geometry::collisions::RectangleToRectangle(r1, r3));
		}

	TEST(RectangleToRectangle, collision_on_intersection) {
			geometry::Rectangle r1(geometry::Point(0, 0), 2, 2);
			geometry::Rectangle r2(geometry::Point(1, 1), 2, 2);

			EXPECT_TRUE(geometry::collisions::RectangleToRectangle(r1, r2));
		}

	TEST(RectangleToRectangle, collision_on_full_overlap) {
			geometry::Rectangle r1(geometry::Point(0, 0), 2, 2);
			geometry::Rectangle r2(geometry::Point(0, 0), 1, 1);

			EXPECT_TRUE(geometry::collisions::RectangleToRectangle(r1, r2));
		}

	// Point to Circle
	TEST(PointToCircle, collision_on_point_inside_circle) {
		geometry::Circle c(geometry::Point(0, 0), 2);
		geometry::Point p(0.5, 0.5);

		EXPECT_TRUE(geometry::collisions::PointToCircle(p, c));
	}

	TEST(PointToCircle, no_collision_on_point_outside_circle) {
		geometry::Circle c(geometry::Point(0, 0), 2);
		geometry::Point p(2, 2);

		EXPECT_FALSE(geometry::collisions::PointToCircle(p, c));
	}

	TEST(PointToCircle, no_collision_on_point_on_circle_edge) {
		geometry::Circle c(geometry::Point(0, 0), 2);
		geometry::Point p(2, 0);

		EXPECT_FALSE(geometry::collisions::PointToCircle(p, c));
	}

	// Circle to Circle
	TEST(CircleToCircle, no_collision_on_no_overlap) {
		geometry::Circle c1(geometry::Point(0, 0), 2);
		geometry::Circle c2(geometry::Point(1, 4), 1);

		EXPECT_FALSE(geometry::collisions::CircleToCircle(c1, c2));
	}

	TEST(CircleToCircle, no_collision_on_touch) {
		geometry::Circle c1(geometry::Point(0, 0), 2);
		geometry::Circle c2(geometry::Point(3, 0), 1);

		EXPECT_FALSE(geometry::collisions::CircleToCircle(c1, c2));
	}

	TEST(CircleToCircle, collision_on_overlap) {
		geometry::Circle c1(geometry::Point(0, 0), 2);
		geometry::Circle c2(geometry::Point(2, 0), 1);

		EXPECT_TRUE(geometry::collisions::CircleToCircle(c1, c2));
	}

	TEST(CircleToCircle, collision_on_full_overlap) {
		geometry::Circle c1(geometry::Point(0, 0), 2);
		geometry::Circle c2(geometry::Point(0, 0), 1);

		EXPECT_TRUE(geometry::collisions::CircleToCircle(c1, c2));
	}
}

namespace Geometry
{
	// Circular Sector
	TEST(CircularSector, angle_to_point_1) 
	{
		geometry::CircularSector s(geometry::Point(0, 0), 2, 0, 1);

		EXPECT_EQ(geometry::collisions::contact::Round(s.SectorBegin().X(), 0.001), 0);
		EXPECT_EQ(geometry::collisions::contact::Round(s.SectorBegin().Y(), 0.001), 2);

		EXPECT_EQ(geometry::collisions::contact::Round(s.SectorEnd().X(), 0.001), 0);
		EXPECT_EQ(geometry::collisions::contact::Round(s.SectorEnd().Y(), 0.001), 2);
	}


	TEST(CircularSector, angle_to_point_2)
	{
		geometry::CircularSector s(geometry::Point(0, 0), 2, 0.125, 0.25);

		EXPECT_EQ(geometry::collisions::contact::Round(s.SectorBegin().X(), 0.001), round(sqrt(2) * 1000) / 1000);
		EXPECT_EQ(geometry::collisions::contact::Round(s.SectorBegin().Y(), 0.001), round(sqrt(2) * 1000) / 1000);

		EXPECT_EQ(geometry::collisions::contact::Round(s.SectorEnd().X(), 0.001), 2);
		EXPECT_EQ(geometry::collisions::contact::Round(s.SectorEnd().Y(), 0.001), 0);
	}
}
