#include "Application.hpp"
#include <string>

int main(int argc, char* argv[]) {
	std::string perfil = argv[1];
	std::string ip = argv[2];
	unsigned short porta = (unsigned short) std::strtoul(argv[3], NULL, 0);

	Application::run(perfil, ip, porta);
	
	return 0;
}