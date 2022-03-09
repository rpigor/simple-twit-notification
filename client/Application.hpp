#pragma once

#include "Client.hpp"
#include <string>
#include <mutex>

class Application {
private:
    static void handleCommands(Client& client, const std::string& profile, const std::string& session);
    static void handleNotifications(Client& client);
    static void printCommands();

public:
    static void run(const std::string&, const std::string&, unsigned short);
};
