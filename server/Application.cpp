#include "Application.hpp"
#include "Server.hpp"
#include "Connection.hpp"
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
#include <thread>

std::mutex Application::commandMutex;

void Application::run() {
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

	Server server(PORT); // open server and bind to port
	std::vector<std::thread> connThreads;

	std::cout << "Listening to port " << PORT << "..." << std::endl;

	while (true) {
		Connection conn = server.acceptClientSocket();
		std::cout << "Creating new thread to handle client " << conn.getAddress() << ":" << conn.getPort() << std::endl;
		connThreads.push_back(std::thread(handleClient, conn, commands));
	}

	for (std::thread& t : connThreads) {
		t.join();
	}
}

void Application::handleClient(Connection conn, std::map<std::string, Command*> commands) {
	// loop while not disconnected
	while (conn.receiveMessage() > 0) {
		std::string message = conn.getMessage();
		std::string commandStr = message.substr(0, message.find(","));
		std::string payloadStr = message.substr(message.find(",") + 1);

		std::lock_guard<std::mutex> commandGuard(commandMutex); // synchronize access to resources

		try {
		    commands.at(commandStr)->setConnection(conn);
            commands.at(commandStr)->setPayload(payloadStr);
			commands.at(commandStr)->execute();
		}
		catch(const std::out_of_range& e) {
			std::cout << "Invalid command." << std::endl;
		}

		std::cout << std::endl;
	}

	conn.closeConnection();

	std::cout << "Finished thread from client " << conn.getAddress() << ":" << conn.getPort() << std::endl;
}