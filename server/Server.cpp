#include "Server.hpp"
#include <unistd.h>
#include <string.h>

Server::Server(unsigned short port) {
    struct sockaddr_in serverAddr;

	if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("a");
		exit(1);
	}

	memset((char *) &serverAddr, 0, sizeof(serverAddr));
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(this->sock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
		perror("b");
		exit(1);
	}

	if (listen(this->sock, SOMAXCONN) < 0) {
		perror("c");
		exit(1);
	}
}

Server::~Server() {
    close(this->sock);
}

Connection Server::acceptClientSocket() {
	struct sockaddr_in clientAddr;
	int clientSocket, clientLen = sizeof(clientAddr);

	if ((clientSocket = accept(this->sock, (struct sockaddr*) &clientAddr, (socklen_t*) &clientLen)) == -1) {
		perror("1");
		exit(1);
	}

	return Connection(clientSocket, clientAddr);
}