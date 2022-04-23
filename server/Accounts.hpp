#pragma once

#include "Account.hpp"
#include <vector>
#include <string>
#include <memory>

class Accounts {
    using iterator=typename std::vector<std::shared_ptr<Account>>::iterator;
    using const_iterator=typename std::vector<std::shared_ptr<Account>>::const_iterator;

private:
    std::vector<std::shared_ptr<Account>> accounts;

public:
    void createAccount(const Account&);
    void createAccount(const std::string&);
    std::shared_ptr<Account> findAccount(const std::string&) const;
    bool accountExists(const std::string&) const;
    std::size_t size() const;
    iterator begin() { return accounts.begin(); }
    iterator end() { return accounts.end(); }
    const_iterator begin() const { return accounts.cbegin(); }
    const_iterator end() const { return accounts.cend(); }
};