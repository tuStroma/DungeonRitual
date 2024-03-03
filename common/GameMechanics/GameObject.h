#pragma once
#include "geometry/Point.h"

class GameObject
{
private:
	Point position;
	Point previous_position;

	double width = 0, height = 0;

	// To be replaced with sprites and animations
	int color = 0;

public:
	GameObject() {}
	GameObject(Point position, double width, double height, int color);

	Point Position() { return position; }

	double Width() { return width; }
	double Height() { return height; }

	int Color() { return color; }

	void moveTo(Point new_pos);
	void moveBy(Point vector);

	// Colisions
	double Colision1D(double position, double previous_position, double size, double obj_a0, double obj_a1);
	void CheckColision(GameObject* obj);

};

