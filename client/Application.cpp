#include "Application.hpp"
#include "Client.hpp"
#include <iostream>
#include <string>

void Application::run(const std::string& profile, const std::string& ip, unsigned short port) {
    Client client(ip, port);

    if (client.sendMessage("sessao," + profile + ",") < 0) {
        perror("a");
        return;
    }

    if (client.receiveMessage() <= 0) {
        perror("b");
        return;
    }

    std::string auxSession = client.getMessage().substr(client.getMessage().find(",") + 1);
    std::string session = auxSession.substr(0, auxSession.find(","));

    try {
        if (std::stoul(session) == 0) {
            std::cout << "[ERROR] Invalid profile. Could not log in." << std::endl;
            return;
        }
        else {
            std::cout << "[SUCCESS] Logged in with account @" << profile << "." << std::endl;
        }
    }
    catch(std::invalid_argument& e) {
        std::cout << "[ERROR] Received invalid message: " << e.what() << ". Check IP and port." << std::endl;
        return;
    }

    std::string message;

    while (true) {
        std::cout << ">";
        std::getline(std::cin, message);

        std::string commandStr = message.substr(0, message.find(" "));
        std::string payloadStr = message.substr(message.find(" ") + 1);

        if (commandStr == "FOLLOW") {
            if (payloadStr.at(0) != '@') {
                std::cout << "[ERROR] Invalid profile syntax. Profile must begin with '@'." << std::endl;
		        std::cout << "Valid profile example: @igor" << std::endl;
                continue;
            }

            std::string serverMessage = "seguir,"+ profile + "," + session + "," + payloadStr.substr(1) + ",";
            if (client.sendMessage(serverMessage) < 0) {
                perror("a");
                continue;
            }

            if (client.receiveMessage() <= 0) {
                perror("b");
                continue;
            }

            std::string serverResponse = client.getMessage();
            if (serverResponse.find("sessao invalida") != std::string::npos) {
                std::cout << "[ERROR] Invalid session. Try to log in again." << std::endl;
                return;
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
            std::string serverMessage = "tweet," + profile + "," + session + "," + payloadStr + ",";
            if (client.sendMessage(serverMessage) < 0) {
                perror("a");
                continue;
            }

            if (client.receiveMessage() <= 0) {
                perror("b");
                continue;
            }

            std::string serverResponse = client.getMessage();
            if (serverResponse.find("sessao invalida") != std::string::npos) {
                std::cout << "[ERROR] Invalid session. Try to log in again." << std::endl;
                return;
            }
            else if (serverResponse.find("mensagem excede 140 caracteres") != std::string::npos) {
                std::cout << "[ERROR] Tweet exceeds 140 characters. Try being more direct." << std::endl;
                continue;
            }
            else if (serverResponse.find(serverMessage) == std::string::npos) {
                std::cout << "[ERROR] Couldn't tweet for some reason. Try again, maybe?" << std::endl;
                continue;
            }

            std::cout << "[SUCCESS] You tweeted '" << payloadStr << "'!" << std::endl;
        }
        else if (message == "HELP") {
            printCommands();
        }
        else {
            std::cout << "[ERROR] Invalid command." << std::endl;
            std::cout << "Type 'HELP' for a list of commands." << std::endl;
        }

        if (client.sendMessage(message) < 0) {
            std::cout << "[ERROR] Failed to send command. Try again." << std::endl;
        }
    }  
}

void Application::printCommands() {
    std::cout << "[INFO] Commands:" << std::endl;
    std::cout << "\t\tFOLLOW <profile>\t\tE.g.: FOLLOW @igor" << std::endl;
    std::cout << "\t\tSEND <message>\t\t\tE.g.: SEND hello world!" << std::endl;
    std::cout << "\t\tHELP" << std::endl;
}