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
        Account auxAcc;
        is >> auxAcc;
        acc.following.push_back(auxAcc);
    }

    int followerLen;
    is >> followerLen;
    for (int i = 0; i < followerLen; ++i) {
        Account auxAcc;
        is >> auxAcc;
        acc.followers.push_back(auxAcc);
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