#include "Network/Network.hpp"

int main() {

	std::map<int, std::string>		erorPages;
	std::map<std::string, Location> locations;
<<<<<<< HEAD
	Server servak = Server("192.168.0.10", 80, erorPages, locations);
=======
	Server servak = Server("127.0.0.1", 8080, erorPages, locations);
>>>>>>> origin/pars_config
	servak.run();
	return 0;
}