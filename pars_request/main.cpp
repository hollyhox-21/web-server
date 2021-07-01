//
// Created by Evgeniy on 26.06.2021.
//

#include "Request.hpp"
#include <string>
#include <cstring>


int main() {
	std::string buffer = "GET / HTTP/1.1\r\n"
						 "Host: developer.mozilla.org\r\n"
						 "Accept-Language: fr\r\n\r\n";
	
	Request myReq(buffer, strlen(buffer.c_str()));
	myReq.printRequest();
	
	return 0;
}