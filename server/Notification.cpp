#include "Notification.hpp"

Notification::Notification(const Account& account, Tweet tweet)
    : author(account), tweet(tweet) {

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