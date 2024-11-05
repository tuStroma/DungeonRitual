#pragma once
#include "ActorController.h"
#include <list>

//class Actor {};

enum Action
{
	Left,
	Right,
	Down,
	Jump,

	StopLeft,
	StopRight,
	StopDown,
	StopJump
};

class OutsideController : public ActorController
{
private:
	std::list<Action> actions;


public:
	OutsideController()
	{}

	void TakeAction(Actor* actor) override
	{
		for (Action action : actions)
		{
			switch (action)
			{
			case Left:
				actor->MoveLeft(true); break;
			case Right:
				actor->MoveRight(true); break;
			case Down:
				actor->MoveDown(true); break;
			case Jump:
				actor->Jump(true); break;

			case StopLeft:
				actor->MoveLeft(false); break;
			case StopRight:
				actor->MoveRight(false); break;
			case StopDown:
				actor->MoveDown(false); break;
			case StopJump:
				actor->Jump(false); break;
			default: break;
			}
		}

		actions.clear();
	}

	void AddAction(Action action)
	{
		actions.push_back(action);
	}
};