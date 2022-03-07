#pragma once

#include "Account.hpp"
#include <string>
#include <ctime>

class Tweet {
private:
    unsigned long uid;
    Account account;
    std::string message;
    std::time_t timestamp;

public:
    Tweet(const Account&, const std::string&);
    unsigned long getUid() const;
    const Account& getAccount() const;
    const std::string& getMessage() const;
    const std::string& getDateTime() const;
    std::intmax_t getEpoch() const;
};