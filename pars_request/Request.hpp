#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/types.h>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <map>
#include <iostream>
#include <cstdlib>

#define CRLF "\r\n"
#define CRLF_END "\r\n\r\n"

class Request {
private:
	int			_result;
    std::string _method;
    std::string _uri;
    std::string _proto;
	
	std::string 						_body;
	std::string							_header;
	std::map<std::string, std::string>  _mapHeaders;
	
	size_t	_findNth(const std::string & str , unsigned int N, const std::string & find);
	bool	_checkEndHeaders(std::string & buffer);
	bool	_checkContentLength();
	void	_parsHeaders(std::string & buffer);
	void	_parsBody(std::string & buffer);
	void	_parsFirstHeader(const std::string& buffer);
	void	_mapingHeaders(std::string & buffer);
	std::string _Key(std::string& buffer);
	std::string _Value(std::string& buffer);
public:
	Request();
	~Request();
	
	std::string	getMethod() const;
	std::string	getUri() const;
	std::string getProto() const;
	std::string	getValueMapHeader(std::string key);
	
	std::string getHeader() const;
	void	parsRequest(std::string & buffer, int size);
	
	void    printRequest();
	void	printMap();



};

#endif