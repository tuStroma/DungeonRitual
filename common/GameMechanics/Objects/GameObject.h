#pragma once
#include "geometry/Point.h"
#include "geometry/Collisions.h"

class GameObject
{
protected:
	geometry::Shape* shape = nullptr;

public:
	GameObject() {}
	GameObject(geometry::Shape* shape)
		:shape(shape)
	{}
	virtual ~GameObject()
	{
		delete shape;
	}

	geometry::Point Position() { return shape->Position(); }
	geometry::Shape* GetShape() { return shape; }
};

