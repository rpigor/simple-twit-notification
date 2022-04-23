#include "Accounts.hpp"

void Accounts::createAccount(const Account& account) {
    this->accounts.push_back(std::make_shared<Account>(account));
}

void Accounts::createAccount(const std::string& username) {
    this->accounts.push_back(std::make_shared<Account>(username));
}

std::shared_ptr<Account> Accounts::findAccount(const std::string& username) const {
    auto it = this->accounts.begin();
    while (it != this->accounts.end()) {
        if ((*it)->getUsername() == username) {
            break;
        }
        ++it;
    }
    return *it;
}

bool Accounts::accountExists(const std::string& username) const {
    auto it = this->accounts.cbegin();
    while (it != this->accounts.cend()) {
        if ((*it)->getUsername() == username) {
            return true;
        }
        ++it;
    }
    return false;
}

std::size_t Accounts::size() const {
    return this->accounts.size();
}