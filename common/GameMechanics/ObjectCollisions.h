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

		if (slope.isHorisontal()) // We are fucked
		{
			return false;
		}

		Point bottom_start = Point(	actor.Position().X() - rectangle->Width() / 2,
									actor.Position().Y() - rectangle->Height() / 2);
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
			actor.GetShape()->MoveTo(actor.GetShape()->PreviousPosition());

			// Calculate collision point
			double actor_feet_y = actor.Position().Y() - actor.getRectangle()->Height() / 2;

			Point slope_higher_point =	slope.Position().Y() >= slope.getSegment()->EndPoint().Y() ?
										slope.Position() :
										slope.getSegment()->EndPoint();

			if (actor_feet_y >= slope_higher_point.Y())
				return slope_higher_point;

			double actor_feet_x = actor.Position().X() - actor.getRectangle()->Width() / 2;

			if (slope.isRightSlope())
				actor_feet_x += actor.getRectangle()->Width();

			return Point(actor_feet_x, actor_feet_y);
		}
	} // contact
} // object_collisions
