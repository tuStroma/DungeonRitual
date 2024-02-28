#include "GameObject.h"

GameObject::GameObject(Point positio, double width, double height, int color)
	:position(position), width(width), height(height), color(color)
{}

void GameObject::moveBy(Point vector)
{
	position.Move(vector);
}

void GameObject::moveTo(Point new_pos)
{
	position = new_pos;
}
