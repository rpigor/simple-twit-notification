#include "NotificationResponseCommand.hpp"
#include "Notification.hpp"
#include <iostream>
#include <algorithm>
#include <ctime>

NotificationResponseCommand::NotificationResponseCommand(Sessions& sessions, std::map<Account, std::vector<Notification>>& notifications)
    : Command(sessions), notifications(notifications) {

}

NotificationResponseCommand::NotificationResponseCommand(Connection connection, Sessions& sessions, std::map<Account, std::vector<Notification>>& notifications, const std::string& payload)
    : Command(connection, sessions, payload), notifications(notifications) {

}

void NotificationResponseCommand::execute() {
    std::string username = this->payload.substr(0, this->payload.find(","));
    std::string auxTimestamp = this->payload.substr(this->payload.find(",") + 1);
    std::time_t timestamp = std::stol(auxTimestamp.substr(0, auxTimestamp.find(",")));
    std::string auxAuthor = auxTimestamp.substr(auxTimestamp.find(",") + 1);
    std::string author = auxAuthor.substr(0, auxAuthor.find(","));
    std::string auxMessage = auxAuthor.substr(auxAuthor.find(",") + 1);
    std::string message = auxMessage.substr(0, auxMessage.find_last_of(","));

    /*Notification notification(author, Tweet(author, message, timestamp));

    auto it = std::find(this->notifications.at(username).begin(), this->notifications.at(username).end(), notification);
    if (it == this->notifications.at(username).end()) {
        return;
    }*/

    auto it = this->notifications.at(username).begin();
    for (; it < this->notifications.at(username).end(); ++it) {
        if (it->getAuthor().getUsername() == author &&
        it->getTweet().getAccount().getUsername() == author &&
        static_cast<std::time_t>(it->getTweet().getEpoch()) == timestamp &&
        message.find(it->getTweet().getMessage()) != std::string::npos) {
            break;
        }
    }

    if (it == this->notifications.at(username).end()) {
        return;
    }

    std::cout << "Notified @" << username << "!" << std::endl;
    this->notifications.at(username).erase(it);
}

std::string NotificationResponseCommand::name() const {
    return "Generates response to notification";
}