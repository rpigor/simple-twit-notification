#pragma once

#include "Command.hpp"
#include "Sessions.hpp"
#include <string>

class GroupSessionCommand : public Command {
public:
    GroupSessionCommand(Sessions&);
    GroupSessionCommand(Connectable*, Sessions&, const std::string&);
    void execute() override;
    std::string name() const override;
};