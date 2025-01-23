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
		if (helpers::ActorWasBelowSlope(actor, slope) && slope.isPenetrable())
			return false;

		// Unpenetrable slope
		if (!slope.isPenetrable())
		{
			geometry::Point left_up = geometry::Point(rectangle->Left(), rectangle->Up());
			geometry::Point right_up = geometry::Point(rectangle->Right(), rectangle->Up());
			geometry::Point vertical_vector = geometry::Point(0, -rectangle->Height());
			geometry::Point horisontal_vector = geometry::Point(rectangle->Width(), 0);

			geometry::Segment left = geometry::Segment(left_up, vertical_vector);
			geometry::Segment right = geometry::Segment(right_up, vertical_vector);
			geometry::Segment up = geometry::Segment(left_up, horisontal_vector);
			geometry::Segment down = geometry::Segment(left_up + vertical_vector, horisontal_vector);

			return geometry::collisions::SegmentToSegment(left, *segment) ||
				   geometry::collisions::SegmentToSegment(right, *segment) ||
				   geometry::collisions::SegmentToSegment(up, *segment) ||
				   geometry::collisions::SegmentToSegment(down, *segment);
		}

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

			// Unpenetrable slope
			if (!slope.isPenetrable())
			{
				geometry::Rectangle* rectangle = actor.getRectangle();
				geometry::Segment* segment = slope.getSegment();

				geometry::Point connection = geometry::collisions::contact::SegmentToRectangle(*segment, *rectangle);

				geometry::Point move_vector = actor.GetShape()->Position() - actor.GetShape()->PreviousPosition();
				double new_position_x = actor.GetShape()->Position().X();
				double new_position_y = actor.GetShape()->Position().Y();

				double previous_up =	actor.GetShape()->PreviousPosition().Y() + rectangle->Height() / 2;
				double previous_down =	actor.GetShape()->PreviousPosition().Y() - rectangle->Height() / 2;
				double previous_left =	actor.GetShape()->PreviousPosition().X() - rectangle->Width() / 2;
				double previous_right =	actor.GetShape()->PreviousPosition().X() + rectangle->Width() / 2;

				if (connection.Y() >= previous_up || connection.Y() <= previous_down)
					new_position_y = actor.GetShape()->PreviousPosition().Y();

				if (connection.X() >= previous_right || connection.X() <= previous_left)
					new_position_x = actor.GetShape()->PreviousPosition().X();

				geometry::Point new_position = geometry::Point(new_position_x, new_position_y);
				actor.GetShape()->MoveTo(new_position);
				//actor.GetShape()->MoveTo(actor.GetShape()->PreviousPosition());

				return connection;
			}

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
