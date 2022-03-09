#pragma once

#include "Command.hpp"
#include "Sessions.hpp"
#include "Notification.hpp"

class NotificationResponseCommand : public Command {
private:
    std::map<Account, std::vector<Notification>>& notifications;

public:
    NotificationResponseCommand(Sessions&, std::map<Account, std::vector<Notification>>&);
    NotificationResponseCommand(Connection, Sessions&, std::map<Account, std::vector<Notification>>&, const std::string&);
    void execute() override;
    std::string name() const override;
};