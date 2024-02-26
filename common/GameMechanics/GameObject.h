#pragma once

class GameObject
{
private:
	double x = 0, y = 0;

	double width = 0, height = 0;

	// To be replaced with sprites and animations
	int color = 0;

public:
	GameObject() {}
	GameObject(double x, double y, double width, double height, int color);

	double X() { return x; }
	double Y() { return y; }

	double Width() { return width; }
	double Height() { return height; }

	int Color() { return color; }

	void setPosition(double x, double y);
};

