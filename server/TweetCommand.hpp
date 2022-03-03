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
    TweetCommand(Network, Sessions&, std::vector<Tweet>&, const std::string&);
    void execute() const override;
    std::string name() const override;
};