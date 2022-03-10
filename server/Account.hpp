#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Account {
private:
    unsigned long uid;
    std::string username;
    std::vector<Account> following;
    std::vector<Account> followers;

public:
    Account();
    Account(const std::string&);
    friend bool operator==(const Account&, const Account&);
    friend bool operator<(const Account&, const Account&);
    friend std::istream& operator>>(std::istream&, Account&);
    friend std::ostream& operator<<(std::ostream&, const Account&);
    unsigned long getUid() const;
    std::string getUsername() const; 
    const std::vector<Account>& getFollowing() const;
    const std::vector<Account>& getFollowers() const;
    void follow(const Account&);
    void beFollowedBy(const Account&);
};