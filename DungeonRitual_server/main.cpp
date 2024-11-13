#include <iostream>

#include "Server.h"

int main(int argc, char* argv[])
{
	Server server = Server(60000);
	server.Start();

	while (true)
	{
		std::string command;
		std::cout << "> ";
		std::cin >> command;

		if (command == "q")
			break;
	}

	server.Stop();
	return 0;
}