#include "Tweet.hpp"
#include <ctime>

Tweet::Tweet(const Account& account, const std::string& message)
    : account(account), message(message) {
    this->timestamp = std::time(nullptr);
}

unsigned long Tweet::getUid() const {
    return this->uid;
}

const Account& Tweet::getAccount() const {
    return this->account;
}

const std::string& Tweet::getMessage() const {
    return this->message;
}

const std::string& Tweet::getDateTime() const {
    return std::ctime(&this->timestamp);
}

std::intmax_t Tweet::getEpoch() const {
    return (std::intmax_t) this->timestamp;
}