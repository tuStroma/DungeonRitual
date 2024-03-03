#include "GameObject.h"

GameObject::GameObject(Point position, double width, double height, int color)
	:position(position), width(width), height(height), color(color)
{}

void GameObject::moveBy(Point vector)
{
	previous_position = position;
	position.Move(vector);
}

double GameObject::Colision1D(double position, double previous_position, double size, double obj_a0, double obj_a1)
{
	double a0 = previous_position - size / 2;
	double a1 = previous_position + size / 2;

	bool intersect = true;

	if (a0 >= obj_a1 || a1 <= obj_a0)
		intersect = false;

	if (intersect)
		return position;

	bool from_left = a1 <= obj_a0;
	
	return from_left ? obj_a0 - size / 2 : obj_a1 + size / 2;
}

void GameObject::CheckColision(GameObject* obj)
{
	double x0 = position.X() - width / 2;
	double x1 = position.X() + width / 2;
	double y0 = position.Y() - height / 2;
	double y1 = position.Y() + height / 2;

	double obj_x0 = obj->position.X() - obj->Width() / 2;
	double obj_x1 = obj->position.X() + obj->Width() / 2;
	double obj_y0 = obj->position.Y() - obj->Height() / 2;
	double obj_y1 = obj->position.Y() + obj->Height() / 2;

	if (x0 >= obj_x1 || x1 <= obj_x0)
		return;

	if (y0 >= obj_y1 || y1 <= obj_y0)
		return;

	double new_x = Colision1D(position.X(), previous_position.X(), width, obj_x0, obj_x1);
	double new_y = Colision1D(position.Y(), previous_position.Y(), height, obj_y0, obj_y1);

	position = Point(new_x, new_y);
}

void GameObject::moveTo(Point new_pos)
{
	position = new_pos;
}
