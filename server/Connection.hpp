#pragma once

#include "Connectable.hpp"
#include <string>
#include <arpa/inet.h>

class Connection : public Connectable {
private:
    static constexpr int BUFLEN = 512;

    int connSocket;
    struct sockaddr_in connAddr;

public:
    Connection();
    Connection(int, struct sockaddr_in);
    int sendMessage(const std::string&) override;
    int receiveMessage() override;
    std::string getMessage() const;
    std::string getAddress() const;
    int getPort() const;
};