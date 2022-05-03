#pragma once

#include "Connectable.hpp"
#include "Sessions.hpp"
#include <string>

class Command {
protected:
    Connectable* connectable;
    Sessions& sessions;
    std::string payload;

public:
    Command(Sessions&);
    Command(Connectable*, Sessions&, const std::string&);
    virtual void execute() = 0;
    virtual std::string name() const = 0;
    void setPayload(const std::string&);
    void setConnectable(Connectable*);
};