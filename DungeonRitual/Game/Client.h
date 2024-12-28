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

	// Ping
	bool ping_sent = false;
	std::chrono::system_clock::time_point ping_request;
	long long ping_ms = -1;

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

	void Ping()
	{
		if (ping_sent)
			return;
		net::common::Message<NetContext> msg(ClientPing, 0);
		ping_sent = true;
		ping_request = std::chrono::system_clock::now();
		Send(msg);
	}

	long long getPing()
	{
		return ping_ms;
	}

protected:
	virtual void OnMessage(net::common::Message<NetContext>* msg)
	{
		// Process message
		switch (msg->getHeader().getType())
		{
		case ServerPing:
		{
			Send(*msg);
			break;
		}
		case ClientPing:
		{
			using namespace std::chrono;
			if (!ping_sent)
				break;

			system_clock::time_point response = system_clock::now();
			ping_ms = duration_cast<milliseconds>(response - ping_request).count();
			ping_sent = false;
			break;
		}
		case GameStart:
		{
			int player_id;
			msg->get(&player_id, sizeof(int));

			std::cout << "Starting new match with position [" << player_id << "]\n";
			
			match = new LocalMatch(window, "test", player_id, this);
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
		{
			if (!match)
				break;

			int player_id;
			Action action;
			msg->get(&player_id, sizeof(int));
			msg->get(&action, sizeof(Action));

			match->MakeActionAsPlayer(player_id, action);

			break;
		}
		case GameState:
		{
			Data* server_state = new Data(msg->getSize());
			msg->get(server_state->getData(), server_state->getSize());

			if (match)
				match->LoadServerState(server_state);

			std::cout << "Game state update!\n";
			break;
		}
		default: break;
		}

		delete msg;
	}
	virtual void OnDisconnect() {
		std::cout << "Disconnected\n";
	}
};