#pragma once

#include "Sessions.hpp"
#include <string>
#include <arpa/inet.h>

struct Network {
    int socket;
    struct sockaddr_in clientAddr;

    Network() {
        
    }

    Network(int socket, struct sockaddr_in clientAddr)
        : socket(socket), clientAddr(clientAddr) {
        
    }
};

class Command {
protected:
    Network network;
    Sessions& sessions;
    std::string payload;

public:
    Command(Sessions&);
    Command(Network, Sessions&, const std::string&);
    virtual void execute() const = 0;
    virtual std::string name() const = 0;
    void setPayload(const std::string&);
    void setNetwork(Network);
};