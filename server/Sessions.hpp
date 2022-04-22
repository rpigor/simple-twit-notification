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
    std::map<std::string, std::pair<Session, Session>> sessions;
	unsigned long sessionCount;

public:
    Sessions(std::vector<std::shared_ptr<Account>>&);
    Session createSession(Connection, const std::string&);
    std::vector<std::shared_ptr<Account>>& getAccounts() const;
    bool accountExists(const std::string&);
    void deleteSession(const std::string&, unsigned long);
    bool hasSession(const std::string&) const;
    bool hasSession(const std::string&, unsigned long) const;
    std::pair<Session, Session> getActiveSessions(const std::string&);
};