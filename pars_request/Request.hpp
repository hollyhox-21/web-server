#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/types.h>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <map>

#define CRLF "\r\n"

class Request {
private:
    std::string _method;
    std::string _uri;
    std::string _proto;

    std::map<std::string, std::string>  _head;
    std::string                         _body;

    Request();
    int     _recvRequest(int fd);
    void    _parsRequest(char *buffer, int size);
    void    _printRequest(int fd);
public:
    Request(int fd);
    ~Request();



};

#endif