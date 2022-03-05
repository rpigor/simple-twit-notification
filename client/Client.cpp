#include "Client.hpp"
#include <iostream>
#include <unistd.h>
#include <string.h>

Client::Client(const std::string& ip, unsigned short port) {
    struct sockaddr_in serverAddr;

	if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("a");
		exit(1);
	}

	memset((char *) &serverAddr, 0, sizeof(serverAddr));
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	
	std::cout << "[INFO] Waiting for server to accept connection..." << std::endl;

    if (connect(this->sock,(struct sockaddr *) &serverAddr,sizeof(serverAddr)) < 0) {
		perror("b");
		exit(1);
	}

	std::cout << "[SUCCESS] Connected to server!" << std::endl;
}

Client::~Client() {
    close(this->sock);
}

int Client::sendMessage(const std::string& message){
    return send(this->sock, message.c_str(), message.length(), 0);
}

int Client::receiveMessage() {
	char buffer[BUFLEN];
	int recvLen;

	recvLen = recv(this->sock, buffer, BUFLEN, 0);
    this->message = buffer;

	return recvLen;
}

std::string Client::getMessage() const {
	return this->message;
}