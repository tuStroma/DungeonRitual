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

protected:
	bool Update(double time_delta) override
	{
		chasing_times[chasing_actor] += time_delta;

		return false; // game_time > 60.0;
	}

	void Finalize() override
	{
		int winner = 0;
		for (int i = 0; i < chasing_times.size(); i++)
		{
			std::cout << "Player " << i << ", time:\t" << chasing_times[i] << '\n';
			if (chasing_times[i] < chasing_times[winner])
				winner = i;
		}
		std::cout << "\nThe winner is " << winner << "!\n";
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
};
