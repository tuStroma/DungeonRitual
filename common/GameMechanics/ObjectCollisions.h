#pragma once
#include "Actor.h"
#include "Slope.h"

#include "geometry/Collisions.h"

namespace object_collisions
{
	namespace helpers
	{
		inline bool ActorWasBelowSlope(Actor& actor, Slope& slope)
		{
			Line slope_line = Line(*slope.getSegment());
			double foot_x = actor.GetShape()->PreviousPosition().X();
			double actor_half_width = actor.getRectangle()->Width() / 2;
			foot_x += slope.isRightSlope() ? actor_half_width : -actor_half_width;

			double foot_y = actor.getRectangle()->PreviousPosition().Y() - actor.getRectangle()->Height() / 2;

			return foot_y < slope.heightAt(foot_x);
		}
	} // helpers

	inline bool ActorToSlope(Actor& actor, Slope& slope)
	{
		if (actor.isAvoidingSlopes() && slope.isPenetrable())
			return false;

		Rectangle* rectangle = actor.getRectangle();
		Segment* segment = slope.getSegment();

		// Check collision from below
		if (helpers::ActorWasBelowSlope(actor, slope))
			return false;

		// Horisontal collision
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
