#include "Notification.hpp"

Notification::Notification(const Account& account, Tweet tweet)
    : author(account), tweet(tweet), pendingCount(0) {

}

bool operator==(const Notification& lhs, const Notification& rhs) {
    return lhs.author == rhs.author && lhs.tweet == rhs.tweet && lhs.pendingCount == rhs.pendingCount;
}

Account Notification::getAuthor() const {
    return this->author;
}

Tweet Notification::getTweet() const {
    return this->tweet;
}

unsigned long Notification::getPendingCount() const {
    return this->pendingCount;
}