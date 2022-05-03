#include "Application.hpp"
#include "Server.hpp"
#include "Account.hpp"
#include "Sessions.hpp"
#include "Tweet.hpp"
#include "SessionCommand.hpp"
#include "FollowCommand.hpp"
#include "TweetCommand.hpp"
#include "NotificationResponseCommand.hpp"
#include "GroupSessionCommand.hpp"
#include "GroupFollowCommand.hpp"
#include "GroupTweetCommand.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <fstream>
#include <csignal>

Accounts Application::accounts;

int Application::leaderId;
int Application::serverId;

std::mutex Application::mutex;
std::mutex Application::connectionMutex;

void Application::run() {
	handleAccountsInitialization(); // recovers accounts and handles backup

	Sessions sessions(accounts);
	std::vector<Tweet> tweets;
	std::map<std::string, std::vector<Notification>> notifications;

	// initializes group commands
	std::map<std::string, Command*> groupCommands;
	GroupSessionCommand groupSessionCommand(sessions);
	GroupFollowCommand groupFollowCommand(sessions);
	GroupTweetCommand groupTweetCommand(sessions, tweets, notifications);
	groupCommands["sessao"] = &groupSessionCommand;
	groupCommands["seguir"] = &groupFollowCommand;
	groupCommands["tweet"] = &groupTweetCommand;

	// creates group socket and bind to port
	Group group(GROUP_IP, GROUP_PORT);
	std::cout << "Listening to port " << GROUP_PORT << " on group IP " << GROUP_IP << "..." << std::endl;

	// asks for leader
	group.sendMessage("group,");
	group.receiveMessage();
	std::string groupMessage = group.getMessage();

	std::thread groupThread(handleGroup, std::ref(group), groupCommands);

	if (!groupMessage.empty()) {
		// there's already a leader: I'm a secondary server
		std::string auxServerId, serverIdStr, auxLeaderId;
		if (groupMessage.substr(0, groupMessage.find(",")) == "leader") {
			auxServerId = groupMessage.substr(groupMessage.find(",") + 1);
			auxLeaderId = auxServerId.substr(auxServerId.find(",") + 1);
			leaderId = std::stoi(auxLeaderId.substr(0, auxLeaderId.find(",")));
		}

		// tries to receive leader message from other servers
		// to calculate this server's id
		serverId = 0;
		group.receiveMessage();
		groupMessage = group.getMessage();
		while (!groupMessage.empty()) {
			if (groupMessage.substr(0, groupMessage.find(",")) != "leader") {
				group.receiveMessage();
				groupMessage = group.getMessage();
				continue;
			}

			auxServerId = groupMessage.substr(groupMessage.find(",") + 1);
			serverIdStr = auxServerId.substr(0, auxServerId.find(","));
			auxLeaderId = auxServerId.substr(auxServerId.find(",") + 1);
			leaderId = std::stoi(auxLeaderId.substr(0, auxLeaderId.find(",")));
			serverId = std::max(serverId, std::stoi(serverIdStr));

			group.receiveMessage();
			groupMessage = group.getMessage();
		}
		++serverId;

		std::cout << "I'm not the leader :(. " << std::endl;
		std::cout << "The leader's id is: " << leaderId << std::endl;
		std::cout << "My id: " << serverId << std::endl;

		groupThread.join();

		return;
	}

	// no response or timeout: I'm the leader
	serverId = 0;
	leaderId = serverId;
	std::cout << "I'm the leader!" << std::endl;
	std::cout << "My id: " << serverId << std::endl;

	// initialize server commands
	std::map<std::string, Command*> commands;
	SessionCommand sessionCommand(sessions);
	FollowCommand followCommand(sessions);
	TweetCommand tweetCommand(sessions, tweets, notifications);
	NotificationResponseCommand notificationResponseCommand(sessions, notifications);
	commands["sessao"] = &sessionCommand;
	commands["seguir"] = &followCommand;
	commands["tweet"] = &tweetCommand;
	commands["notify"] = &notificationResponseCommand;

	// create server socket and bind to port
	Server server(SERVER_PORT);
	std::cout << "Listening to port " << SERVER_PORT << "..." << std::endl;

	std::thread notifyThread(handleNotifications, std::ref(sessions), std::ref(notifications));

	std::vector<std::thread> reqThreads;

	while (true) {
		std::string message = server.receiveMessage();
		Connection conn = server.getConnection();
		reqThreads.push_back(std::thread(handleRequest, conn, std::ref(group), commands, message));
	}

	for (std::thread& t : reqThreads) {
		t.join();
	}

	notifyThread.join();

	groupThread.join();
}

