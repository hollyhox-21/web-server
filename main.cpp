#include "Network/Network.hpp"

int main() {

	std::map<int, std::string>		erorPages;
	std::map<std::string, Location> locations;
	Server servak = Server("127.0.0.1", 80, erorPages, locations);
	servak.run();
	return 0;
}