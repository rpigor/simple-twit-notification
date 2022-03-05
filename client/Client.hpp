#pragma once

#include <string>
#include <arpa/inet.h>

class Client{
private:
    static constexpr int BUFLEN = 512;

    int sock;
    std::string message;

public:
    Client(const std::string&, unsigned short);
    ~Client();
    int sendMessage(const std::string&);
    int receiveMessage();
    std::string getMessage() const;
};
