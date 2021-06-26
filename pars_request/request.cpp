#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

class Request {
private:
    std::string _method;
    std::string _uri;
    std::string _proto;

    std::string _head;
    std::string _body;



public:
    Request(int fd);
    ~Request();

    Request() {
        
    };

};

#endif