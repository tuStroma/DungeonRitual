#include "Actor.h"

Actor::Actor()
{
}

void Actor::Jump(bool jump)
{
	if (standing_on && !jumping)
	{
  		vertical_speed = jump_speed;
		standing_on = nullptr;
	}
	
	jumping = jump;
}

void Actor::Move(double t)
{
	double vx = 0;

	if (standing_on)
	{
		WalkOnObject(standing_on, t);
		return;
	}

	// Horisontal
	if (moving_left && !moving_right) vx = -speed;
	else if (!moving_left && moving_right) vx = speed;

	// Vertical
	vertical_speed += t * GRAVITY;
	vertical_speed = MAX_FALL_SPEED > vertical_speed ? MAX_FALL_SPEED : vertical_speed;

	shape->MoveBy(Point(vx * t, vertical_speed * t));
}

void Actor::WalkOnObject(GameObject* floor, double t)
{
	Shape* floor_shape = floor->GetShape();

	if (Rectangle* r = static_cast<Rectangle*>(floor_shape))
	{
		double vx = 0.0;
		if (moving_left && !moving_right) vx = -speed;
		else if (!moving_left && moving_right) vx = speed;

		shape->MoveBy(Point(vx * t, 0));

		// Check if still standing on
		double x_distance = fabs(shape->Position().X() - r->Position().X());
		double standing_width = (((Rectangle*)shape)->Width() + r->Width()) / 2;

		if (x_distance >= standing_width)
			standing_on = nullptr;
	}
}

void Actor::ResolveCollision(Point connection, GameObject* obj)
{
	double con_x = connection.X();
	double con_y = connection.Y();
	if (isnan(con_x) || isnan(con_y))
		return;

	double lower_edge = shape->Position().Y() - ((Rectangle*)shape)->Height() / 2;
	double higher_edge = shape->Position().Y() + ((Rectangle*)shape)->Height() / 2;

	if (con_y == lower_edge)
	{
		standing_on = obj;
		vertical_speed = 0;
	}
	else if (con_y == higher_edge)
		vertical_speed = 0;
}
