#pragma once

#include "Client.hpp"
#include <string>

class Application {
private:
    static void printCommands();

public:
    static void run(const std::string&, const std::string&, unsigned short);
};
