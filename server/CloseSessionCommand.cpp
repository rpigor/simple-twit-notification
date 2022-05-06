#include "CloseSessionCommand.hpp"
#include "Messages.hpp"
#include <iostream>

CloseSessionCommand::CloseSessionCommand(Sessions& sessions)
    : Command(sessions) {

}

CloseSessionCommand::CloseSessionCommand(Connectable* connectable, Sessions& sessions, const std::string& payload)
    : Command(connectable, sessions, payload) {

}

void CloseSessionCommand::execute() {
    std::string username = this->payload.substr(0, this->payload.find(","));
    std::string auxSessionId = this->payload.substr(this->payload.find(",") + 1);
    int sessionId = std::stoi(auxSessionId.substr(0, auxSessionId.find(",")));

    this->sessions.deleteSession(username, sessionId);

    std::cout << "Closed session [" << sessionId << "] to account @" << username << ".\n";
}

std::string CloseSessionCommand::name() const {
    return "Closes session";
}