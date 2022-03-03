#include "Command.hpp"
#include "Sessions.hpp"
#include <string>

class SessionCommand : public Command {
public:
    SessionCommand(Sessions&);
    SessionCommand(Network, Sessions&, const std::string&);
    void execute() const override;
    std::string name() const override;
};