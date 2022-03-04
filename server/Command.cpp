#include "Command.hpp"

Command::Command(Sessions& sessions)
    : sessions(sessions) {

}

Command::Command(Connection connection, Sessions& sessions, const std::string& payload)
    : connection(connection), sessions(sessions), payload(payload) {

}

void Command::setPayload(const std::string& payload) {
    this->payload = payload;
}

void Command::setConnection(Connection connection) {
    this->connection = connection;
}