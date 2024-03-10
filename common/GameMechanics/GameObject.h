#pragma once
#include "geometry/Point.h"
#include "geometry/Collisions.h"

class GameObject
{
protected:
	Shape* shape = nullptr;

public:
	GameObject() {}
	GameObject(Shape* shape)
		:shape(shape)
	{}

	Point Position() { return shape->Position(); }
	Shape* GetShape() { return shape; }
};

