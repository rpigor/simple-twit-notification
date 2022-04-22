#pragma once

#include "Account.hpp"
#include "Tweet.hpp"
#include <string>

class Notification {
private:
    std::string author;
    Tweet tweet;
    unsigned long pendingCount;

public:
    Notification(const std::string&, Tweet);
    friend bool operator==(const Notification&, const Notification&);
    std::string getAuthor() const;
    Tweet getTweet() const;
    unsigned long getPendingCount() const;
};