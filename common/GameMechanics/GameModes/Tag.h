#pragma once
#include "GameMode.h"
#include "../Communication/Events/HitEvent.h"

class Tag : public GameMode
{
private:
	int chasing_actor = 0;

	std::vector<double> chasing_times;

	void InitializeState() override
	{
		// Initialize chasing times for all actors
		for (int i = 0; i < environment->actors.size(); i++)
			chasing_times.push_back(0);
	}

public:
	void onEvent(Event* event) override
	{
		if (HitEvent* hit_e = dynamic_cast<HitEvent*>(event))
		{
			std::cout << hit_e->getAttacker() << " just hit " << hit_e->getReceiver() << "!\n";

			// Change chaser
			if (environment->actors[chasing_actor] == hit_e->getAttacker())
			{
				int new_chaser = environment->FindActorIndex(hit_e->getReceiver());
				if (new_chaser >= 0)
					chasing_actor = new_chaser;
			}
		}
	}

	void Update(double time_delta) override
	{
		chasing_times[chasing_actor] += time_delta;
	}
};
