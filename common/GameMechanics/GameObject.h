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
	virtual ~GameObject() {}

	Point Position() { return shape->Position(); }
	Shape* GetShape() { return shape; }
};

