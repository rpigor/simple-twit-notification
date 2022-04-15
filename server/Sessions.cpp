#include "Sessions.hpp"
#include <algorithm>
#include <limits>

Sessions::Sessions(std::vector<std::shared_ptr<Account>>& accounts)
    : accounts(accounts), sessionCount(0) {

}

Session Sessions::createSession(Connection connection, const Account& account) {
    Session invalidSession(0, connection);

    if (!accountExists(account)) {
        this->accounts.push_back(std::make_shared<Account>(account));
    }

    if (this->sessions.find(account) == this->sessions.end()) {
        this->sessionCount = (this->sessionCount + 1) % std::numeric_limits<unsigned long>::max();

        Session validSession(this->sessionCount, connection);
        this->sessions[account] = std::make_pair(validSession, invalidSession);
        return validSession;
    }

    this->sessionCount = (this->sessionCount + 1) % std::numeric_limits<unsigned long>::max();

    Session validSession(this->sessionCount, connection);
    if (this->sessions.at(account).second.getSessionId() < this->sessions.at(account).first.getSessionId()) {
        this->sessions[account].second = validSession;
    }
    else {
        this->sessions[account].first = validSession;
    }

    return validSession;
}

std::vector<std::shared_ptr<Account>>& Sessions::getAccounts() const {
    return this->accounts;
}

bool Sessions::accountExists(const Account& account) {
    // return std::find(this->accounts.begin(), this->accounts.end(), account) != this->accounts.end();
    
    auto it = this->accounts.begin();
    while (it != this->accounts.end()) {
        if ((**it) == account) {
            return true;
        }
        ++it;
    }
    return false;
}

void Sessions::deleteSession(const Account& account, unsigned long session) { 
    if (!hasSession(account, session)) {
        return;
    }

    if (this->sessions.at(account).first.getSessionId() == session) {
        this->sessions[account].first.setSessionId(0);
    }
    else {
        this->sessions[account].second.setSessionId(0);
    }
}

bool Sessions::hasSession(const Account& account) const {
    auto it = this->sessions.find(account);

    if (it == this->sessions.end()) {
        return false;
    }

    if (it->second.first.getSessionId() == 0 && it->second.second.getSessionId() == 0) {
        return false;
    }

    return true;
}

bool Sessions::hasSession(const Account& account, unsigned long session) const {
    if (session == 0) {
        return false;
    }

    if (!hasSession(account)) {
        return false;
    }

    return this->sessions.find(account)->second.first.getSessionId() == session || this->sessions.find(account)->second.second.getSessionId() == session;
}

std::pair<Session, Session> Sessions::getActiveSessions(const Account& account) {
    return this->sessions[account];
}