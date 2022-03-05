#pragma once

#include <string>
#include <arpa/inet.h>

class Client{
private:
    int sock;

public:
    Client(unsigned short);
    ~Client();

    int sendMessage(const std::string&);
};
