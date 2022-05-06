#pragma once

#include "Client.hpp"
#include <string>
#include <mutex>

class Application {

private:
    static constexpr int CHAR_LIMIT = 128;

    static std::string profile;
    static unsigned long session;
    static Client* client;

    static void handleCommands();
    static void handleNotifications();
    static void initializeExitSignals();
    static void sendExitMessageAndExit(int);
    static void printCommands();

public:
    static void run(const std::string&, unsigned short);
    static void setProfile(const std::string&);
};
