#pragma once
#include "../Communication/Observer.h"
#include "../MatchEnvironment.h"

class GameMode : public Observer
{
protected:
	MatchEnvironment* environment = nullptr;

	double game_time = 0;

	virtual void InitializeState() {}

	virtual bool Update(double time_delta) = 0;

	virtual void Finalize() = 0;
public:
	GameMode()
	{}

	void SetEnvironment(MatchEnvironment* environment)
	{
		this->environment = environment;
		InitializeState();
	}

	bool ModeUpdate(double time_delta)
	{
		game_time += time_delta;
		bool end_game = Update(time_delta);

		if (end_game)
			Finalize();

		return end_game;
	}
};
