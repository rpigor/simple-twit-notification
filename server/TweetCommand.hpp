#pragma once

#include "Command.hpp"
#include "Sessions.hpp"
#include "Tweet.hpp"
#include <vector>
#include <string>

class TweetCommand : public Command {
private:
    std::vector<Tweet>& tweets;

public:
    TweetCommand(Sessions&, std::vector<Tweet>&);
    TweetCommand(Connection, Sessions&, std::vector<Tweet>&, const std::string&);
    void execute() override;
    std::string name() const override;
};