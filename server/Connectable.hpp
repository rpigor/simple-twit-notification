#pragma once

#include <string>

class Connectable {
protected:
    std::string message;

public:
    virtual int sendMessage(const std::string&) = 0;
    virtual int receiveMessage() = 0;
    virtual std::string getMessage() const = 0;
};