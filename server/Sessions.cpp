#include "Sessions.hpp"
#include <algorithm>
#include <limits>

Sessions::Sessions(Accounts& accounts)
    : accounts(accounts), sessionCount(0) {

}

Session Sessions::createSession(Connection connection, const std::string& username) {
    Session invalidSession(0, connection);

    if (!this->accounts.accountExists(username)) {
        this->accounts.createAccount(username);
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

Accounts& Sessions::getAccounts() const {
    return this->accounts;
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