#pragma once
#include "Actor.h"
#include "Slope.h"

#include "geometry/Collisions.h"

namespace object_collisions
{
	inline bool ActorToSlope(Actor& actor, Slope& slope)
	{
		Rectangle* rectangle = actor.getRectangle();
		Segment* segment = slope.getSegment();

		// Check collision from below
		Line slope_line = Line(*slope.getSegment());
		double foot_x = slope.isRightSlope() ? actor.getRectangle()->Right() : actor.getRectangle()->Left();

		if (actor.getRectangle()->PreviousPosition().Y() - actor.getRectangle()->Height() / 2 <
			slope.heightAt(foot_x))
			return false;

		if (slope.isHorisontal())
		{
			Point left_start = Point(rectangle->Left(), rectangle->Up());
			Point right_start = Point(rectangle->Right(), rectangle->Up());
			Point vertical_vector = Point(0, -rectangle->Height());

			Segment left_segment = Segment(left_start, vertical_vector);
			Segment right_segment = Segment(right_start, vertical_vector);

			bool collision = collisions::SegmentToSegment(left_segment, *segment);
			collision |= collisions::SegmentToSegment(right_segment, *segment);
			return collision;
		}

		Point bottom_start = Point(rectangle->Left(), rectangle->Down());
		Point bottom_vector = Point(rectangle->Width(), 0);

		Segment bottom = Segment(bottom_start, bottom_vector);

		return collisions::SegmentToSegment(bottom, *segment);
	}

	namespace contact
	{
		inline Point ActorToSlope(Actor& actor, Slope& slope)
		{
			// Evaluate collision
			if (!object_collisions::ActorToSlope(actor, slope))
				return Point(NAN, NAN);

			// Move actor
			if (actor.getRectangle()->Down() < slope.getSegment()->UpperPoint().Y())
				actor.GetShape()->MoveTo(actor.GetShape()->PreviousPosition());

			// Calculate collision point
			double actor_feet_y = actor.getRectangle()->Down();

			Point slope_higher_point =	slope.getSegment()->UpperPoint();

			if (actor_feet_y >= slope_higher_point.Y())
				return slope_higher_point;

			double actor_feet_x =	slope.isRightSlope() ?
									actor.getRectangle()->Left() :
									actor.getRectangle()->Right();

			return Point(actor_feet_x, actor_feet_y);
		}
	} // contact
} // object_collisions
