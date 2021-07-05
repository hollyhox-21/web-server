//
// Created by Evgeniy on 26.06.2021.
//

#include "Request.hpp"
#include <string>
#include <cstring>
#include "Response.hpp"
#include "../ParseFile.hpp"


int main(int ac, char **av) {
	std::string buffer1 = "GET /Network/html/autoindex.html HTTP/1.1\r\n"
						 "scheme: https\r\n"
						 "accept: */*\r\n"
						 "accept-encoding: gzip, deflate, br\r\n"
						 "accept-language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7\r\n"
						 "if-modified-since: Wed, 30 Jun 2021 15:00:00 GMT\r\n"
						 "referer: https://coderoad.ru/\r\n"
						 "sec-ch-ua-mobile: ?0\r\n"
						 "sec-fetch-dest: script\r\n"
						 "sec-fetch-mode: no-cors\r\n"
						 "Content-Length: 12\r\n"
						 "sec-fetch-site: cross-site\r\n"
						 "\r\n"
						 "Hello World!";
	
	Request myReq;
	myReq.parsRequest(buffer1, strlen(buffer1.c_str()));
	std::vector<Server*> serv = parser(av[1]);

//	std::string buffer2 = " worl";
//
//	myReq.parsRequest(buffer2, strlen(buffer2.c_str()));
//
//	std::string buffer3 = "d!";
//
//	myReq.parsRequest(buffer3, strlen(buffer3.c_str()));
	std::map<int, std::string> errorPage = serv[0]->getErrorPages();
	std::map<std::string, Location> locations = serv[0]->getLocations();
	Response response(myReq, errorPage, locations);
	std::cout << response.toFront().first << std::endl;
//	myReq.printRequest();
//	printf("%s", myReq.getHeader().c_str());
	return 0;
}

