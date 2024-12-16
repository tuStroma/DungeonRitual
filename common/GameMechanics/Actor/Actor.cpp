#include "Actor.h"
#include "OutsideController.h"

void Actor::CheckFacingDirection()
{
	if (moving_left == moving_right)
		return;

	facing_right = moving_right;
}

Actor::Actor()
{
}

Actor::Actor(rapidxml::xml_node<>* node)
{
	// Load Actor object
	float position_x = atof(node->first_attribute("position_x")->value());
	float position_y = atof(node->first_attribute("position_y")->value());
	float width = atof(node->first_attribute("width")->value());
	float height = atof(node->first_attribute("height")->value());

	bool is_player = AssetLoader::BoolFromAttribute(node, "is_player");

	geometry::Point centre(position_x, position_y);

	std::cout << "Actor"
		<< "\n\tPosition_x:\t" << position_x
		<< "\n\tPosition_y:\t" << position_y
		<< "\n\tWidth:\t" << width
		<< "\n\tHeight:\t" << height
		<< "\n";

	this->shape = new geometry::Rectangle(centre, width, height);

	// Controller
	controller = new OutsideController();
}

void Actor::AddAbility(Ability* ability)
{
	if (basic_attack)
	{
		delete basic_attack;
		basic_attack = nullptr;
	}

	basic_attack = ability;
}

void Actor::Jump(bool jump)
{
	if (standing_on && !jumping)
	{
  		vertical_speed = jump_speed;
		standing_on = nullptr;
	}
	
	jumping = jump;
}

void Actor::BasicAttack()
{
	if (basic_attack)
		basic_attack->Execute();
}

void Actor::TakeAction()
{
	controller->TakeAction(this);
}

void Actor::SetVerticalSpeed(double speed)
{
	vertical_speed = speed;
	standing_on = nullptr;
}

void Actor::Update(double t)
{
	TakeAction();

	Move(t);

	// Continue abilities
	basic_attack->Continue(t);
}

void Actor::Move(double t)
{
	double vx = 0;

	CheckFacingDirection();

	if (standing_on)
	{
		WalkOnObject(standing_on, t);
		return;
	}

	// Horisontal
	if (moving_left && !moving_right) vx = -speed;
	else if (!moving_left && moving_right) vx = speed;

	// Vertical
	vertical_speed += t * GRAVITY;
	vertical_speed = MAX_FALL_SPEED > vertical_speed ? MAX_FALL_SPEED : vertical_speed;

	shape->MoveBy(geometry::Point(vx * t, vertical_speed * t));
}

void Actor::WalkOnObject(GameObject* floor, double t)
{
	geometry::Shape* floor_shape = floor->GetShape();

	double vx = 0;
	double vy = 0;

	if (geometry::Rectangle* r = dynamic_cast<geometry::Rectangle*>(floor_shape))
	{
		if (moving_left && !moving_right) vx = -speed;
		else if (!moving_left && moving_right) vx = speed;

		shape->MoveBy(geometry::Point(vx * t, 0));

		// Check if still standing on
		double x_distance = fabs(shape->Position().X() - r->Position().X());
		double standing_width = (((geometry::Rectangle*)shape)->Width() + r->Width()) / 2;

		if (x_distance >= standing_width)
			standing_on = nullptr;
	}
	if (geometry::Segment* s = dynamic_cast<geometry::Segment*>(floor_shape))
	{
		Slope* slope = dynamic_cast<Slope*>(floor);

		// Check avoiding slopes
		if (moving_down && slope->isPenetrable())
			standing_on = nullptr;

		// Slope top
		double feet_y = Position().Y() - getRectangle()->Height() / 2;
		geometry::Point slope_top = s->Vector().Y() >=0 ? s->EndPoint() : s->Position();



		if (feet_y >= slope_top.Y())
		{
			if (moving_left && !moving_right) vx = -speed;
			else if (!moving_left && moving_right) vx = speed;

			shape->MoveBy(geometry::Point(vx * t, 0));

			// Check if still standing on
			if (slope->isHorisontal())
			{
				if (this->getRectangle()->Right() <= slope->getSegment()->LeftPoint().X() ||
					this->getRectangle()->Left() >= slope->getSegment()->RightPoint().X())
					standing_on = nullptr;
			}
			else if (fabs(slope_top.X() - this->Position().X()) > this->getRectangle()->Width() / 2)
				standing_on = nullptr;

			return;
		}

		// Slope body
		if (moving_left && !moving_right)
		{
			vx = -speed * slope->HorisontalMove();
			vy = -speed * slope->VerticalMove();
		}
		else if (!moving_left && moving_right)
		{
			vx = speed * slope->HorisontalMove();
			vy = speed * slope->VerticalMove();
		}

		if (slope->isLeftSlope())
			vy = -vy;

		shape->MoveBy(geometry::Point(vx * t, vy * t));

		// Check if still standing on
		double feet_x = Position().X() - getRectangle()->Width() / 2;

		if (slope->isRightSlope())
			feet_x += getRectangle()->Width();

		if (feet_x < s->Position().X() && feet_x < s->EndPoint().X() ||
			feet_x > s->Position().X() && feet_x > s->EndPoint().X())
			standing_on = nullptr;
	}
}

void Actor::ResolveCollision(geometry::Point connection, GameObject* obj)
{
	double con_x = connection.X();
	double con_y = connection.Y();
	if (isnan(con_x) || isnan(con_y))
		return;

	double lower_edge = shape->Position().Y() - ((geometry::Rectangle*)shape)->Height() / 2;
	double higher_edge = shape->Position().Y() + ((geometry::Rectangle*)shape)->Height() / 2;

	if (con_y <= lower_edge)
	{
		standing_on = obj;
		vertical_speed = 0;
	}
	else if (con_y == higher_edge)
		vertical_speed = 0;
}

geometry::Rectangle* Actor::getRectangle()
{
	return dynamic_cast<geometry::Rectangle*>(shape);
}

Data* Actor::Serialize()
{
	size_t size = SerializationSize();
	Data* data = new Data(size);

	geometry::Point position = Position();

	data->put(&position, sizeof(geometry::Point));
	data->put(&vertical_speed, sizeof(double));
	return data;
}

size_t Actor::SerializationSize()
{
	size_t size = sizeof(geometry::Point) + sizeof(double);
	return size;
}

void Actor::Deserialize(Data* data)
{
	geometry::Point position;
	double vertical_speed;

	// Extract data
	data->get(&position, sizeof(geometry::Point));
	data->get(&vertical_speed, sizeof(double));

	// Update state
	shape->MoveTo(position);
	this->vertical_speed = vertical_speed;
}
