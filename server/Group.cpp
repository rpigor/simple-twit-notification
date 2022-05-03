#include "Group.hpp"
#include <cstring>
#include <unistd.h>
#include <netdb.h>

Group::Group() {
    
}

Group::Group(const std::string& group, unsigned short port) {
    initGroup(group, port);
    initClient(group, port);
}

Group::~Group() {
    close(this->groupSock);
    close(this->clientSock);
}

int Group::sendMessage(const std::string& message) {
    int len = sendto(this->clientSock, message.c_str(), message.length(), 0, (const sockaddr*) &this->clientAddr, sizeof(this->clientAddr));

    // sets timeout and reads own message to prevent loopback
    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);
    FD_SET(this->groupSock, &set);

    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;

    if (select(this->groupSock + 1, &set, NULL, NULL, &timeout) == 0) {
        return 0;
    }

    char buffer[BUFLEN];
    int addrLen = sizeof(this->groupAddr);
	if (recvfrom(this->groupSock, buffer, BUFLEN, 0, (sockaddr*) &this->groupAddr, (socklen_t*) &addrLen) < 0) {
		perror("recvfrom");
		exit(1);
	}

    return len;
}

int Group::receiveMessage() {
	char buffer[BUFLEN];

    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);
    FD_SET(this->groupSock, &set);

    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;

    if (select(this->groupSock + 1, &set, NULL, NULL, &timeout) == 0) {
        this->message = "";
        return 0;
    }

	int addrLen = sizeof(this->groupAddr);
    int recvLen = recvfrom(this->groupSock, buffer, BUFLEN, 0, (sockaddr*) &this->groupAddr, (socklen_t*) &addrLen);
	if (recvLen < 0) {
		perror("recvfrom");
		exit(1);
	}

    this->message = buffer;

	return recvLen;
}

Connection Group::getGroupConnection() const {
    return Connection(this->groupSock, this->groupAddr);
}

Connection Group::getClientConnection() const {
	return Connection(this->clientSock, this->clientAddr);
}

void Group::initGroup(const std::string& group, unsigned short port) {
    if ((this->groupSock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

    u_int yes = 1;
    if (setsockopt(this->groupSock, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) == -1) {
       perror("setsockopt -- reusing addr");
       exit(1);
    }

    memset(&this->groupAddr, 0, sizeof(this->groupAddr));
	this->groupAddr.sin_family = AF_INET;
	this->groupAddr.sin_port = htons(port);
	this->groupAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(this->groupSock, (struct sockaddr*) &this->groupAddr, sizeof(this->groupAddr)) == -1) {
		perror("bind");
		exit(1);
	}

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group.c_str());
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(this->groupSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) == -1) {
        perror("setsockopt -- enter group");
        exit(1);
    }
}

void Group::initClient(const std::string& group, unsigned short port) {
    if ((this->clientSock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

    memset(&this->clientAddr, 0, sizeof(this->clientAddr));
	this->clientAddr.sin_family = AF_INET;
	this->clientAddr.sin_port = htons(port);
    this->clientAddr.sin_addr.s_addr = inet_addr(group.c_str());
}

std::string Group::getMessage() const {
    return this->message;
}