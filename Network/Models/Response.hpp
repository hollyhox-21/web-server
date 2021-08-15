#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include "Request.hpp"
#include "../../CGI/CgiService.hpp"
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
#include <ctime>

class Response
{
private:
	unsigned long _fileLength;
	char *_fileSrc;
	Request &_request;
	std::map<int, std::string> &_errorPage;
	std::map<std::string, Location> &_locations;
	void responseOnGet();
	void responseOnPost();
	void responseOnDelete();
	void responseOnPut();
	void fileNotFound(std::string root);
	void methodnotallowed(std::string root);
	int findLocation(std::map<std::string, Location>::iterator *it);
	void createSrc(std::map<std::string, Location>::iterator it, const std::string&);
	std::string makeHeader(std::string &uri, std::string &src, const std::string& code);
	static std::string getdate();
	void	generateResponse(std::string uri, std::map<std::string, Location>::iterator it);
	void	genetateResponseAutoIn(DIR *dir, struct dirent *ent, std::string src, std::ifstream &file, std::string path);
public:
	Response(Request &request, std::map<int, std::string> &errorPage, std::map<std::string, Location> &locations);
	~Response();
	std::pair<char *, int> toFront();
};



#endif