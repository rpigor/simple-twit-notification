#include "Application.hpp"
#include "Server.hpp"
#include "Account.hpp"
#include "Sessions.hpp"
#include "Tweet.hpp"
#include "Command.hpp"
#include "SessionCommand.hpp"
#include "FollowCommand.hpp"
#include "TweetCommand.hpp"

#include <iostream>
#include <vector>
#include <map>

void Application::run() {
	Server server(PORT); // open server and bind to port

	std::vector<Account> contas = { Account("matheus"), Account("gabriel"), Account("pedro"), Account("igor") };
	std::vector<Tweet> tweets;
	Sessions sessoes(contas);

	std::map<std::string, Command*> commands;

	SessionCommand sessionCommand(sessoes);
	FollowCommand followCommand(sessoes);
	TweetCommand tweetCommand(sessoes, tweets);

	commands["sessao"] = &sessionCommand;
	commands["seguir"] = &followCommand;
	commands["tweet"] = &tweetCommand;

	int recvLen;

	// keep listening for data
	while (true) {
		std::cout << "Waiting for data..." << std::endl;
		
		std::string buffer = server.recvMessage(); // try to receive some data, this is a blocking call

		std::cout << "Received request from " << inet_ntoa(server.getClientAddr().sin_addr) << ":" << ntohs(server.getClientAddr().sin_port) << "\n";

		std::string commandStr = buffer.substr(0, buffer.find(","));
		std::string payloadStr = buffer.substr(buffer.find(",") + 1);

		try {
            commands.at(commandStr)->setPayload(payloadStr);
		    commands.at(commandStr)->setNetwork(Network(server.getSocket(), server.getClientAddr()));
			commands.at(commandStr)->execute();
		}
		catch(const std::out_of_range& e) {
			std::cout << "Invalid command." << std::endl;
		}

		std::cout << std::endl;
	}
}