#include "SessionCommand.hpp"
#include <iostream>

SessionCommand::SessionCommand(Sessions& sessions)
    : Command(sessions) {

}

SessionCommand::SessionCommand(Network network, Sessions& sessions, const std::string& payload)
    : Command(network, sessions, payload) {

}

void SessionCommand::execute() const {
    std::string username = this->payload.substr(0, this->payload.find(","));

    unsigned long sessionId = this->sessions.createSession(username);

    std::cout << "Session [" << sessionId << "] generated to account @" << username << ".\n";

    std::string sessionMsg = "sessao," + std::to_string(sessionId) + "\n";
    if (sendto(this->network.socket, sessionMsg.c_str(), sessionMsg.length(), 0, (struct sockaddr*) &this->network.clientAddr, sizeof(this->network.clientAddr)) == -1) {
        perror("sendto()");
        exit(1);
    }
}

std::string SessionCommand::name() const {
    return "Gera sessão";
}