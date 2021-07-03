#include "Network/Network.hpp"

int main() {
	std::map<int, std::string>		erorPages;
	std::map<std::string, Location> locations;
	Server *servak = new Server();
	servak->setHost(std::string("127.0.0.1"));
	servak->setPort(1080);
	servak->ready();
	servak->run();
	return 0;
}