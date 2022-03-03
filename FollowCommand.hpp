#include "Command.hpp"
#include "Sessions.hpp"
#include <string>

class FollowCommand : public Command {
public:
    FollowCommand(Sessions&);
    FollowCommand(Network, Sessions&, const std::string&);
    void execute() const override;
    std::string name() const override;
};