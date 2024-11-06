#pragma once
#include <server_infrastructure.h>
#include "../common/Network/NetContext.h"

class Server : public net::server::IServer<NetContext>
{
public:
	Server(int port) : net::server::IServer<NetContext>(port) {}
protected:
	virtual void OnMessage(net::common::Message<NetContext>* msg, uint64_t sender)
	{
		// Process message
		switch (msg->getHeader().getType())
		{
		case FindGame:
		case GameFound:
		case Ready:
		case GameStart:
		case GameFinished:
		case LeaveGame:
		}

		delete msg;
	}

	virtual bool OnClientConnect(std::string address, uint64_t client_id)
	{
		std::cout << "Welcome " << client_id << " with address " << address << "\n";
		return true;
	}

	virtual void OnClientDisconnect(uint64_t client_id)
	{
		std::cout << "Client " << client_id << " disconnected\n";
	}
};
