#include "Application.hpp"
#include "Client.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <ctime>
#include <csignal>

std::string Application::profile;

unsigned long Application::session;

Client* Application::client;

void Application::run(const std::string& ip, unsigned short port) {
    Client auxClient(ip, port);
    client = &auxClient;

    initializeExitSignals();

    if (client->sendMessage("sessao," + profile + ",") < 0) {
        perror("a");
        return;
    }

    if (client->receiveMessage() < 0) {
        perror("b");
        return;
    }

    std::string auxSession = client->getMessage().substr(client->getMessage().find(",") + 1);
    session = std::stoul(auxSession.substr(0, auxSession.find(",")));

    try {
        if (session == 0) {
            std::cout << "[ERROR] Invalid profile. Could not log in." << std::endl;
            exit(1);
        }
        else {
            std::cout << "[SUCCESS] Logged in with account @" << profile << "." << std::endl;
        }
    }
    catch(std::invalid_argument& e) {
        std::cout << "[ERROR] Received invalid message: " << e.what() << ". Check IP and port." << std::endl;
        return;
    }

    std::thread notificationsThread(handleNotifications);

    handleCommands();

    notificationsThread.join();
}

void Application::setProfile(const std::string& profile) {
    Application::profile = profile;
}

void Application::handleCommands() {
    std::string message;

    while (true) {
        std::cout << ">";
        std::getline(std::cin, message); // blocking call

        std::string commandStr = message.substr(0, message.find(" "));
        std::string payloadStr = message.substr(message.find(" ") + 1);

        std::transform(commandStr.begin(), commandStr.end(), commandStr.begin(), ::toupper);
        if (commandStr == "FOLLOW") {
            if (payloadStr.at(0) != '@') {
                std::cout << "[ERROR] Invalid profile syntax. Profile must begin with '@'." << std::endl;
		        std::cout << "Valid profile example: @igor" << std::endl;
                continue;
            }

            if (payloadStr == profile) {
                std::cout << "[ERROR] You cannot follow yourself, fool." << std::endl;
                continue;
            }

            std::string serverMessage = "seguir,"+ profile + "," + std::to_string(session) + "," + payloadStr.substr(1) + ",";
            if (client->sendMessage(serverMessage) < 0) {
                perror("a");
                continue;
            }

            if (client->receiveMessage() < 0) {
                perror("b");
                continue;
            }

            std::string serverResponse = client->getMessage();
            if (serverResponse.find("sessao invalida") != std::string::npos) {
                std::cout << "[ERROR] Invalid session. Try to log in again." << std::endl;
                exit(1);
            }
            else if (serverResponse.find("perfil nao pode seguir a si mesmo") != std::string::npos) {
                std::cout << "[ERROR] You cannot follow yourself, fool." << std::endl;
                continue;
            }
            else if (serverResponse.find("perfil para seguir nao existe") != std::string::npos) {
                std::cout << "[ERROR] This profile doesn't exist." << std::endl;
                continue;
            }
            else if (serverResponse.find(serverMessage) == std::string::npos) {
                std::cout << "[ERROR] Couldn't follow this profile. Try again." << std::endl;
                continue;
            }

            std::cout << "[SUCCESS] You followed " << payloadStr << "!" << std::endl;
        }
        else if (commandStr == "SEND") {
            if (payloadStr.length() > CHAR_LIMIT) {
                std::cout << "[ERROR] Tweet exceeds " << CHAR_LIMIT << " characters. Try being more direct." << std::endl;
                continue;
            }

            std::string serverMessage = "tweet," + profile + "," + std::to_string(session) + "," + payloadStr + ",";
            if (client->sendMessage(serverMessage) < 0) {
                perror("a");
                continue;
            }

            if (client->receiveMessage() < 0) {
                perror("b");
                continue;
            }

            std::string serverResponse = client->getMessage();
            if (serverResponse.find("sessao invalida") != std::string::npos) {
                std::cout << "[ERROR] Invalid session. Try to log in again." << std::endl;
                exit(1);
            }
            else if (serverResponse.find("mensagem excede limite") != std::string::npos) {
                std::cout << "[ERROR] Tweet exceeds " << CHAR_LIMIT << " characters. Try being more direct." << std::endl;
                continue;
            }
            else if (serverResponse.find(serverMessage) == std::string::npos) {
                std::cout << "[ERROR] Couldn't tweet for some reason. Try again, maybe?" << std::endl;
                continue;
            }

            std::cout << "[SUCCESS] You tweeted '" << payloadStr << "'!" << std::endl;
        }
        else if (commandStr == "HELP") {
            printCommands();
        }
        else if (commandStr == "EXIT") {
            sendExitMessageAndExit(0); // useless parameter (only in this case)
        }
        else {
            std::cout << "[ERROR] Invalid command." << std::endl;
            std::cout << "Type 'HELP' for a list of commands." << std::endl;
        }
    }  
}

void Application::handleNotifications() {
    while (true) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (client->nonBlockingReceiveMessage() < 0) {
            continue;
        }

        std::string message = client->getMessage();
        std::string commandStr = message.substr(0, message.find(","));

        if (commandStr != "notify") {
            continue;
        }

        std::string auxUsername = message.substr(message.find(",") + 1);
        std::string username = auxUsername.substr(0, auxUsername.find(","));
        std::string auxTimestamp = auxUsername.substr(auxUsername.find(",") + 1);
        std::time_t timestamp = std::stol(auxTimestamp.substr(0, auxTimestamp.find(",")));
        std::string auxAuthor = auxTimestamp.substr(auxTimestamp.find(",") + 1);
        std::string author = auxAuthor.substr(0, auxAuthor.find(","));
        std::string auxLen = auxAuthor.substr(auxAuthor.find(",") + 1);
        int len = std::stoi(auxLen.substr(0, auxLen.find(",")));
        std::string auxTweet = auxLen.substr(auxLen.find(",") + 1);
        std::string tweet = auxTweet.substr(0, len);

        std::tm* time = std::localtime(&timestamp);
        char formatedTime[32];
        std::strftime(formatedTime, 32, "%d/%m/%Y %H:%M:%S", time);

        std::cout << std::endl << "[" << formatedTime << "] @" << author << " just tweeted: '" << tweet << "'." << std::endl;

        std::string notifyMessage = "notify," + username + "," + std::to_string(timestamp) + "," + author + "," + std::to_string(len) + "," + tweet + ",";
        bool sent = false;
        do {
            sent = client->sendMessage(notifyMessage) >= 0;
        } while (!sent);

        std::cout << ">";
        fflush(stdout);
    }
}

void Application::initializeExitSignals() {
    std::signal(SIGINT, sendExitMessageAndExit);
	std::signal(SIGABRT, sendExitMessageAndExit);
	std::signal(SIGTERM, sendExitMessageAndExit);
	std::signal(SIGTSTP, sendExitMessageAndExit);
}

void Application::sendExitMessageAndExit(int i) {
    std::cout << std::endl << "[INFO] Exiting..." << std::endl;
    client->sendMessage("sair," + profile + "," + std::to_string(session) + ",");
    exit(0);
}

void Application::printCommands() {
    std::cout << "[INFO] Commands:" << std::endl;
    std::cout << "\t\tFOLLOW <profile>\t\tE.g.: FOLLOW @igor" << std::endl;
    std::cout << "\t\tSEND <message>\t\t\tE.g.: SEND hello world!" << std::endl;
    std::cout << "\t\tHELP" << std::endl;
    std::cout << "\t\tEXIT" << std::endl;
}