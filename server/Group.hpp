#pragma once

#include "Connectable.hpp"
#include "Connection.hpp"
#include <string>
#include <arpa/inet.h>

class Group : public Connectable {
private:
    static constexpr int BUFLEN = 512;
    static constexpr int TIMEOUT_SEC = 1;

    int groupSock;
    int clientSock;
    struct sockaddr_in groupAddr;
    struct sockaddr_in clientAddr;

    void initGroup(const std::string&, unsigned short);
    void initClient(const std::string&, unsigned short);

public:
    Group();
    Group(const std::string&, unsigned short);
    ~Group();
    int sendMessage(const std::string&) override;
    int receiveMessage() override;
    std::string getMessage() const;
    Connection getGroupConnection() const;
    Connection getClientConnection() const;
};