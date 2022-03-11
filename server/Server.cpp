#include "Server.hpp"
#include <unistd.h>
#include <string.h>

Server::Server(unsigned short port) {
    struct sockaddr_in serverAddr;

	if ((this->sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("a");
		exit(1);
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bzero(&(serverAddr.sin_zero), 8);
	
	if (bind(this->sock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
		perror("b");
		exit(1);
	}
}

Server::~Server() {
    close(this->sock);
}

std::string Server::receiveMessage() {
	char buffer[BUFLEN];
	int addrLen = sizeof(this->clientAddr);

	if (recvfrom(this->sock, buffer, BUFLEN, 0, (sockaddr*) &this->clientAddr, (socklen_t*) &addrLen) < 0) {
		perror("c");
		exit(1);
	}

	return buffer;
}

Connection Server::getConnection() {
	return Connection(this->sock, this->clientAddr);
}