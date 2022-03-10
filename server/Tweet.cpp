#include "Tweet.hpp"
#include <ctime>

Tweet::Tweet(const Account& account, const std::string& message)
    : account(account), message(message) {
    this->timestamp = std::time(nullptr);
}

Tweet::Tweet(const Account& account, const std::string& message, std::time_t timestamp) 
    : account(account), message(message), timestamp(timestamp) {

}

bool operator==(const Tweet& lhs, const Tweet& rhs) {
    return lhs.account == rhs.account && lhs.message == rhs.message && lhs.timestamp == rhs.timestamp;
}

unsigned long Tweet::getUid() const {
    return this->uid;
}

Account Tweet::getAccount() const {
    return this->account;
}

std::string Tweet::getMessage() const {
    return this->message;
}

std::string Tweet::getDateTime() const {
    return std::ctime(&this->timestamp);
}

std::intmax_t Tweet::getEpoch() const {
    return (std::intmax_t) this->timestamp;
}