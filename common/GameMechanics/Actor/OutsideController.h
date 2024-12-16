#pragma once
#include "ActorController.h"
#include "server_infrastructure.h"

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
	StopJump,

	BasicAttack
};

class OutsideController : public ActorController
{
private:
	//std::list<Action> actions;
	net::common::ThreadSharedQueue<Action> actions;


public:
	OutsideController()
	{}

	void TakeAction(Actor* actor) override
	{
		Action action;
		while (actions.pop(&action))
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
			case BasicAttack:
				actor->BasicAttack(); break;
			default: break;
			}
		}
	}

	void AddAction(Action action)
	{
		actions.push(action);
	}
};