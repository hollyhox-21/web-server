#include <vector>
#include "Response.hpp"
#include <fcntl.h>
#include <cstdlib>

Response::~Response()
{
	delete[] _fileSrc;
}

Response::Response(Request &request, std::map<int, std::string> &errorPage, std::map<std::string, Location> &locations) : _fileLength(0), _fileSrc(NULL), _request(request), _errorPage(errorPage), _locations(locations)
{
	if (request.getMethod().find("GET", 0, 3) != std::string::npos)
		responseOnGet();
	else if (request.getMethod().find("POST", 0, 4) != std::string::npos)
		responseOnPost();
	else if (request.getMethod().find("DELETE", 0, 6) != std::string::npos)
		responseOnDelete();
	else if (request.getMethod().find("PUT", 0, 3) != std::string::npos)
		responseOnPut();
}

char month[12][4] = {"Jan",
					 "Feb",
					 "Mar",
					 "Apr",
					 "May",
					 "Jun",
					 "Jul",
					 "Aug",
					 "Sep",
					 "Oct",
					 "Nov",
					 "Dec"};

char week[7][4] = {
		"Mod",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat",
		"Sun"
};

std::string Response::getdate()
{
	std::time_t t = std::time(0);
	std::tm *now = std::localtime(&t);
	std::string date;
	date += week[now->tm_wday];
	date += ", ";
	date += std::to_string(now->tm_mon);
	date += ' ';
	date += month[now->tm_mon];
	date += ' ';
	date += std::to_string(now->tm_year + 1900);
	date += ' ';
	date += std::to_string(now->tm_hour);
	date += ':';
	date += std::to_string(now->tm_min);
	date += ':';
	date += std::to_string(now->tm_sec);
	date += " GMT\r\n";
	// std::cout << "|" << date << "|" << std::endl;
	return date;
}

std::string Response::makeHeader(std::string &uri, std::string &src, std::string code)
{
	std::string header;
	header += "HTTP/1.1 ";
	header += code;
	header += "\r\n";
	header += "Connection: Keep-Alive\r\n";
	header += "Content-Length: ";
	header += std::to_string(_fileLength);
	header += "\r\n";
	if (uri.rfind(".html") != std::string::npos)
		header += "Content-Type: text/html\r\n";
	else if (uri.rfind(".png") != std::string::npos)
		header += "Content-Type: image/png\r\n";
	else if (uri.rfind(".jpg") != std::string::npos)
		header += "Content-Type: image/jpg\r\n";
	else if (uri.rfind(".jpeg") != std::string::npos)
		header += "Content-Type: image/jpg\r\n";
	else if (uri.rfind(".ico") != std::string::npos)
		header += "Content-Type: image/ico\r\n";
	else
		header += "Content-Type: text/html\r\n";
	header += "Date: ";
	header += getdate();
	header += "Server: WebServer By Monsters\r\n";
	header += "Last-Modified: ";
	header += getdate();
	header += "\r\n";
	header += src;
	_fileLength += header.length();
	return header;
}

void	Response::generateResponse(std::string uri, std::map<std::string, Location>::iterator it)
{
	int fd = open(uri.c_str(), O_RDONLY);
	if (fd < 0)
	{
		fileNotFound(it->second.root);
		return;
	} else
	{
		char buff[1025];
		std::string src;
		int len;
		while ((len = read(fd, buff, 1024)) > 0)
		{
			_fileLength += len;
			std::string dst(buff, len);
			src += dst;
		}
		src = makeHeader(uri, src, "200 OK");
		_fileLength = src.length();
		_fileSrc = new char[_fileLength + 1];
		for (unsigned long i = 0; i < _fileLength; ++i)
			_fileSrc[i] = src[i];
		_fileSrc[_fileLength] = 0;
	}
}

