#pragma once

#include "Session.hpp"
#include "Connection.hpp"
#include "Account.hpp"
#include <utility>
#include <string>
#include <vector>
#include <map>
#include <memory>

class Sessions {
private:
    std::vector<std::shared_ptr<Account>>& accounts;
    std::map<Account, std::pair<Session, Session>> sessions;
	unsigned long sessionCount;

public:
    Sessions(std::vector<std::shared_ptr<Account>>&);
    Session createSession(Connection, const Account&);
    std::vector<std::shared_ptr<Account>>& getAccounts() const;
    bool accountExists(const Account&);
    void deleteSession(const Account&, unsigned long);
    bool hasSession(const Account&) const;
    bool hasSession(const Account&, unsigned long) const;
    std::pair<Session, Session> getActiveSessions(const Account&);
};