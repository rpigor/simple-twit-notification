#include "Command.hpp"

Command::Command(Sessions& sessions)
    : sessions(sessions) {

}

Command::Command(Connectable* connectable, Sessions& sessions, const std::string& payload)
    : connectable(connectable), sessions(sessions), payload(payload) {

}

void Command::setPayload(const std::string& payload) {
    this->payload = payload;
}

void Command::setConnectable(Connectable* connectable) {
    this->connectable = connectable;
}