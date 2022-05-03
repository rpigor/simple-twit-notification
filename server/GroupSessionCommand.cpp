#include "GroupSessionCommand.hpp"
#include <iostream>

GroupSessionCommand::GroupSessionCommand(Sessions& sessions)
    : Command(sessions) {

}

GroupSessionCommand::GroupSessionCommand(Connectable* connectable, Sessions& sessions, const std::string& payload)
    : Command(connectable, sessions, payload) {

}

void GroupSessionCommand::execute() {
    std::string username = this->payload.substr(0, this->payload.find(","));

    unsigned long sessionId = this->sessions.createSession(*static_cast<Connection*>(this->connectable), username).getSessionId();

    std::cout << "Session [" << sessionId << "] generated to account @" << username << ".\n";
}

std::string GroupSessionCommand::name() const {
    return "Execute replicated session generation";
}