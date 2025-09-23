#pragma once
#include "../Communication/Observer.h"
#include "../MatchEnvironment.h"

class GameMode : public Observer
{
protected:
	MatchEnvironment* environment = nullptr;

	virtual void InitializeState() {}
public:
	GameMode()
	{}

	void SetEnvironment(MatchEnvironment* environment)
	{
		this->environment = environment;
		InitializeState();
	}

	virtual void Update(double time_delta) {}
};
