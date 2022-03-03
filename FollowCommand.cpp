#include "FollowCommand.hpp"
#include <iostream>
#include <algorithm>

FollowCommand::FollowCommand(Sessions& sessions)
    : Command(sessions) {

}

FollowCommand::FollowCommand(Network network, Sessions& sessions, const std::string& payload)
    : Command(network, sessions, payload) {

}

void FollowCommand::execute() const {
    std::string username = this->payload.substr(0, this->payload.find(","));
    std::string auxSession = this->payload.substr(this->payload.find(",") + 1);
    std::string session = auxSession.substr(0, auxSession.find(","));
    std::string auxUserToFollow = auxSession.substr(auxSession.find(",") + 1);
    std::string userToFollow = auxUserToFollow.substr(0, auxUserToFollow.find(","));

    if (!this->sessions.accountExists(username)) {
        if (sendto(this->network.socket, std::string("perfil nao existe\n").c_str(), std::string("perfil nao existe\n").length(), 0, (struct sockaddr*) &this->network.clientAddr, sizeof(this->network.clientAddr)) == -1) {
            perror("sendto()");
            exit(1);
        }
        std::cout << "Account @" << username << " does not exist. Aborting operation.\n";
        return;
    }

    if (!this->sessions.accountExists(userToFollow)) {
        if (sendto(this->network.socket, std::string("perfil para seguir nao existe\n").c_str(), std::string("perfil para seguir nao existe\n").length(), 0, (struct sockaddr*) &this->network.clientAddr, sizeof(this->network.clientAddr)) == -1) {
            perror("sendto()");
            exit(1);
        }
        std::cout << "Account @" << userToFollow << " does not exist to follow. Aborting operation.\n";
        return;
    }

    if (!this->sessions.hasSession(username, std::stoul(session))) {
        if (sendto(this->network.socket, std::string("sessao invalida\n").c_str(), std::string("sessao invalida\n").length(), 0, (struct sockaddr*) &this->network.clientAddr, sizeof(this->network.clientAddr)) == -1) {
            perror("sendto()");
            exit(1);
        }
        std::cout << "Invalid session [" << session << "] for account @" << username << ". Aborting operation.\n";
        return;
    }

    if (userToFollow == username) {
        if (sendto(this->network.socket, std::string("perfil nao pode seguir a si mesmo\n").c_str(), std::string("perfil nao pode seguir a si mesmo\n").length(), 0, (struct sockaddr*) &this->network.clientAddr, sizeof(this->network.clientAddr)) == -1) {
            perror("sendto()");
            exit(1);
        }
        std::cout << "Account @" << userToFollow << " cannot follow itself. Aborting operation.\n";
        return;
    }

    std::vector<Account>::iterator it = this->sessions.getAccounts().begin();
    while (it != this->sessions.getAccounts().end()) {
        if (it->getUsername() == username)
            break;
        ++it;
    }

    it->follow(userToFollow);

    std::cout << "Account @" << username << " just followed account @" << userToFollow << ".\n";

    std::cout << "@" << username <<  " is following: ";
    for (const Account& acc : it->getFollowing()) {
        std::cout << "@" << acc.getUsername() << " ";
    }
    std::cout << std::endl;

    if (sendto(this->network.socket, std::string("seguir," + username + "," + session + "," + userToFollow + "\n").c_str(), std::string("seguir," + username + "," + session + "," + userToFollow + "\n").length(), 0, (struct sockaddr*) &this->network.clientAddr, sizeof(this->network.clientAddr)) == -1) {
        perror("sendto()");
        exit(1);
    }
}

std::string FollowCommand::name() const {
    return "Segue usuário";
}