void Response::responseOnGet()
{
	bool f = true;
	std::string uri = _request.getUri();
	while (!uri.empty() && f)
	{
		for (std::map<std::string, Location>::iterator it = _locations.begin();
			 it != _locations.end(); ++it)
		{
			if (it->first.find(uri) != std::string::npos)
			{
				f = false;
				if (!it->second.methods.at("GET"))
				{
					methodnotallowed(it->second.root);
					break;
				}
				uri = it->second.root;
				if (_request.getUri().substr(it->first.length())[0] != '/')
					uri += "/";
				uri += _request.getUri().substr(it->first.length());
				struct stat buf;
				if (::stat(uri.c_str(), &buf) != 0)
				{
					fileNotFound(it->second.root);
					break;
				}
				if (buf.st_mode & S_IFREG)
				{
					generateResponse(uri, it);
				}
				else if (buf.st_mode & S_IFDIR)
				{
					if (it->second.autoindex.find("on", 0, 2) != std::string::npos)
					{
						DIR *dir;
						struct dirent *ent;
						std::string src;
						if ((dir = opendir(uri.c_str())) != NULL)
						{
							std::string path = it->second.root + "/www/html/autoindex.html";
							std::ifstream file(path);
							if (file.fail())
							{
								fileNotFound(it->second.root);
								break;
							}
							std::string one_line;
							while (getline(file, one_line))
							{
								if (one_line.find("<$ListOfFiles>") != std::string::npos)
									break;
								one_line += "\n";
								src += one_line;
							}
							while ((ent = readdir(dir)) != NULL)
							{
								std::string pre = "\t\t\t<li class=\"files\"><a href=\"";
								std::string path_dir = _request.getUri();
								if (path_dir[path_dir.length() - 1] == '/')
									pre += _request.getUri() + ent->d_name;
								else
									pre += _request.getUri() + '/' + ent->d_name;
								pre += "\">";
								pre += ent->d_name;
								pre += "</a></li>\n";
								src += pre;
							}
							while (getline(file, one_line))
							{
								one_line += "\n";
								src += one_line;
							}
							_fileLength = src.length();
							src = makeHeader(path, src, "200 OK");
							_fileLength = src.length();
							_fileSrc = new char[_fileLength + 1];
							for (unsigned long i = 0; i < _fileLength; ++i)
								_fileSrc[i] = src[i];
							_fileSrc[_fileLength] = 0;
						}
					}
					else if (it->second.autoindex.find("off", 0, 2) != std::string::npos)
						generateResponse(it->second.root + it->second.index, it);
					else if (uri == "/")
						uri = "";
				}
				break;
			}
		}
		if (uri != "/" && !uri.empty())
		{
			uri.erase(uri.rfind('/') + 1, uri.length());
			if (uri != "/")
				uri.erase(uri.rfind('/'));
		}
	}
}

