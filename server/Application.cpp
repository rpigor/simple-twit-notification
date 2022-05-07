#include "Application.hpp"
#include "Server.hpp"
#include "Account.hpp"
#include "Sessions.hpp"
#include "Tweet.hpp"
#include "Messages.hpp"
#include "SessionCommand.hpp"
#include "CloseSessionCommand.hpp"
#include "FollowCommand.hpp"
#include "TweetCommand.hpp"
#include "NotificationResponseCommand.hpp"
#include "GroupSessionCommand.hpp"
#include "GroupFollowCommand.hpp"
#include "GroupTweetCommand.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <thread>
#include <chrono>
#include <fstream>
#include <csignal>

Accounts Application::accounts;

int Application::leaderId;
int Application::serverId;
int Application::serverCount = 1;

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
	CloseSessionCommand groupCloseSessionCommand(sessions);
	GroupFollowCommand groupFollowCommand(sessions);
	GroupTweetCommand groupTweetCommand(sessions, tweets, notifications);
	NotificationResponseCommand groupNotificationResponseCommand(sessions, notifications);
	groupCommands[Messages::SESSION_COMMAND] = &groupSessionCommand;
	groupCommands[Messages::CLOSE_SESSION_COMMAND] = &groupCloseSessionCommand;
	groupCommands[Messages::FOLLOW_COMMAND] = &groupFollowCommand;
	groupCommands[Messages::TWEET_COMMAND] = &groupTweetCommand;
	groupCommands[Messages::NOTIFICATION_RESPONSE_COMMAND] = &groupNotificationResponseCommand;

	// creates group socket and bind to port
	Group group(GROUP_IP, GROUP_PORT);
	std::cout << "Listening to port " << GROUP_PORT << " on group IP " << GROUP_IP << "..." << std::endl;

	// asks for leader
	group.sendMessage(Messages::ASK_FOR_LEADER_COMMAND + ",");
	group.receiveMessage();
	std::string groupMessage = group.getMessage();

	std::thread groupThread(handleGroup, std::ref(group), groupCommands, std::ref(sessions), std::ref(notifications));

	if (!groupMessage.empty()) {
		// there's already a leader: I'm a secondary server
		std::string auxServerId, serverIdStr, auxLeaderId;
		if (groupMessage.substr(0, groupMessage.find(",")) == Messages::LEADER_RESPONSE_COMMAND) {
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
			if (groupMessage.substr(0, groupMessage.find(",")) != Messages::LEADER_RESPONSE_COMMAND) {
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

		// send message to group confirming the id
		// if rejected by the group, adjusts the id
		group.sendMessage(Messages::CONFIRM_ID_COMMAND + "," + std::to_string(serverId) + ",");
		group.receiveMessage();
		if (!group.getMessage().empty() && group.getMessage().substr(0, group.getMessage().find(",")) == Messages::CONFIRM_ID_COMMAND) {
			std::cout << "Adjusting serverId..." << std::endl;
			++serverId;
		}
		std::cout << "My id: " << serverId << std::endl;

		serverCount = serverId + 1;
		std::cout << "Number of servers (incluing coordinator): " << serverCount << std::endl;

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
	CloseSessionCommand closeSessionCommand(sessions);
	FollowCommand followCommand(sessions);
	TweetCommand tweetCommand(sessions, tweets, notifications);
	NotificationResponseCommand notificationResponseCommand(sessions, notifications);
	commands[Messages::SESSION_COMMAND] = &sessionCommand;
	commands[Messages::CLOSE_SESSION_COMMAND] = &closeSessionCommand;
	commands[Messages::FOLLOW_COMMAND] = &followCommand;
	commands[Messages::TWEET_COMMAND] = &tweetCommand;
	commands[Messages::NOTIFICATION_RESPONSE_COMMAND] = &notificationResponseCommand;

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

void Application::handleGroup(Group& group, std::map<std::string, Command*> commands, Sessions& sessions, std::map<std::string, std::vector<Notification>>& notifications) {
	std::vector<std::thread> checkThreads;
	
	while (true) {
		std::string commandStr, payloadStr;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		{
			std::lock_guard<std::mutex> connectionGuard(connectionMutex);

			group.receiveMessage();
			std::string message = group.getMessage();

			if (message.empty()) {
				continue;
			}

			commandStr = message.substr(0, message.find(","));
			payloadStr = message.substr(message.find(",") + 1);

			if (commandStr == Messages::ASK_FOR_LEADER_COMMAND) {
				std::string groupResponse = Messages::LEADER_RESPONSE_COMMAND + "," + std::to_string(serverId) + "," + std::to_string(leaderId) + ",";
				group.sendMessage(groupResponse);
				++serverCount;

				std::cout << "Answered election request from group." << std::endl;
				std::cout << "Number of servers: " << serverCount << std::endl;
				continue;
			}
			else if (commandStr == Messages::CONFIRM_ID_COMMAND) {
				int id = std::stoi(payloadStr.substr(0, payloadStr.find(",")));
				if (id == serverId) {
					std::cout << "Server tried initializing with used id. Warning..." << std::endl;
					group.sendMessage(Messages::CONFIRM_ID_COMMAND + "," + std::to_string(serverId) + ",");
				}
				continue;
			}
			else if (commandStr == Messages::LEADER_RESPONSE_COMMAND) {
				int senderId = std::stoi(payloadStr.substr(0, payloadStr.find(",")));
				std::string auxGroupLeader = payloadStr.substr(payloadStr.find(",") + 1);
				leaderId = std::stoi(auxGroupLeader.substr(0, auxGroupLeader.find(",")));
				continue;
			}
			/*else if (commandStr == Messages::REPLICATION_RESPONSE_COMMAND) {
				continue;
			}*/
		}

		// leader shouldn't execute replicated commands
		if (leaderId == serverId) {
			continue;
		}

		/*
		// answers primary server
		std::string replicationResponse = Messages::REPLICATION_RESPONSE_COMMAND + "," + std::to_string(serverId) + "," + message;
		group.sendMessage(replicationResponse);
		*/

		try {
			Connection auxConnection = Connection();
			commands.at(commandStr)->setConnectable(static_cast<Connectable*>(&auxConnection));
			commands.at(commandStr)->setPayload(payloadStr);

			{
				std::lock_guard<std::mutex> commandGuard(mutex); // synchronize access to resources
				commands.at(commandStr)->execute();
			}

			if (commandStr == Messages::TWEET_COMMAND) {
				checkThreads.push_back(std::thread(checkNotificationFailure, std::ref(sessions), std::ref(notifications)));
			}
		}
		catch(const std::out_of_range& e) {
			std::cout << "Invalid group command: " << commandStr << std::endl;
		}
	}

	for (auto& t: checkThreads) {
		t.join();
	}
}

void Application::handleRequest(Connection conn, Group& group, std::map<std::string, Command*> commands, std::string message) {	
	std::string commandStr = message.substr(0, message.find(","));
	std::string payloadStr = message.substr(message.find(",") + 1);

	try {
		commands.at(commandStr)->setConnectable(static_cast<Connectable*>(&conn));
		commands.at(commandStr)->setPayload(payloadStr);

		{ 
			std::lock_guard<std::mutex> connectionGuard(connectionMutex); // synchronize access to connection
			group.sendMessage(message); // replicate message to secondary servers
		}

		/*
		// count replication responses
		std::unordered_set<int> serverIds;
		for (int i = 0; i < 2*serverCount; ++i) {
			group.receiveMessage();
			std::string replicationResponse = group.getMessage();
			if (!replicationResponse.empty() && replicationResponse.substr(0, replicationResponse.find(",")) == Messages::REPLICATION_RESPONSE_COMMAND) {
				std::string replicationPayload = replicationResponse.substr(replicationResponse.find(",") + 1);
				std::string auxMessage = replicationPayload.substr(replicationPayload.find(",") + 1);
				if (auxMessage.find(message) != std::string::npos) {
					serverIds.insert(std::stoi(replicationPayload.substr(0, replicationPayload.find(","))));
				}
			}
		}

		if (serverIds.size() != static_cast<size_t>(serverCount - 1)) {
			for (auto i : serverIds) {
				std::cout << i << " ";
			}
			std::cout << std::endl << "Replication error! Check secondary servers." << std::endl;
		}
		*/

		{ 
			std::lock_guard<std::mutex> commandGuard(mutex); // synchronize access to resources
			commands.at(commandStr)->execute();
		}
	}
	catch(const std::out_of_range& e) {
		std::cout << "Invalid command: " << commandStr << std::endl;
	}

	std::cout << std::endl;
}

void Application::handleNotifications(Sessions& sessions, std::map<std::string, std::vector<Notification>>& notifications) {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		std::lock_guard<std::mutex> notificationGuard(mutex);

		for (auto& entry : notifications) {
			Account account = *accounts.findAccount(entry.first);
			std::pair<Session, Session> activeSessions = sessions.getActiveSessions(account.getUsername());

			if (activeSessions.first.getSessionId() == 0 && activeSessions.second.getSessionId() == 0) {
				continue;
			}

			// consumes pending notification for client
			for (auto it = entry.second.begin(); it < entry.second.end(); ++it) {
				std::string notifyMessage = Messages::NOTIFICATION_RESPONSE_COMMAND + "," + account.getUsername() + "," + std::to_string(it->getTweet().getEpoch()) + "," + it->getAuthor() + "," + std::to_string(it->getTweet().getMessage().length()) + "," + it->getTweet().getMessage() + ",";
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

void Application::checkNotificationFailure(Sessions& sessions, std::map<std::string, std::vector<Notification>>& notifications) {
	std::this_thread::sleep_for(std::chrono::milliseconds(3500));
	std::lock_guard<std::mutex> notificationGuard(mutex);

	for (auto& entry : notifications) {
		Account account = *(sessions.getAccounts().findAccount(entry.first));
		std::pair<Session, Session> activeSessions = sessions.getActiveSessions(account.getUsername());

		if (activeSessions.first.getSessionId() == 0 && activeSessions.second.getSessionId() == 0) {
			continue;
		}
		
		// checks if there is any deliverable notification
		if (!entry.second.empty()) {
			std::cout << "Possible coordinator failure! Elect a new coordinator ASAP!" << std::endl;
			return;
		}
	}

	std::cout << "No coordination problem." << std::endl;
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