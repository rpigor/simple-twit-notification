#include "Account.hpp"
#include <algorithm>

Account::Account(const std::string& username)
    : username(username) {
    
}

bool operator==(const Account& lhs, const Account& rhs) {
    return lhs.username == rhs.username;
}

bool operator<(const Account& lhs, const Account& rhs) {
    return lhs.username < rhs.username;
}

unsigned long Account::getUid() const {
    return this->uid;
}

const std::string& Account::getUsername() const {
    return this->username;
}

const std::vector<Account>& Account::getFollowing() const {
    return this->following;
}

const std::vector<Account>& Account::getFollowers() const {
    return this->followers;
}

void Account::follow(const Account& account) {
    if (std::find(this->following.begin(), this->following.end(), account) != this->following.end()) {
        return;
    }

    this->following.push_back(account);
}

void Account::beFollowedBy(const Account& account) {
    if (std::find(this->followers.begin(), this->followers.end(), account) != this->followers.end()) {
        return;
    }

    this->followers.push_back(account);
}