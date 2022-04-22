#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Account {
private:
    unsigned long uid;
    std::string username;
    std::vector<std::string> following;
    std::vector<std::string> followers;

public:
    Account();
    Account(const std::string&);
    friend bool operator==(const Account&, const Account&);
    friend bool operator<(const Account&, const Account&);
    friend std::istream& operator>>(std::istream&, Account&);
    friend std::ostream& operator<<(std::ostream&, const Account&);
    unsigned long getUid() const;
    std::string getUsername() const; 
    const std::vector<std::string>& getFollowing() const;
    const std::vector<std::string>& getFollowers() const;
    void follow(const std::string&);
    void beFollowedBy(const std::string&);
};