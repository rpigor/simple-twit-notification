#pragma once

#include "Server.hpp"
#include "Command.hpp"
#include <map>
#include <string>

class Application {
private:
    static constexpr unsigned short PORT = 8888;

    static void handleClient(Connection, std::map<std::string, Command*>);

public:
    static void run();
};