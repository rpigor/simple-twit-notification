#pragma once

#include "Account.hpp"
#include <string>
#include <ctime>

class Tweet {
private:
    unsigned long uid;
    std::string username;
    std::string message;
    std::time_t timestamp;

public:
    Tweet(const std::string&, const std::string&);
    Tweet(const std::string&, const std::string&, std::time_t);
    friend bool operator==(const Tweet&, const Tweet&);
    unsigned long getUid() const;
    std::string getUsername() const;
    std::string getMessage() const;
    std::string getDateTime() const;
    std::intmax_t getEpoch() const;
};