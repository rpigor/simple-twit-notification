#include "GroupFollowCommand.hpp"
#include <iostream>
#include <algorithm>

GroupFollowCommand::GroupFollowCommand(Sessions& sessions)
    : Command(sessions) {

}

GroupFollowCommand::GroupFollowCommand(Connectable* connectable, Sessions& sessions, const std::string& payload)
    : Command(connectable, sessions, payload) {

}

void GroupFollowCommand::execute() {
    std::string username = this->payload.substr(0, this->payload.find(","));
    std::string auxSession = this->payload.substr(this->payload.find(",") + 1);
    std::string session = auxSession.substr(0, auxSession.find(","));
    std::string auxUserToFollow = auxSession.substr(auxSession.find(",") + 1);
    std::string userToFollow = auxUserToFollow.substr(0, auxUserToFollow.find(","));

    if (!this->sessions.getAccounts().accountExists(username)) {
        std::cout << "Account @" << username << " does not exist. Aborting operation.\n";
        return;
    }

    if (!this->sessions.getAccounts().accountExists(userToFollow)) {
        std::cout << "Account @" << userToFollow << " does not exist to follow. Aborting operation.\n";
        return;
    }

    if (!this->sessions.hasSession(username, std::stoul(session))) {
        std::cout << "Invalid session [" << session << "] for account @" << username << ". Aborting operation.\n";
        return;
    }

    if (userToFollow == username) {
        std::cout << "Account @" << userToFollow << " cannot follow itself. Aborting operation.\n";
        return;
    }

    std::vector<std::shared_ptr<Account>>::iterator it = this->sessions.getAccounts().begin();
    while (it != this->sessions.getAccounts().end()) {
        if ((*it)->getUsername() == username)
            break;
        ++it;
    }
    (*it)->follow(userToFollow);

    std::cout << "Account @" << username << " just followed account @" << userToFollow << ".\n";

    std::cout << "@" << username <<  " is following: ";
    for (const std::string& auxUsername : (*it)->getFollowing()) {
        std::cout << "@" << auxUsername << " ";
    }
    std::cout << std::endl;

    it = this->sessions.getAccounts().begin();
    while (it != this->sessions.getAccounts().end()) {
        if ((*it)->getUsername() == userToFollow)
            break;
        ++it;
    }
    (*it)->beFollowedBy(username);

    std::cout << "@" << (*it)->getUsername() <<  " is being followed by: ";
    for (const std::string& auxUsername : (*it)->getFollowers()) {
        std::cout << "@" << auxUsername << " ";
    }
    std::cout << std::endl;
}

std::string GroupFollowCommand::name() const {
    return "Executes replicated user follow";
}