#include "Network/Network.hpp"

int main() {

	std::map<int, std::string>		erorPages;
	std::map<std::string, Location> locations;
	Server servak = Server("192.168.0.10", 80, erorPages, locations);
	servak.run();
	return 0;
}