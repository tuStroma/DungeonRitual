#include "GameObject.h"

GameObject::GameObject(Point position, double width, double height, int color)
	:position(position), width(width), height(height), color(color)
{}

void GameObject::moveBy(Point vector)
{
	previous_position = position;
	position.Move(vector);
}

void GameObject::moveTo(Point new_pos)
{
	position = new_pos;
}
