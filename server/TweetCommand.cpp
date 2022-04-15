#include "TweetCommand.hpp"
#include <iostream>

TweetCommand::TweetCommand(Sessions& sessions, std::vector<Tweet>& tweets, std::map<Account, std::vector<Notification>>& notifications)
    : Command(sessions), tweets(tweets), notifications(notifications) {

}

TweetCommand::TweetCommand(Connection connection, Sessions& sessions, std::vector<Tweet>& tweets, std::map<Account, std::vector<Notification>>& notifications, const std::string& payload)
    : Command(connection, sessions, payload), tweets(tweets), notifications(notifications) {

}

void TweetCommand::execute() {
    std::string username = this->payload.substr(0, this->payload.find(","));
    std::string auxSession = this->payload.substr(this->payload.find(",") + 1);
    std::string session = auxSession.substr(0, auxSession.find(","));
    std::string auxMessage = auxSession.substr(auxSession.find(",") + 1);
    std::string message = auxMessage.substr(0, auxMessage.find_last_of(","));
    
    if (!this->sessions.accountExists(username)) {
        if (this->connection.sendMessage("perfil nao existe") < 0) {
            perror("sendto()");
            exit(1);
        }
        std::cout << "Account @" << username << " does not exist. Aborting operation.\n";
        return;
    }

    if (!this->sessions.hasSession(username, std::stoul(session))) {
        if (this->connection.sendMessage("sessao invalida") < 0) {
            perror("sendto()");
            exit(1);
        }
        std::cout << "Invalid session [" << session << "] for account @" << username << ". Aborting operation.\n";
        return;
    }

    if (message.length() > 128) {
        if (this->connection.sendMessage("mensagem excede 128 caracteres") < 0) {
            perror("sendto()");
            exit(1);
        }
        std::cout << "Tweet message exceeds 128 characters. Abording operation.\n";
        return;
    }

    std::vector<std::shared_ptr<Account>>::iterator it = this->sessions.getAccounts().begin();
    while (it != this->sessions.getAccounts().end()) {
        if ((*it)->getUsername() == username)
            break;
        ++it;
    }
    Account user = **it;
    Tweet userTweet(user, message);
    this->tweets.push_back(userTweet);

    // push to notification queue
    for (const Account& follower : user.getFollowers()) {
        this->notifications[follower].push_back(Notification(user, userTweet));
    }

    std::cout << "Account @" << username << " just tweeted: \"" << message << "\"." << std::endl;

    std::cout << "@" << username << "'s tweets: ";
    for (const Tweet& tt : this->tweets) {
        if (tt.getAccount().getUsername() == username) {
            std::cout << "\"" << tt.getMessage() << "\" ";
        }
    }
    std::cout << std::endl;

    if (this->connection.sendMessage("tweet," + username + "," + session + "," + message + ",") < 0) {
        perror("sendto()");
        exit(1);
    }
}

std::string TweetCommand::name() const {
    return "Tweets message";
}