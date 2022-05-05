#include "SessionCommand.hpp"
#include "Messages.hpp"
#include <iostream>

SessionCommand::SessionCommand(Sessions& sessions)
    : Command(sessions) {

}

SessionCommand::SessionCommand(Connectable* connectable, Sessions& sessions, const std::string& payload)
    : Command(connectable, sessions, payload) {

}

void SessionCommand::execute() {
    std::string username = this->payload.substr(0, this->payload.find(","));

    unsigned long sessionId = this->sessions.createSession(*static_cast<Connection*>(this->connectable), username).getSessionId();

    std::cout << "Session [" << sessionId << "] generated to account @" << username << ".\n";

    std::string sessionMsg = Messages::SESSION_COMMAND + "," + std::to_string(sessionId) + ",";
    this->connectable->sendMessage(sessionMsg);
}

std::string SessionCommand::name() const {
    return "Generates session";
}