void Response::responseOnPost()
{
	std::string uri = _request.getUri();
	bool f = true;
	while (!uri.empty() && f)
	{
		for (std::map<std::string, Location>::iterator it = _locations.begin();
			 it != _locations.end(); ++it)
		{
			if (it->first.find(uri) != std::string::npos)
			{
				f = false;
				if (!it->second.methods.at("POST"))
				{
					methodnotallowed(it->second.root);
					break;
				}
				uri = it->second.root;
				if (_request.getUri().substr(it->first.length())[0] != '/')
					uri += "/";
				uri += _request.getUri().substr(it->first.length());
				struct stat buf;
				if (::stat(uri.c_str(), &buf) != 0)
				{
					std::ofstream file(uri);
					if (file.fail())
						fileNotFound(it->second.root);
					else
					{
						std::string * a = CgiService::getCgiResponse(_request);
						std::cout << "\n\nPOST CGI:\n\n" << *a;
						// std::string body = _request.getBody();
						// file << body;
						// _fileLength = atoi(_request.getValueMapHeader(
						// 		"Content-Length").c_str());
						_fileSrc = new char[a->length()];
						// for (unsigned long i = 0; i < _fileLength; ++i)
						// 	_fileSrc[i] = body[i];
						strcpy(_fileSrc, a->c_str());
						file.close();
						break;
					}
				}
				if (buf.st_mode & S_IFREG)
				{
					std::string * a = CgiService::getCgiResponse(_request);
					std::cout << "\n\nPOST CGI:\n\n" << *a;
					_fileSrc = new char[a->length()];
					strcpy(_fileSrc, a->c_str());
					// int fd = open(uri.c_str(), O_RDWR);
					// if (fd < 0)
					// {
					// 	//TODO: add diff error
					// 	fileNotFound(it->second.root);
					// 	break;
					// }
					// else
					// {
					// 	std::cout << "\n\nPOST 2:\n\n";
					// 	char buff[1025];
					// 	std::string src;
					// 	int len;
					// 	while ((len = read(fd, buff, 1024)) > 0)
					// 	{
					// 		_fileLength += len;
					// 		std::string dst(buff, len);
					// 		src += dst;
					// 	}
					// 	int contLength = atoi(_request.getValueMapHeader("Content-Length").c_str());
					// 	if (contLength > 0)
					// 	{
					// 		std::string body = _request.getBody();
					// 		src += body;
					// 		_fileLength += contLength;
					// 	}
					// 	src = makeHeader(uri, src, "200 OK");
					// 	_fileSrc = new char[_fileLength];
					// 	for (unsigned long i = 0; i < _fileLength; ++i)
					// 		_fileSrc[i] = src[i];
					//	write(fd, _fileSrc + (_fileLength - contLength), contLength);
					//	}
				}
				else if (buf.st_mode & S_IFDIR)
				{
					fileNotFound(it->second.root);
					break;
				}
				break;
			}
		}
		if (uri != "/" && !uri.empty())
		{
			uri.erase(uri.rfind('/') + 1, uri.length());
			if (uri != "/")
				uri.erase(uri.rfind('/'));
		}
	}
}

void Response::responseOnPut()
{
	std::string uri = _request.getUri();
	bool f = true;
	while (!uri.empty() && f)
	{
		for (std::map<std::string, Location>::iterator it = _locations.begin();
			 it != _locations.end(); ++it)
		{
			if (it->first.find(uri) != std::string::npos)
			{
				f = false;
				if (!it->second.methods.at("POST"))
				{
					methodnotallowed(it->second.root);
					break;
				}
				uri = it->second.root;
				if (_request.getUri().substr(it->first.length())[0] != '/')
					uri += "/";
				uri += _request.getUri().substr(it->first.length());
				struct stat buf;
				if (::stat(uri.c_str(), &buf) != 0)
				{
					std::ofstream file(uri);
					if (file.fail())
						fileNotFound(it->second.root);
					else
					{
						std::string body = _request.getBody();
						file << body;
						std::string src = "";
						src = makeHeader(it->second.root, src, "201 Created");
						_fileLength = src.length();
						_fileSrc = new char[_fileLength + 1];
						for (unsigned long i = 0; i < _fileLength; ++i)
							_fileSrc[i] = src[i];
						_fileSrc[_fileLength] = 0;
						file.close();
						break;
					}
				}
				if (buf.st_mode & S_IFREG)
				{
					std::ofstream file(uri);
					if (file.fail())
						fileNotFound(it->second.root);
					else
					{
						std::string body = _request.getBody();
						file << body;
						std::string src = "";
						src = makeHeader(it->second.root, src, "200 OK");
						_fileLength = src.length();
						_fileSrc = new char[_fileLength + 1];
						for (unsigned long i = 0; i < _fileLength; ++i)
							_fileSrc[i] = src[i];
						_fileSrc[_fileLength] = 0;
						file.close();
						break;
					}
				}
				else if (buf.st_mode & S_IFDIR)
				{
					fileNotFound(it->second.root);
					break;
				}
				break;
			}
		}
		if (uri != "/" && !uri.empty())
		{
			uri.erase(uri.rfind('/') + 1, uri.length());
			if (uri != "/")
				uri.erase(uri.rfind('/'));
		}
	}
}

