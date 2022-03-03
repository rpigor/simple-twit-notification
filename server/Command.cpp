#include "Command.hpp"

Command::Command(Sessions& sessions)
    : sessions(sessions) {

}

Command::Command(Network network, Sessions& sessions, const std::string& payload)
    : network(network), sessions(sessions), payload(payload) {

}

void Command::setPayload(const std::string& payload) {
    this->payload = payload;
}

void Command::setNetwork(Network network) {
    this->network = network;
}