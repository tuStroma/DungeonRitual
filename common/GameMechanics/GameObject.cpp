#include "GameObject.h"

GameObject::GameObject(double x, double y, double width, double height, int color)
	:x(x), y(y), width(width), height(height), color(color)
{}

void GameObject::setPosition(double x, double y)
{
	this->x = x;
	this->y = y;
}
