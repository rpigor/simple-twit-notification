#pragma once

#include "Command.hpp"
#include "Sessions.hpp"
#include "Notification.hpp"
#include <string>
#include <map>
#include <vector>

class NotificationResponseCommand : public Command {
private:
    std::map<std::string, std::vector<Notification>>& notifications;

public:
    NotificationResponseCommand(Sessions&, std::map<std::string, std::vector<Notification>>&);
    NotificationResponseCommand(Connection, Sessions&, std::map<std::string, std::vector<Notification>>&, const std::string&);
    void execute() override;
    std::string name() const override;
};