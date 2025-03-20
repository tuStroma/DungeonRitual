#pragma once
#include "Event.h"
#include "../../Actor/Actor.h"

class HitEvent : public Event
{
private:
	Actor* attacker;
	Actor* receiver;
public:
	HitEvent(Actor* attacker, Actor* receiver)
		:attacker(attacker), receiver(receiver)
	{}

	Actor* getAttacker() { return attacker; }
	Actor* getReceiver() { return receiver; }
};
