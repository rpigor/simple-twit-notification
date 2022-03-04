#pragma once

#include <string>
#include <arpa/inet.h>

class Connection {
private:
    static constexpr int BUFLEN = 512;

    int connSocket;
    struct sockaddr_in connAddr;
    std::string message;

public:
    Connection();
    Connection(int, struct sockaddr_in);
    int sendMessage(const std::string&);
    int receiveMessage();
    void closeConnection();
    std::string getMessage() const;
    std::string getAddress() const;
    int getPort() const;
};