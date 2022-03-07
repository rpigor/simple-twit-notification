#include "Application.hpp"
#include "Server.hpp"
#include "Account.hpp"
#include "Sessions.hpp"
#include "Tweet.hpp"
#include "SessionCommand.hpp"
#include "FollowCommand.hpp"
#include "TweetCommand.hpp"

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>

std::mutex Application::mutex;

void Application::run() {
	std::vector<Account> accounts = { Account("matheus"), Account("gabriel"), Account("pedro"), Account("igor") };
	std::vector<Tweet> tweets;
	Sessions sessions(accounts);
	std::map<Account, std::vector<Notification>> notifications;

	std::map<std::string, Command*> commands;
	SessionCommand sessionCommand(sessions);
	FollowCommand followCommand(sessions);
	TweetCommand tweetCommand(sessions, tweets, notifications);

	commands["sessao"] = &sessionCommand;
	commands["seguir"] = &followCommand;
	commands["tweet"] = &tweetCommand;

	Server server(PORT); // create socket and bind to port

	std::cout << "Listening to port " << PORT << "..." << std::endl;

	std::thread notifyThread(handleNotifications, std::ref(sessions), std::ref(notifications));

	std::vector<std::thread> connThreads;
	while (true) {
		Connection conn = server.acceptClientSocket();
		connThreads.push_back(std::thread(handleClient, conn, commands));
	}

	for (std::thread& t : connThreads) {
		t.join();
	}

	notifyThread.join();
}

void Application::handleClient(Connection conn, std::map<std::string, Command*> commands) {
	std::cout << "Creating new thread to handle client " << conn.getAddress() << ":" << conn.getPort() << std::endl;

	// loop while not disconnected
	while (conn.receiveMessage() > 0) {
		std::string message = conn.getMessage();
		std::string commandStr = message.substr(0, message.find(","));
		std::string payloadStr = message.substr(message.find(",") + 1);

		std::lock_guard<std::mutex> commandGuard(mutex); // synchronize access to resources

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

void Application::handleNotifications(Sessions& sessions, std::map<Account, std::vector<Notification>>& notifications) {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		std::lock_guard<std::mutex> notificationGuard(mutex);

		for (auto& entry : notifications) {
			Account account = entry.first;

			// consumes pending notification for client
			for (auto it = entry.second.begin(); it < entry.second.end(); ++it) {				
				std::string notifyMessage = "notify," + std::to_string(it->getTweet().getEpoch()) + "," + it->getAuthor().getUsername() + "," + it->getTweet().getMessage() + ",";
				std::pair<Session, Session> activeSessions = sessions.getActiveSessions(account);

				if (activeSessions.first.getClientConnection().sendMessage(notifyMessage) < 0 && activeSessions.second.getClientConnection().sendMessage(notifyMessage) < 0) {
					continue;
				}

				std::cout << "Notifying @" << account.getUsername() << "..." << std::endl;
				it = notifications.at(entry.first).erase(it);
			}
		}
	}
}