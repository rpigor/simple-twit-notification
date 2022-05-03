#pragma once

#include "Command.hpp"
#include "Sessions.hpp"
#include <string>

class GroupFollowCommand : public Command {
public:
    GroupFollowCommand(Sessions&);
    GroupFollowCommand(Connectable*, Sessions&, const std::string&);
    void execute() override;
    std::string name() const override;
};