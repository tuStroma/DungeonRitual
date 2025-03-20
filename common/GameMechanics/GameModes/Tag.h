#pragma once
#include "GameMode.h"
#include "../Communication/Events/HitEvent.h"

class Tag : public GameMode
{
private:
public:
	void onEvent(Event* event) override
	{
		if (HitEvent* hit_e = dynamic_cast<HitEvent*>(event))
		{
			std::cout << hit_e->getAttacker() << " just hit " << hit_e->getReceiver() << "!\n";
		}
	}
};
