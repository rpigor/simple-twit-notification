#include "Application.hpp"
#include "Server.hpp"
#include "Account.hpp"
#include "Sessions.hpp"
#include "Tweet.hpp"
#include "SessionCommand.hpp"
#include "FollowCommand.hpp"
#include "TweetCommand.hpp"
#include "NotificationResponseCommand.hpp"

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <csignal>
#include <fstream>

std::vector<Account> Application::accounts;

std::mutex Application::mutex;

void Application::run() {
	// handle accounts backup
	accounts = recoverAccounts();
	std::signal(SIGINT, backupAccounts);
	std::signal(SIGABRT, backupAccounts);
	std::signal(SIGTERM, backupAccounts);
	std::signal(SIGTSTP, backupAccounts);

	Sessions sessions(accounts);
	std::vector<Tweet> tweets;
	std::map<Account, std::vector<Notification>> notifications;

	// initialize commands
	std::map<std::string, Command*> commands;
	SessionCommand sessionCommand(sessions);
	FollowCommand followCommand(sessions);
	TweetCommand tweetCommand(sessions, tweets, notifications);
	NotificationResponseCommand notificationResponseCommand(sessions, notifications);
	commands["sessao"] = &sessionCommand;
	commands["seguir"] = &followCommand;
	commands["tweet"] = &tweetCommand;
	commands["notify"] = &notificationResponseCommand;

	// create socket and bind to port
	Server server(PORT);
	std::cout << "Listening to port " << PORT << "..." << std::endl;

	std::thread notifyThread(handleNotifications, std::ref(sessions), std::ref(notifications));

	std::vector<std::thread> reqThreads;
	while (true) {
		std::string message = server.receiveMessage();
		Connection conn = server.getConnection();
		reqThreads.push_back(std::thread(handleRequest, conn, commands, message));
	}

	for (std::thread& t : reqThreads) {
		t.join();
	}

	notifyThread.join();
}

void Application::handleRequest(Connection conn, std::map<std::string, Command*> commands, std::string message) {	
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

void Application::handleNotifications(Sessions& sessions, std::map<Account, std::vector<Notification>>& notifications) {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(700));
		std::lock_guard<std::mutex> notificationGuard(mutex);

		for (auto& entry : notifications) {
			Account account = entry.first;

			// consumes pending notification for client
			for (auto it = entry.second.begin(); it < entry.second.end(); ++it) {
				std::string notifyMessage = "notify," + account.getUsername() + "," + std::to_string(it->getTweet().getEpoch()) + "," + it->getAuthor().getUsername() + "," + it->getTweet().getMessage() + ",";
				std::pair<Session, Session> activeSessions = sessions.getActiveSessions(account);

				if (activeSessions.first.getSessionId() == 0 && activeSessions.second.getSessionId() == 0) {
					continue;
				}

				if (activeSessions.first.getSessionId() != 0) {
					if (activeSessions.first.getClientConnection().sendMessage(notifyMessage) < 0) {
						continue;
					}
				}

				if (activeSessions.second.getSessionId() != 0) {
					if (activeSessions.second.getClientConnection().sendMessage(notifyMessage) < 0) {
						continue;
					}
				}
			}
		}
	}
}

void Application::backupAccounts(int i) {
	std::ofstream file(accountsFilename, std::ios::out | std::ios::trunc);
	if (file.good()) {
		file << accounts.size() << " ";

		for (const Account& acc : accounts) {
			file << acc << " ";
		}

		std::cout << "Saved " << accounts.size() << " accounts to backup." << std::endl;
	}
	file.close();

	exit(0);
}

std::vector<Account> Application::recoverAccounts() {
	std::vector<Account> accountVector;

	std::ifstream file(accountsFilename, std::ios::in);
	if (file.good()) {
		int accountsLen;
		file >> accountsLen;

		for (int i = 0; i < accountsLen; ++i) {
			Account auxAcc;
			file >> auxAcc;
			accountVector.push_back(auxAcc);
		}

		std::cout << "Recovered " << accountsLen << " accounts from backup." << std::endl;
	}
	file.close();

	return accountVector;
}