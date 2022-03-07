#pragma once

#include "Connection.hpp"

class Session {
private:
    unsigned long sessionId;
    Connection clientConnection;

public:
    Session();
    Session(unsigned long, Connection);
    unsigned long getSessionId() const;
    void setSessionId(unsigned long);
    Connection getClientConnection() const;
};