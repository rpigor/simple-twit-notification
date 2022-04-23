#pragma once

#include "Session.hpp"
#include "Connection.hpp"
#include "Accounts.hpp"
#include <utility>
#include <string>
#include <vector>
#include <map>
#include <memory>

class Sessions {
private:
    Accounts& accounts;
    std::map<std::string, std::pair<Session, Session>> sessions;
	unsigned long sessionCount;

public:
    Sessions(Accounts&);
    Session createSession(Connection, const std::string&);
    Accounts& getAccounts() const;
    void deleteSession(const std::string&, unsigned long);
    bool hasSession(const std::string&) const;
    bool hasSession(const std::string&, unsigned long) const;
    std::pair<Session, Session> getActiveSessions(const std::string&);
};