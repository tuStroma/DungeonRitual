#pragma once
#include "Actor.h"

class ActorController
{
private:
public:
	ActorController();

	virtual void TakeAction(Actor* actor);
};

