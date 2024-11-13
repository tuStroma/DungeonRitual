#pragma once
#include <server_infrastructure.h>
#include <map>
#include <list>
#include "../common/Network/NetContext.h"

#include "ServerMatch.h"
#include "GameClient.h"

class Server : public net::server::IServer<NetContext>
{
private:
	std::map<uint64_t, GameClient*> players;
	std::list<GameClient*> queue;

	uint64_t match_id = 0;
	std::map<uint64_t, ServerMatch*> game_room;

	void StartNewMatch(std::string map, int players_number)
	{
		std::cout << "Starting new match [" << match_id << "]\n";
		ServerMatch* match = new ServerMatch(map);

		std::cout << "Add players\n";
		for (int i = 0; i < players_number; i++)
		{
			GameClient* client = queue.front(); queue.pop_front();
			match->AddPlayer(client);
			client->JoinMatch(match, i);
		}

		std::cout << "Add match to game room\n";
		game_room[match_id] = match;
		match->StartInThread([&]() {game_room.erase(match_id); });
		match_id++;

		// Send info about game start
		std::cout << "Send starting info\n";
		match->ForEachPlayer([&](uint64_t id, GameClient* game_client) {
			int player_id = game_client->getMatchId();

			net::common::Message<NetContext> start_msg(GameStart, sizeof(int));
			start_msg.put(&player_id, sizeof(int));
			Send(start_msg, id);
			std::cout << "Sent start info to player [" << player_id << "]\n";
			});
	}

	OutsideController* FindPlayerController(uint64_t player_id)
	{
		GameClient* player = players[player_id];
		ServerMatch* match = player->getMatch();

		if (!match)
			return nullptr;

		return match->getPlayerController(player_id);
	}

	void PlayerTakesAction(uint64_t client_id, Action action)
	{
		GameClient* player = players[client_id];
		ServerMatch* match = player->getMatch();

		if (!match)
			return;

		OutsideController* controller = match->getPlayerController(client_id);
		int player_id = match->getGameClient(client_id)->getMatchId();

		if (!controller)
			return;

		controller->AddAction(action);
		
		// Send action to all players
		net::common::Message<NetContext> msg(MoveAction, sizeof(int) + sizeof(Action));
		msg.put(&player_id, sizeof(int));
		msg.put(&action, sizeof(Action));

		match->ForEachPlayer([&](uint64_t id, GameClient* game_client) {
			if (client_id != id)
				this->Send(msg, id);
			});
	}

public:
	Server(int port) : net::server::IServer<NetContext>(port) {}
protected:
	virtual void OnMessage(net::common::Message<NetContext>* msg, uint64_t sender)
	{
		// Process message
		switch (msg->getHeader().getType())
		{
		case FindGame:
		{
			std::cout << "Client " << sender << " joined the queue\n";
			queue.push_back(players[sender]);

			if (queue.size() >= 2)
				StartNewMatch("test", 2);

			break;
		}
		case PlayerReady:
		case LeaveGame: break;

		// In game
		case MoveAction:
		{
			Action action;
			msg->get(&action, sizeof(action));

			PlayerTakesAction(sender, action);
			break;
		}
		default: break;
		}

		delete msg;
	}

	virtual bool OnClientConnect(std::string address, uint64_t client_id)
	{
		GameClient* client = new GameClient(client_id);
		players[client_id] = client;

		std::cout << "Welcome " << client_id << " with address " << address << "\n";

		return true;
	}

	virtual void OnClientDisconnect(uint64_t client_id)
	{
		std::cout << "Client " << client_id << " disconnected\n";
	}
};
