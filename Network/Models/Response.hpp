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
	unsigned long _fileLength;
	char *_fileSrc;
	Request &_request;
	t_server & _serverSettings;
	int responseOnGet();
	int responseOnPost();
	int responseOnDelete();
	int responseOnPut();
	int responseOnHead();
	void deleteMultiSl(std::string &uri);
	void fileNotFound(std::string root);
	int methodnotallowed(std::string root);
	void errorFileNotExist();
	void errorFileExist(std::string &path, int fd);
	void generateAutoIndex(std::string &uri, std::map<std::string, Location>::iterator &it);
	int rediraction(std::map<std::string, Location>::iterator *it);
	int findLocation(std::map<std::string, Location>::iterator *it);
	int returnErrors();
	void createSrc(std::map<std::string, Location>::iterator it, const std::string&);
	void createCgiResponse(std::string &uri);
	void createResponseWOCgi(std::string &uri, std::string code);
	std::string makeHeader(std::string &uri, std::string &src, const std::string& code, const std::string& type);
	static std::string getdate();
	void	generateResponse(std::string uri, std::map<std::string, Location>::iterator it);
	void	genetateResponseAutoIn(DIR *dir, struct dirent *ent, std::string src, std::ifstream &file, std::string path);
public:
	Response(Request &request, t_server &serverSettings);
	~Response();
	std::pair<char *, int> toFront();
};

#endif