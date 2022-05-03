#pragma once

#include "Connection.hpp"
#include "Command.hpp"
#include "Notification.hpp"
#include "Account.hpp"
#include "Accounts.hpp"
#include "Group.hpp"
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <mutex>

class Application {
private:
    static constexpr const char* GROUP_IP = "228.8.8.8";
    static constexpr unsigned short SERVER_PORT = 8888;
    static constexpr unsigned short GROUP_PORT = 8889;
    static constexpr const char* ACCOUNTS_FILENAME = "backup.txt";

    static Accounts accounts;
    static int leaderId;
    static int serverId;
    static std::mutex mutex;
    static std::mutex connectionMutex;

    static void handleGroup(Group&, std::map<std::string, Command*>);
    static void handleRequest(Connection, Group&, std::map<std::string, Command*>, std::string);
    static void handleNotifications(Sessions&, std::map<std::string, std::vector<Notification>>&);
    static void handleAccountsInitialization();
    static void backupAccounts(int i);
    static void recoverAccounts();

public:
    static void run();
};