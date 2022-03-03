#include "Tweet.hpp"

Tweet::Tweet(const Account& account, const std::string& message)
    : account(account), message(message) {

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