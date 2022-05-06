#include "Application.hpp"
#include <iostream>
#include <string>
#include <algorithm>

int main(int argc, char* argv[]) {
	if (argc != 4) {
		std::cout << "[ERROR] You must specify a profile, an IP and a port, in this order." << std::endl;
		std::cout << "Example: ./main @igor 127.0.0.1 8888" << std::endl;
		return 1;
	}

	std::string profile = argv[1];
	if (profile.at(0) != '@') {
		std::cout << "[ERROR] Invalid profile syntax. Profile must begin with '@'." << std::endl;
		std::cout << "Valid profile example: @igor" << std::endl;
		return 1;
	}
	profile = profile.substr(1);
	if (profile.length() < 4 || profile.length() > 20) {
		std::cout << "[ERROR] Invalid username." << std::endl;
		std::cout << "Username must have atleast 4 and at most 20 characters." << std::endl;
		return 1;
	}

	std::string ip = argv[2];
	if (ip.length() > 15 || std::count(ip.begin(), ip.end(), '.') != 3) {
		std::cout << "[ERROR] Invalid IPv4 syntax." << std::endl;
		std::cout << "Valid IPv4 example: 127.0.0.1" << std::endl;
		return 1;
	}

	unsigned short port;
	try {
		port = std::stoul(argv[3]);
	}
	catch(std::exception& e) {
		std::cout << "[ERROR] Invalid port: " << e.what() << "." << std::endl;
		std::cout << "Valid port example: 8888" << std::endl;
		return 1;
	}

	Application::setProfile(profile);
	Application::run(ip, port);
	
	return 0;
}