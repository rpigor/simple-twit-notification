#include "Tweet.hpp"
#include <ctime>

Tweet::Tweet(const std::string& username, const std::string& message)
    : username(username), message(message) {
    this->timestamp = std::time(nullptr);
}

Tweet::Tweet(const std::string& username, const std::string& message, std::time_t timestamp) 
    : username(username), message(message), timestamp(timestamp) {

}

bool operator==(const Tweet& lhs, const Tweet& rhs) {
    return lhs.username == rhs.username && lhs.message == rhs.message && lhs.timestamp == rhs.timestamp;
}

unsigned long Tweet::getUid() const {
    return this->uid;
}

std::string Tweet::getUsername() const {
    return this->username;
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