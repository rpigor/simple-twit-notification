#include "SessionCommand.hpp"
#include <iostream>

SessionCommand::SessionCommand(Sessions& sessions)
    : Command(sessions) {

}

SessionCommand::SessionCommand(Connection connection, Sessions& sessions, const std::string& payload)
    : Command(connection, sessions, payload) {

}

void SessionCommand::execute() {
    std::string username = this->payload.substr(0, this->payload.find(","));

    unsigned long sessionId = this->sessions.createSession(this->connection, username).getSessionId();

    std::cout << "Session [" << sessionId << "] generated to account @" << username << ".\n";

    std::string sessionMsg = "sessao," + std::to_string(sessionId) + ",";
    this->connection.sendMessage(sessionMsg);
}

std::string SessionCommand::name() const {
    return "Generates session";
}