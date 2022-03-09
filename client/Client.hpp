#pragma once

#include <string>
#include <mutex>
#include <arpa/inet.h>

class Client {
private:
    static constexpr int BUFLEN = 512;
    static std::mutex mutex;

    int sock;
    struct sockaddr_in serverAddr;
    std::string message;

public:
    Client(const std::string&, unsigned short);
    ~Client();
    int sendMessage(const std::string&);
    int receiveMessage();
    int nonBlockingReceiveMessage();
    std::string getMessage() const;
};
