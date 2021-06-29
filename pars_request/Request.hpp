#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/types.h>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <map>
#include <iostream>

#define CRLF "\r\n"
#define CRLF_END "\r\n\r\n"

class Request {
private:
    std::string _method;
    std::string _uri;
    std::string _proto;

    std::map<std::string, std::string>  _headers;
    std::string                         _body;
	
	size_t	_findNth(const std::string & str , unsigned int N, const std::string & find);
    int     _recvRequest(int fd);
    void    _parsRequest(std::string & buffer, int size);
public:
	Request(std::string &buffer, size_t size);
	~Request();
	
	void    printRequest();



};

#endif