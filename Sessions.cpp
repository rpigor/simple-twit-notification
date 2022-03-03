#include "Sessions.hpp"
#include <algorithm>

Sessions::Sessions(std::vector<Account>& accounts) : accounts(accounts), sessionCount(0) {

}

unsigned long Sessions::createSession(const Account& account) {
    if (!accountExists(account)) {
        return 0;
    }

    if (this->sessions.find(account) == this->sessions.end()) {
        ++this->sessionCount;
        this->sessions[account] = std::make_pair(this->sessionCount, 0);
        return this->sessionCount;
    }

    ++this->sessionCount;
    if (this->sessions.at(account).second < this->sessions.at(account).first) {
        this->sessions[account].second = this->sessionCount;
    }
    else {
        this->sessions[account].first = this->sessionCount;
    }

    return this->sessionCount;
}

unsigned long Sessions::createSession(const std::string& username) {
    if (!accountExists(username)) {
        return 0; // throw exception
    }

    if (this->sessions.find(username) == this->sessions.end()) {
        ++this->sessionCount;
        this->sessions[username] = std::make_pair(this->sessionCount, 0);
        return this->sessionCount;
    }

    ++this->sessionCount;
    if (this->sessions.at(username).second < this->sessions.at(username).first) {
        this->sessions[username].second = this->sessionCount;
    }
    else {
        this->sessions[username].first = this->sessionCount;
    }

    return this->sessionCount;
}

std::vector<Account>& Sessions::getAccounts() const {
    return this->accounts;
}

bool Sessions::accountExists(const Account& account) {
    return std::find(this->accounts.begin(), this->accounts.end(), account) != this->accounts.end();
}

void Sessions::deleteSessions(const Account& account, unsigned long session) { 
    if (!hasSession(account)) {
        return;
    }

    if (this->sessions.at(account).first != session && this->sessions.at(account).second != session) {
        return;
    }

    if (this->sessions.at(account).first == session) {
        this->sessions[account].first = 0;
    }
    else {
        this->sessions[account].second = 0;
    }
}

void Sessions::deleteSessions(const std::string& username, unsigned long session) { 
    if (!hasSession(username)) {
        return;
    }

    if (this->sessions.at(username).first != session && this->sessions.at(username).second != session) {
        return;
    }

    if (this->sessions.at(username).first == session) {
        this->sessions[username].first = 0;
    }
    else {
        this->sessions[username].second = 0;
    }
}

bool Sessions::hasSession(const Account& account) const {
    return this->sessions.find(account) != this->sessions.end();
}

bool Sessions::hasSession(const std::string& username) const {
    return this->sessions.find(username) != this->sessions.end();
}

bool Sessions::hasSession(const Account& account, unsigned long session) const {
    if (!hasSession(account)) {
        return false;
    }

    return this->sessions.find(account)->second.first == session || this->sessions.find(account)->second.second == session;
}

bool Sessions::hasSession(const std::string& username, unsigned long session) const {
    if (!hasSession(username)) {
        return false;
    }

    return this->sessions.find(username)->second.first == session || this->sessions.find(username)->second.second == session;
}

std::pair<unsigned long, unsigned long> Sessions::getActiveSessions(const Account& account) const {
    return this->sessions.at(account);
}

std::pair<unsigned long, unsigned long> Sessions::getActiveSessions(const std::string& username) const {
    return this->sessions.at(username);
}