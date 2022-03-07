#pragma once

#include "Account.hpp"
#include "Tweet.hpp"

class Notification {
private:
    Account author;
    Tweet tweet;
    unsigned long pendingCount;

public:
    Notification(const Account&, Tweet);
    Account getAuthor() const;
    Tweet getTweet() const;
    unsigned long getPendingCount() const;
};