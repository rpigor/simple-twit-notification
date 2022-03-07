#pragma once

#include "Command.hpp"
#include "Sessions.hpp"
#include "Account.hpp"
#include "Notification.hpp"
#include "Tweet.hpp"
#include <vector>
#include <map>
#include <string>

class TweetCommand : public Command {
private:
    std::vector<Tweet>& tweets;
    std::map<Account, std::vector<Notification>>& notifications;

public:
    TweetCommand(Sessions&, std::vector<Tweet>&, std::map<Account, std::vector<Notification>>&);
    TweetCommand(Connection, Sessions&, std::vector<Tweet>&, std::map<Account, std::vector<Notification>>&, const std::string&);
    void execute() override;
    std::string name() const override;
};