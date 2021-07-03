#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include "Request.hpp"
#include "Location.hpp"
#include <map>
#include <string>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <dirent.h>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>


class Response
{
private:
	std::string		_version;
	int				_statusCode;
	char			*_time;
	std::string		_name;
	unsigned long _fileLength;
	char *_fileSrc;
	std::vector<std::string> *_autoindex;
	Request &_request;
	std::map<int, std::string> &_errorPage;
	std::map<std::string, Location> &_locations;
	void responseOnGet();
	void responseOnPost();
	void responseOnDelete();
	void reple(char *source, char *dest);
public:
	Response(Request &request, std::map<int, std::string> &errorPage, std::map<std::string, Location> &locations);
	~Response();
	std::pair<char *, int> toFront();
};



#endif