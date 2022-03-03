#include "Server.hpp"
#include <unistd.h>
#include <string.h>

Server::Server(unsigned short port) {
    struct sockaddr_in serverAddr;

    if ((this->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		exit(1);
	}

	memset((char *) &serverAddr, 0, sizeof(serverAddr));
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(this->sock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
		exit(1);
	}
}

Server::~Server() {
    close(this->sock);
}

std::string Server::recvMessage() {
	char buf[BUFLEN];
	int recvLen, clientLen = sizeof(this->clientAddr);

	if ((recvLen = recvfrom(this->sock, buf, BUFLEN, 0, (struct sockaddr*) &this->clientAddr, (socklen_t*) &clientLen)) == -1) {
		exit(1);
	}

	return buf;
}

struct sockaddr_in Server::getClientAddr() const {
	return this->clientAddr;
}

int Server::getSocket() const {
	return this->sock;
}