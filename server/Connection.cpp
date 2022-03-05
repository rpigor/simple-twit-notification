#include "Connection.hpp"
#include <unistd.h>

Connection::Connection() {
    
}

Connection::Connection(int connSocket, struct sockaddr_in connAddr)
    :  connSocket(connSocket), connAddr(connAddr) {

}

int Connection::sendMessage(const std::string& message) {
    return send(this->connSocket, message.c_str(), message.length(), 0);
}

int Connection::receiveMessage() {
	char buffer[BUFLEN];
	int recvLen;

	recvLen = recv(this->connSocket, buffer, BUFLEN, 0);
    this->message = buffer;

	return recvLen;
}

void Connection::closeConnection() {
    close(this->connSocket);
}

std::string Connection::getMessage() const {
    return this->message;
}

std::string Connection::getAddress() const {
    return inet_ntoa(this->connAddr.sin_addr);
}

int Connection::getPort() const {
    return ntohs(this->connAddr.sin_port);
}