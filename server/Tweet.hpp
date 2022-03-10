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
    Tweet(const Account&, const std::string&, std::time_t);
    friend bool operator==(const Tweet&, const Tweet&);
    unsigned long getUid() const;
    Account getAccount() const;
    std::string getMessage() const;
    std::string getDateTime() const;
    std::intmax_t getEpoch() const;
};