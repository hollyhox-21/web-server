#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include "Location.hpp"
# include "../WServer.hpp"
# include "../../CGI/CgiService.hpp"
# include <map>
# include <string>
# include <string>
# include <fstream>
# include <iostream>
# include <cstring>
# include <dirent.h>
# include <vector>
# include <fcntl.h>
# include <sys/stat.h>
# include <unistd.h>
# include <ctime>

class Response
{
private:
//	std::string		_version;
//	int				_statusCode;
//	char			*_time;
//	std::string		_name;
	unsigned long _fileLength;
	char *_fileSrc;
	Request &_request;
	t_server & _serverSettings;
	void responseOnGet();
	void responseOnPost();
	void responseOnDelete();
	void responseOnPut();
	void fileNotFound(std::string root);
	void methodnotallowed(std::string root);
	std::string makeHeader(std::string &uri, std::string &src, std::string code);
	std::string getdate();
	void	generateResponse(std::string uri, std::map<std::string, Location>::iterator it);
public:
	Response(Request &request, t_server & serverSettings);
	~Response();
	std::pair<char *, int> toFront();
};

#endif