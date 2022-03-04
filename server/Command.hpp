#pragma once

#include "Connection.hpp"
#include "Sessions.hpp"
#include <string>

class Command {
protected:
    Connection connection;
    Sessions& sessions;
    std::string payload;

public:
    Command(Sessions&);
    Command(Connection, Sessions&, const std::string&);
    virtual void execute() = 0;
    virtual std::string name() const = 0;
    void setPayload(const std::string&);
    void setConnection(Connection);
};