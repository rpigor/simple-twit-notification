#include "Sessions.hpp"
#include <algorithm>
#include <limits>

Sessions::Sessions(std::vector<std::shared_ptr<Account>>& accounts)
    : accounts(accounts), sessionCount(0) {

}

Session Sessions::createSession(Connection connection, const std::string& username) {
    Session invalidSession(0, connection);

    if (!accountExists(username)) {
        this->accounts.push_back(std::make_shared<Account>(username));
    }

    if (this->sessions.find(username) == this->sessions.end()) {
        this->sessionCount = (this->sessionCount + 1) % std::numeric_limits<unsigned long>::max();

        Session validSession(this->sessionCount, connection);
        this->sessions[username] = std::make_pair(validSession, invalidSession);
        return validSession;
    }

    this->sessionCount = (this->sessionCount + 1) % std::numeric_limits<unsigned long>::max();

    Session validSession(this->sessionCount, connection);
    if (this->sessions.at(username).second.getSessionId() < this->sessions.at(username).first.getSessionId()) {
        this->sessions[username].second = validSession;
    }
    else {
        this->sessions[username].first = validSession;
    }

    return validSession;
}

std::vector<std::shared_ptr<Account>>& Sessions::getAccounts() const {
    return this->accounts;
}

bool Sessions::accountExists(const std::string& username) {
    // return std::find(this->accounts.begin(), this->accounts.end(), account) != this->accounts.end();
    
    auto it = this->accounts.begin();
    while (it != this->accounts.end()) {
        if ((*it)->getUsername() == username) {
            return true;
        }
        ++it;
    }
    return false;
}

void Sessions::deleteSession(const std::string& username, unsigned long session) { 
    if (!hasSession(username, session)) {
        return;
    }

    if (this->sessions.at(username).first.getSessionId() == session) {
        this->sessions[username].first.setSessionId(0);
    }
    else {
        this->sessions[username].second.setSessionId(0);
    }
}

bool Sessions::hasSession(const std::string& username) const {
    auto it = this->sessions.find(username);

    if (it == this->sessions.end()) {
        return false;
    }

    if (it->second.first.getSessionId() == 0 && it->second.second.getSessionId() == 0) {
        return false;
    }

    return true;
}

bool Sessions::hasSession(const std::string& username, unsigned long session) const {
    if (session == 0) {
        return false;
    }

    if (!hasSession(username)) {
        return false;
    }

    return this->sessions.find(username)->second.first.getSessionId() == session || this->sessions.find(username)->second.second.getSessionId() == session;
}

std::pair<Session, Session> Sessions::getActiveSessions(const std::string& username) {
    return this->sessions[username];
}