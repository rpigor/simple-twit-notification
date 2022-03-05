#include "Client.hpp"
#include "Application.hpp"
#include <iostream>
#include <string>

void Application::run(std::string perfil, std::string ip, unsigned short porta) {
    Client cliente(PORT);

    std::string message;
    std::string sessao = "1";
    
    cliente.sendMessage("sessao,"+ perfil + ",");

    while (true){
        std::getline(std::cin, message);

        std::string commandStr = message.substr(0, message.find(" "));
        std::string payloadStr = message.substr(message.find(" ") + 1);

        if(commandStr == "FOLLOW"){
            cliente.sendMessage("seguir,"+ perfil + "," + sessao + "," + payloadStr + ",");
        } else if (commandStr == "SEND") {
            cliente.sendMessage("tweet,"+ perfil + "," + sessao + "," + payloadStr + ",");
        } else {
            std::cout << "Comando inválido";
        }


        cliente.sendMessage(message);
    }
        
}