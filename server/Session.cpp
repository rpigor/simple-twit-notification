#include "Session.hpp"

Session::Session() {
    
}

Session::Session(unsigned long sessionId, Connection clientConnection)
    : sessionId(sessionId), clientConnection(clientConnection) {

}

unsigned long Session::getSessionId() const {
    return this->sessionId;
}

void Session::setSessionId(unsigned long sessionId) {
    this->sessionId = sessionId;
}

Connection Session::getClientConnection() const {
    return this->clientConnection;
}