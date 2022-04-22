#pragma once

#include "Connection.hpp"
#include "Command.hpp"
#include "Notification.hpp"
#include "Account.hpp"
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <mutex>

class Application {
private:
    static constexpr unsigned short PORT = 8888;
    static constexpr const char* accountsFilename = "backup.txt";

    static std::vector<std::shared_ptr<Account>> accounts;
    static std::mutex mutex;

    static void handleRequest(Connection, std::map<std::string, Command*>, std::string);
    static void handleNotifications(Sessions&, std::map<std::string, std::vector<Notification>>&);
    static void backupAccounts(int i);
    static void recoverAccounts();

public:
    static void run();
};