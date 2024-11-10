#pragma once

#include "ServerMatch.h"

class GameClient
{
private:
	ServerMatch* in_match = nullptr;
	int player_number = 0;

	uint64_t client_id;

public:
	GameClient(uint64_t id)
		:client_id(id)
	{}

	void JoinMatch(ServerMatch* match, int player_number)
	{
		in_match = match;
		this->player_number = player_number;
	}

	void LeaveMatch()
	{
		in_match = nullptr;
	}

	ServerMatch* getMatch()
	{
		return in_match;
	}

	int getMatchId()
	{
		return player_number;
	}

	uint64_t getClientId()
	{
		return client_id;
	}
};
