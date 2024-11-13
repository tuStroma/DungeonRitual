#pragma once
#include "Actor/Actor.h"
#include "Slope.h"

#include "geometry/Collisions.h"

namespace object_collisions
{
	namespace helpers
	{
		inline bool ActorWasBelowSlope(Actor& actor, Slope& slope)
		{
			geometry::Line slope_line = geometry::Line(*slope.getSegment());
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

		geometry::Rectangle* rectangle = actor.getRectangle();
		geometry::Segment* segment = slope.getSegment();

		// Check collision from below
		if (helpers::ActorWasBelowSlope(actor, slope))
			return false;

		// Horisontal collision
		if (slope.isHorisontal())
		{
			geometry::Point left_start = geometry::Point(rectangle->Left(), rectangle->Up());
			geometry::Point right_start = geometry::Point(rectangle->Right(), rectangle->Up());
			geometry::Point vertical_vector = geometry::Point(0, -rectangle->Height());

			geometry::Segment left_segment = geometry::Segment(left_start, vertical_vector);
			geometry::Segment right_segment = geometry::Segment(right_start, vertical_vector);

			bool collision = geometry::collisions::SegmentToSegment(left_segment, *segment);
			collision |= geometry::collisions::SegmentToSegment(right_segment, *segment);
			return collision;
		}

		geometry::Point bottom_start = geometry::Point(rectangle->Left(), rectangle->Down());
		geometry::Point bottom_vector = geometry::Point(rectangle->Width(), 0);

		geometry::Segment bottom = geometry::Segment(bottom_start, bottom_vector);

		return geometry::collisions::SegmentToSegment(bottom, *segment);
	}

	namespace contact
	{
		inline geometry::Point ActorToSlope(Actor& actor, Slope& slope)
		{
			// Evaluate collision
			if (!object_collisions::ActorToSlope(actor, slope))
				return geometry::Point(NAN, NAN);

			// Move actor
			if (actor.getRectangle()->Down() < slope.getSegment()->UpperPoint().Y())
				actor.GetShape()->MoveTo(actor.GetShape()->PreviousPosition());

			// Calculate collision point
			double actor_feet_y = actor.getRectangle()->Down();

			geometry::Point slope_higher_point =	slope.getSegment()->UpperPoint();

			if (actor_feet_y >= slope_higher_point.Y())
				return slope_higher_point;

			double actor_feet_x =	slope.isRightSlope() ?
									actor.getRectangle()->Left() :
									actor.getRectangle()->Right();

			return geometry::Point(actor_feet_x, actor_feet_y);
		}
	} // contact
} // object_collisions
