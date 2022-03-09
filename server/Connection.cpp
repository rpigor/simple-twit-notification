#include "Connection.hpp"
#include <unistd.h>

Connection::Connection() {
    
}

Connection::Connection(int connSocket, struct sockaddr_in connAddr)
    :  connSocket(connSocket), connAddr(connAddr) {

}

int Connection::sendMessage(const std::string& message) {
    return sendto(this->connSocket, message.c_str(), message.length(), 0, (sockaddr*) &this->connAddr, sizeof(this->connAddr));
}

int Connection::receiveMessage() {
	char buffer[BUFLEN];
	int recvLen, addrLen = sizeof(this->connAddr);

	recvLen = recvfrom(this->connSocket, buffer, BUFLEN, 0, (sockaddr*) &this->connAddr, (socklen_t*) &addrLen);
    this->message = buffer;

	return recvLen;
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