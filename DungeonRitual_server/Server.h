#pragma once
#include <server_infrastructure.h>
#include <map>
#include <list>
#include "../common/Network/NetContext.h"

#include "ServerMatch.h"
#include "GameClient.h"

#define LAG_SIMULATION false
#if LAG_SIMULATION
	#define LAG 0.5
#endif

class Server : public net::server::IServer<NetContext>
{
private:
	std::map<uint64_t, GameClient*> players;
	std::list<GameClient*> queue;

	uint64_t match_id = 0;
	std::map<uint64_t, ServerMatch*> game_room;

#if LAG_SIMULATION
	// LAG simulation
	struct delayed_msg
	{
		net::common::Message<NetContext>* msg;
		uint64_t client_id;
		std::chrono::system_clock::time_point send_time;
		delayed_msg(net::common::Message<NetContext>* msg, uint64_t client_id, std::chrono::system_clock::time_point send_time)
			:msg(msg), client_id(client_id), send_time(send_time)
		{}
		explicit delayed_msg(const delayed_msg& msg)
			//:msg(msg.msg), client_id(msg.client_id), delay(msg.delay)
		{
			this->msg = msg.msg;
			this->client_id = msg.client_id;
			this->send_time = msg.send_time;
		}
	};
	std::list<delayed_msg> delayed_messages;
	std::thread sending_thread;
	bool close_sending_thread = false;
#endif

	void StartNewMatch(std::string map, int players_number)
	{
		std::cout << "Starting new match [" << match_id << "]\n";
		ServerMatch* match = new ServerMatch(map, this);

		for (int i = 0; i < players_number; i++)
		{
			GameClient* client = queue.front(); queue.pop_front();
			match->AddPlayer(client);
			client->JoinMatch(match, i);
		}

		game_room[match_id] = match;
		match->StartInThread([&]() {game_room.erase(match_id); });
		match_id++;

		// Send info about game start
		match->ForEachPlayer([&](uint64_t id, GameClient* game_client) {
			int player_id = game_client->getMatchId();

			net::common::Message<NetContext> start_msg(GameStart, sizeof(int));
			start_msg.put(&player_id, sizeof(int));
			Send(start_msg, id);
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

		// Send to everyone, including player taking
		// the move action, to compensate the lag
		match->ForEachPlayer([&](uint64_t id, GameClient* game_client) {
				this->Send(msg, id);
			});
	}

public:
	Server(int port) : net::server::IServer<NetContext>(port) 
	{
#if LAG_SIMULATION
		using namespace std::chrono;
		sending_thread = std::thread([&]() {
			while (!close_sending_thread)
			{
				system_clock::time_point t2 = system_clock::now();
				for (auto it = delayed_messages.begin(); it != delayed_messages.end();) {
					double delta = duration_cast<microseconds>(t2 - (*it).send_time).count() * 0.000001;

					if (delta >= LAG) {
						IServer::Send(*(*it).msg, (*it).client_id);
						//delete &(*it).msg;
						it = delayed_messages.erase(it);
					}
					else
						it++;
				}
			}
			});
#endif
	}
#if LAG_SIMULATION
	~Server()
	{
		close_sending_thread = true;
		sending_thread.join();
	}
#endif

#if LAG_SIMULATION
	// LAG simulation
	void Send(net::common::Message<NetContext>& msg, uint64_t client_id)
	{
		delayed_messages.push_back(delayed_msg(new net::common::Message<NetContext>(msg), client_id, std::chrono::system_clock::now()));
	}
#endif
protected:
	virtual void OnMessage(net::common::Message<NetContext>* msg, uint64_t sender)
	{
		// Process message
		switch (msg->getHeader().getType())
		{
		case ClientPing:
		{
			Send(*msg, sender);
			break;
		}
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

			std::string actions[] = {"Left","Right","Down","Jump","StopLeft","StopRight","StopDown","StopJump"};
			std::cout << "Player [" << sender << "], action: " << actions[action] << "\n";
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
