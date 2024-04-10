#pragma once
#include "../Match.h"

namespace maps
{
	inline void BasicTest(Match* match)
	{
		match->addActor(new Actor(new Rectangle(Point(0, 0), 1, 2)), true);

		match->addObject(new GameObject(new Rectangle(Point(0, -7.0), 20, 1)));
		match->addObject(new GameObject(new Rectangle(Point(0, 7.0), 20, 1)));
		match->addObject(new GameObject(new Rectangle(Point(10.0, 0), 1, 14)));
		match->addObject(new GameObject(new Rectangle(Point(-10.0, 0), 1, 14)));
		match->addObject(new GameObject(new Rectangle(Point(0, -2.5), 4, 1)));


		match->addObject(new Slope(new Segment(Point(2, -2), Point(4.5, -4.5))));
		match->addObject(new Slope(new Segment(Point(-2, -2), Point(-4.5, -4.5))));
		match->addObject(new Slope(new Segment(Point(-2, 2), Point(4, 0))));
		//slopes[2] = Slope(new Segment(Point(-6.5, -6.5), Point(-3, 3)), false);
	}
} // maps
