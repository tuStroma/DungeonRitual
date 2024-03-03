#include "Actor.h"



Actor::Actor()
{
}

Actor::Actor(Point position, double width, double height, int color)
	:GameObject(position, width, height, color)
{}

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

	// Horisontal
	if (moving_left && !moving_right) vx = -speed;
	else if (!moving_left && moving_right) vx = speed;

	// Vertical
	if (standing_on)
		vertical_speed = 0.0;
	else
	{
		vertical_speed += t * GRAVITY;
		vertical_speed = MAX_FALL_SPEED > vertical_speed ? MAX_FALL_SPEED : vertical_speed;
	}

	moveBy(Point(vx * t, vertical_speed * t));
}

double Actor::Colision1D(double position, double previous_position, double size, double obj_a0, double obj_a1, bool& left_colision, bool& right_colision)
{
	// Return values - side of colision
	left_colision = false;
	right_colision = false;

	// Bounds of object
	double a0 = previous_position - size / 2;
	double a1 = previous_position + size / 2;

	bool intersect = true;

	if (a0 >= obj_a1 || a1 <= obj_a0)
		intersect = false;

	// Slide on the edge
	if (intersect)
		return position;

	// Stick to the edge
	bool from_left = a1 <= obj_a0;

	left_colision = from_left;
	right_colision = !from_left;

	return from_left ? obj_a0 - size / 2 : obj_a1 + size / 2;
}

void Actor::CheckColision(GameObject* obj)
{
	double x0 = position.X() - width / 2;
	double x1 = position.X() + width / 2;
	double y0 = position.Y() - height / 2;
	double y1 = position.Y() + height / 2;

	double obj_x0 = obj->Position().X() - obj->Width() / 2;
	double obj_x1 = obj->Position().X() + obj->Width() / 2;
	double obj_y0 = obj->Position().Y() - obj->Height() / 2;
	double obj_y1 = obj->Position().Y() + obj->Height() / 2;

	if (x0 >= obj_x1 || x1 <= obj_x0)
	{
		if (obj == standing_on)
			standing_on = nullptr;
		return;
	}

	if (y0 >= obj_y1 || y1 <= obj_y0)
		return;

	bool from_down, from_up, from_left, from_right;
	double new_x = Colision1D(position.X(), previous_position.X(), width, obj_x0, obj_x1, from_left, from_right);
	double new_y = Colision1D(position.Y(), previous_position.Y(), height, obj_y0, obj_y1, from_down, from_up);

	position = Point(new_x, new_y);

	// Vertical colision
	if (from_up)
		standing_on = obj;
	
	if (from_down || from_up)
		vertical_speed = 0;
	
}