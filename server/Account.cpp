#include "Account.hpp"
#include <algorithm>

Account::Account() {

}

Account::Account(const std::string& username)
    : username(username) {
    
}

bool operator==(const Account& lhs, const Account& rhs) {
    return lhs.username == rhs.username;
}

bool operator<(const Account& lhs, const Account& rhs) {
    return lhs.username < rhs.username;
}

std::istream& operator>>(std::istream& is, Account& acc) {
    is >> acc.username;

    int followingLen;
    is >> followingLen;
    for (int i = 0; i < followingLen; ++i) {
        std::string auxUsername;
        is >> auxUsername;
        acc.following.push_back(auxUsername);
    }

    int followerLen;
    is >> followerLen;
    for (int i = 0; i < followerLen; ++i) {
        std::string auxUsername;
        is >> auxUsername;
        acc.followers.push_back(auxUsername);
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Account& acc) {
    os << acc.username << " " << acc.getFollowing().size() << " ";

    for (auto it = acc.getFollowing().begin(); it < acc.getFollowing().end(); ++it) {
        os << *it << " ";
    }

    int followersLen = acc.getFollowers().size();
    os << followersLen;
    if (followersLen != 0) {
        os << " ";
    }

    for (auto it = acc.getFollowers().begin(); it < acc.getFollowers().end(); ++it) {
        os << *it;
        if (it != acc.getFollowers().end() - 1) {
            os << " ";
        }
    }

    return os;
}

unsigned long Account::getUid() const {
    return this->uid;
}

std::string Account::getUsername() const {
    return this->username;
}

const std::vector<std::string>& Account::getFollowing() const {
    return this->following;
}

const std::vector<std::string>& Account::getFollowers() const {
    return this->followers;
}

void Account::follow(const std::string& username) {
    if (std::find(this->following.begin(), this->following.end(), username) != this->following.end()) {
        return;
    }

    this->following.push_back(username);
}

void Account::beFollowedBy(const std::string& username) {
    if (std::find(this->followers.begin(), this->followers.end(), username) != this->followers.end()) {
        return;
    }

    this->followers.push_back(username);
}