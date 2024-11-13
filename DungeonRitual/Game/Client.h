#pragma once
#include <server_infrastructure.h>
#include "../../common/Network/NetContext.h"
#include "LocalMatch.h"

class Client : public net::client::IClient<NetContext>
{
private:
	Window* window;
	LocalMatch* match = nullptr;
	int position = -1;

public:
	Client(Window* window) : net::client::IClient<NetContext>(), window(window) {}

	LocalMatch* getMatch()
	{
		return match;
	}
	int getPosition()
	{
		return position;
	}

	void FindMatch()
	{
		net::common::Message<NetContext> msg(FindGame, 0);
		Send(msg);
	}

protected:
	virtual void OnMessage(net::common::Message<NetContext>* msg)
	{
		// Process message
		switch (msg->getHeader().getType())
		{
		case GameStart:
		{
			int player_id;
			msg->get(&player_id, sizeof(int));

			std::cout << "Starting new match with position [" << player_id << "]\n";
			
			match = new LocalMatch(window, "test", player_id);
			//match = new LocalMatch(window, "test", 0);
			//match->Start();
			position = player_id;

			std::cout << "Match created\n";

			break;
		}
		case GameFinished:
		case LeaveGame:

		// Match communication
		case MoveAction:
		case GameState:
		default: break;
		}

		delete msg;
	}
	virtual void OnDisconnect() {
		std::cout << "Disconnected\n";
	}
};