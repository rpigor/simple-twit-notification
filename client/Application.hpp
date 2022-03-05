#pragma once

#include <string>

class Application {
private:
    static constexpr unsigned short PORT = 8888;
    
public:
    static void run(std::string, std::string, unsigned short);
};
