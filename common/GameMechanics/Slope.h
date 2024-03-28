#pragma once
#include "GameObject.h"
class Slope : public GameObject
{
private:
	double vertical_movement_component = 0;
	double horisontal_movement_component = 0;

public:
	Slope() {}
	Slope(Segment* shape)
		:GameObject(shape)
	{
		double dx = fabs(shape->Vector().X());
		double dy = fabs(shape->Vector().Y());

		double c = sqrt(dx * dx + dy * dy);

		vertical_movement_component = dy / c;
		horisontal_movement_component = dx / c;
	}

	Segment* getSegment()
	{
		return dynamic_cast<Segment*>(shape);
	}

	double VerticalMove()
	{
		return vertical_movement_component;
	}
	double HorisontalMove()
	{
		return horisontal_movement_component;
	}

	// Segment properties
	bool isVertical() { return getSegment()->Vector().X() == 0; }
	bool isHorisontal() { return getSegment()->Vector().Y() == 0; }
	bool isRightSlope()
	{
		if (isVertical() || isHorisontal())
			return false;

		Point vector = getSegment()->Vector();

		return vector.X() > 0 == vector.Y() > 0;
	}
	bool isLeftSlope()
	{
		if (isVertical() || isHorisontal())
			return false;

		Point vector = getSegment()->Vector();

		return vector.X() > 0 != vector.Y() > 0;
	}
};
