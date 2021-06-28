#include "Network/Network.hpp"
#include <string>
#include <iostream>
#include <map>
#include <iterator>

int main() {
	std::map<int, std::string> errPage;
	std::pair<int, std::string> p;
	p.first = 404;
	p.second = "/custom_error/page_404.html";
	errPage.insert(p);
	std::map<int, std::string>::iterator it = errPage.begin();
	while (it != errPage.end())
		std::cout << it->second << " " << it++->first << std::endl;
//	std::map<int, std::string>		erorPages;
//	std::map<std::string, Location> locations;
//	Server servak = Server("192.168.0.10", 80, erorPages, locations);
////	Server servak = Server("127.0.0.1", 8080, erorPages, locations);
//	servak.run();
	return 0;
}