void Response::responseOnDelete()
{
	bool f = true;
	std::string uri = _request.getUri();
	while (!uri.empty() && f)
	{
		for (std::map<std::string, Location>::iterator it = _locations.begin();
			 it != _locations.end(); ++it)
		{
			if (it->first.find(uri) != std::string::npos)
			{
				f = false;
				if (!it->second.methods.at("DELETE"))
				{
					methodnotallowed(it->second.root);
					break;
				}
				uri = it->second.root;
				if (_request.getUri().substr(it->first.length())[0] != '/')
					uri += "/";
				uri += _request.getUri().substr(it->first.length());
				struct stat buf;
				if (::stat(uri.c_str(), &buf) != 0)
				{
					fileNotFound(it->second.root);
					break;
				}
				if (buf.st_mode & S_IFREG)
				{
					if (remove(uri.c_str()))
					{
						fileNotFound(it->second.root);
						break;
					}
				}
				else if (buf.st_mode & S_IFDIR)
				{
					fileNotFound(it->second.root);
					break;
				}
				std::string body = "<html>\n"
						  "\t<body>\n"
						  "\t\t<h1>File deleted.</h1>\n"
						  "\t</body>\n"
						  "</html>";
				_fileLength = body.length();
				std::string header = makeHeader(uri, body, "200 OK");
				_fileLength = header.length();
				_fileSrc = new char[_fileLength + 1];
				for (size_t i = 0; i < _fileLength; ++i)
					_fileSrc[i] = header[i];
				_fileSrc[_fileLength] = 0;
				std::cout << _fileSrc;
				break;
			}
		}
		if (uri != "/" && !uri.empty())
		{
			uri.erase(uri.rfind('/') + 1, uri.length());
			if (uri != "/")
				uri.erase(uri.rfind('/'));
		}
	}
}

std::pair<char *, int> Response::toFront()
{
	return std::pair<char *, int>(_fileSrc, _fileLength);
}

void Response::fileNotFound(std::string root)
{
	std::map<int, std::string>::iterator it = _errorPage.begin();
	for (; it != _errorPage.end() && it->first != 404; ++it) {	}
	std::string path;
	if (root.rfind('/') == (root.length() - 1))
		root.erase(root.length() - 1);
	path = root + it->second;
	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
	{
		path = "./Network/html/404.html";
		fd = open(path.c_str(), O_RDONLY);
		if (fd < 0)
			return;
	}
	else
	{
		char buff[1025];
		std::string src;
		int len;
		while ((len = read(fd, buff, 1024)) > 0)
		{
			_fileLength += len;
			std::string dst(buff, len);
			src += dst;
		}
		_fileLength = src.length();
		src = makeHeader(path, src, "404 Not Found");
		_fileLength = src.length();
		_fileSrc = new char[_fileLength];
		for (unsigned long i = 0; i < _fileLength; ++i)
			_fileSrc[i] = src[i];
		_fileSrc[_fileLength] = 0;
	}
}


void Response::methodnotallowed(std::string root)
{
	std::map<int, std::string>::iterator it = _errorPage.begin();
	for (; it != _errorPage.end() && it->first != 405; ++it) {	}
	std::string path;
	if (root.rfind('/') == (root.length() - 1))
		root.erase(root.length() - 1);
	path = root + it->second;
	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
	{
		fileNotFound(root);
	}
	else
	{
		char buff[1025];
		std::string src;
		int len;
		while ((len = read(fd, buff, 1024)) > 0)
		{
			_fileLength += len;
			std::string dst(buff, len);
			src += dst;
		}
		_fileLength = src.length();
		src = makeHeader(path, src, "405 Method Not Allowed");
		_fileLength = src.length();
		_fileSrc = new char[_fileLength];
		for (unsigned long i = 0; i < _fileLength; ++i)
			_fileSrc[i] = src[i];
		_fileSrc[_fileLength] = 0;
	}
}
