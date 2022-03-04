#pragma once

#include "Connection.hpp"
#include <string>
#include <arpa/inet.h>

class Server {
private:
    int sock;

public:
    Server(unsigned short);
    ~Server();
    Connection acceptClientSocket();
};