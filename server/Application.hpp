#pragma once

#include "Connection.hpp"
#include "Command.hpp"
#include "Notification.hpp"
#include <map>
#include <string>
#include <mutex>

class Application {
private:
    static constexpr unsigned short PORT = 8888;

    static std::mutex mutex;

    static void handleRequest(Connection, std::map<std::string, Command*>, std::string);
    static void handleNotifications(Sessions&, std::map<Account, std::vector<Notification>>&);

public:
    static void run();
};