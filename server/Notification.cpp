#include "Notification.hpp"

Notification::Notification(const std::string& username, Tweet tweet)
    : author(username), tweet(tweet), pendingCount(0) {

}

bool operator==(const Notification& lhs, const Notification& rhs) {
    return lhs.author == rhs.author && lhs.tweet == rhs.tweet && lhs.pendingCount == rhs.pendingCount;
}

std::string Notification::getAuthor() const {
    return this->author;
}

Tweet Notification::getTweet() const {
    return this->tweet;
}

unsigned long Notification::getPendingCount() const {
    return this->pendingCount;
}