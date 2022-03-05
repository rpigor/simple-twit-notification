#pragma once

#include "Account.hpp"
#include <utility>
#include <string>
#include <vector>
#include <map>

class Sessions {
private:
    std::vector<Account>& accounts;
    std::map<Account, std::pair<unsigned long, unsigned long>> sessions;
	unsigned long sessionCount;

public:
    Sessions(std::vector<Account>&);
    unsigned long createSession(const Account&);
    unsigned long createSession(const std::string&);
    std::vector<Account>& getAccounts() const;
    bool accountExists(const Account&);
    void deleteSession(const Account&, unsigned long);
    void deleteSession(const std::string&, unsigned long);
    bool hasSession(const Account&) const;
    bool hasSession(const std::string&) const;
    bool hasSession(const Account&, unsigned long) const;
    bool hasSession(const std::string&, unsigned long) const;
    std::pair<unsigned long, unsigned long> getActiveSessions(const Account&) const;
    std::pair<unsigned long, unsigned long> getActiveSessions(const std::string&) const;
};