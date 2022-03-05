#include "Client.hpp"
#include <unistd.h>
#include <string.h>

Client::Client(unsigned short port) {
    struct sockaddr_in serverAddr;

	if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("a");
		exit(1);
	}

	memset((char *) &serverAddr, 0, sizeof(serverAddr));
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
    if (connect(this->sock,(struct sockaddr *) &serverAddr,sizeof(serverAddr)) < 0) 
        printf("ERROR connecting\n");

}

Client::~Client() {
    close(this->sock);
}

int Client::sendMessage(const std::string& message){
    return send(this->sock, message.c_str(), message.length(), 0);
}
