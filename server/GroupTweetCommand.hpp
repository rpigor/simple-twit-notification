#pragma once

#include "Command.hpp"
#include "Sessions.hpp"
#include "Account.hpp"
#include "Notification.hpp"
#include "Tweet.hpp"
#include <vector>
#include <map>
#include <string>

class GroupTweetCommand : public Command {
private:
    std::vector<Tweet>& tweets;
    std::map<std::string, std::vector<Notification>>& notifications;

public:
    GroupTweetCommand(Sessions&, std::vector<Tweet>&, std::map<std::string, std::vector<Notification>>&);
    GroupTweetCommand(Connectable*, Sessions&, std::vector<Tweet>&, std::map<std::string, std::vector<Notification>>&, const std::string&);
    void execute() override;
    std::string name() const override;
};