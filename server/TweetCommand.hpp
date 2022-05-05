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
    static constexpr int CHAR_LIMIT = 128;

    std::vector<Tweet>& tweets;
    std::map<std::string, std::vector<Notification>>& notifications;

public:
    TweetCommand(Sessions&, std::vector<Tweet>&, std::map<std::string, std::vector<Notification>>&);
    TweetCommand(Connectable*, Sessions&, std::vector<Tweet>&, std::map<std::string, std::vector<Notification>>&, const std::string&);
    void execute() override;
    std::string name() const override;
};