void Application::handleGroup(Group& group, std::map<std::string, Command*> commands) {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::lock_guard<std::mutex> connectionGuard(connectionMutex);

		group.receiveMessage();
		std::string message = group.getMessage();

		if (message.empty()) {
			continue;
		}

		std::string commandStr = message.substr(0, message.find(","));
		std::string payloadStr = message.substr(message.find(",") + 1);

		if (commandStr == "group") {
			std::cout << "Answered election request from group." << std::endl;
			std::string groupResponse = "leader," + std::to_string(serverId) + "," + std::to_string(leaderId) + ",";
			group.sendMessage(groupResponse);
			continue;
		}
		else if (commandStr == "leader") {
			std::string auxGroupLeader = payloadStr.substr(payloadStr.find(",") + 1);
			leaderId = std::stoi(auxGroupLeader.substr(0, auxGroupLeader.find(",")));
			continue;
		}

		try {
			Connection auxConnection = Connection();
			commands.at(commandStr)->setConnectable(static_cast<Connectable*>(&auxConnection));
			commands.at(commandStr)->setPayload(payloadStr);

			{
				std::lock_guard<std::mutex> commandGuard(mutex);
				commands.at(commandStr)->execute();
			}
		}
		catch(const std::out_of_range& e) {
			std::cout << "Invalid group command." << std::endl;
		}
	}
}

void Application::handleRequest(Connection conn, Group& group, std::map<std::string, Command*> commands, std::string message) {	
	std::string commandStr = message.substr(0, message.find(","));
	std::string payloadStr = message.substr(message.find(",") + 1);

	try {
		commands.at(commandStr)->setConnectable(static_cast<Connectable*>(&conn));
		commands.at(commandStr)->setPayload(payloadStr);

		{ 
			std::lock_guard<std::mutex> commandGuard(mutex); // synchronize access to resources
			commands.at(commandStr)->execute();
		}

		{ 
			std::lock_guard<std::mutex> connectionGuard(connectionMutex);
			group.sendMessage(message);
		}
	}
	catch(const std::out_of_range& e) {
		std::cout << "Invalid command." << std::endl;
	}

	std::cout << std::endl;
}

void Application::handleNotifications(Sessions& sessions, std::map<std::string, std::vector<Notification>>& notifications) {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(750));
		std::lock_guard<std::mutex> notificationGuard(mutex);

		for (auto& entry : notifications) {
			Account account = *accounts.findAccount(entry.first);

			// consumes pending notification for client
			for (auto it = entry.second.begin(); it < entry.second.end(); ++it) {
				std::string notifyMessage = "notify," + account.getUsername() + "," + std::to_string(it->getTweet().getEpoch()) + "," + it->getAuthor() + "," + std::to_string(it->getTweet().getMessage().length()) + "," + it->getTweet().getMessage() + ",";
				std::pair<Session, Session> activeSessions = sessions.getActiveSessions(account.getUsername());

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

void Application::handleAccountsInitialization() {
	recoverAccounts();

	// configures signals to backup accounts when called
	std::signal(SIGINT, backupAccounts);
	std::signal(SIGABRT, backupAccounts);
	std::signal(SIGTERM, backupAccounts);
	std::signal(SIGTSTP, backupAccounts);
}

void Application::backupAccounts(int i) {
	std::ofstream file(ACCOUNTS_FILENAME, std::ios::out | std::ios::trunc);
	if (file.good()) {
		file << accounts.size() << " ";

		for (auto acc : accounts) {
			file << *acc << " ";
		}

		std::cout << "Saved " << accounts.size() << " accounts to backup." << std::endl;
	}
	file.close();

	exit(0);
}

void Application::recoverAccounts() {
	std::ifstream file(ACCOUNTS_FILENAME, std::ios::in);
	if (file.good()) {
		int accountsLen;
		file >> accountsLen;

		for (int i = 0; i < accountsLen; ++i) {
			Account auxAcc;
			file >> auxAcc;
			accounts.createAccount(auxAcc);
		}

		std::cout << "Recovered " << accountsLen << " accounts from backup." << std::endl;
	}
	file.close();
}