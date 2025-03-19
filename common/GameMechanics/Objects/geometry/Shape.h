#pragma once
#include "Point.h"

namespace geometry
{
	class Shape
	{
	protected:
		Point position;
		Point previous_position;

	public:
		Shape() {}
		virtual ~Shape() {}
		Shape(Point position)
			: position(position), previous_position(position)
		{}

		// Getters
		Point Position() { return position; }
		Point PreviousPosition() { return previous_position; }

		// Modificators
		void MoveBy(Point vector)
		{
			previous_position = position;
			position.Move(vector);
		}

		void MoveTo(Point new_position)
		{
			//previous_position = position;
			position.Set(new_position);
		}
	};
} // geometry
