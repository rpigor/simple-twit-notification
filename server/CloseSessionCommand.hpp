#pragma once

#include "Command.hpp"
#include "Sessions.hpp"
#include <string>

class CloseSessionCommand : public Command {
public:
    CloseSessionCommand(Sessions&);
    CloseSessionCommand(Connectable*, Sessions&, const std::string&);
    void execute() override;
    std::string name() const override;
};