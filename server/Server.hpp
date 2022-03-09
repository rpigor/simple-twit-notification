#pragma once

#include "Connection.hpp"
#include "SessionCommand.hpp"
#include <string>
#include <arpa/inet.h>

class Server {
private:
    static constexpr int BUFLEN = 512;

    int sock;
    struct sockaddr_in clientAddr;

public:
    Server(unsigned short);
    ~Server();
    std::string receiveMessage();
    Connection getConnection();
};