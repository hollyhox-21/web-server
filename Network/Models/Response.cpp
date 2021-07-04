#include <vector>
#include "Response.hpp"
#include <fcntl.h>

Response::~Response()
{
}

Response::Response(Request &request, std::map<int, std::string> &errorPage, std::map<std::string, Location> &locations) : _request(request), _errorPage(errorPage), _locations(locations), _fileLength(0), _fileSrc(NULL)
{
	if (request.getMethod().find("GET", 0, 3) != std::string::npos)
		responseOnGet();
	else if (request.getMethod().find("POST", 0, 4) != std::string::npos)
		responseOnPost();
	else if (request.getMethod().find("DELETE", 0, 6) != std::string::npos)
		responseOnDelete();
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
				uri = it->second.root;
				if (uri.rfind('/') == (uri.length() - 1))
					uri.erase(uri.length() - 1);
				uri += _request.getUri();
				struct stat buf;
				if (::stat(uri.c_str(), &buf) != 0)
				{
					fileNotFound(it->second.root);
					break;
				}
				if (buf.st_mode & S_IFREG)
				{
					int fd = open(uri.c_str(), O_RDONLY);
					if (fd < 0)
					{
						fileNotFound(it->second.root);
						break;
					}
					else
					{
						char buff[1025];
						std::string src;
						int len;
//						char dst[3000000];
						while ((len = read(fd, buff, 1024)) > 0)
						{
//							(char*)memmove(dst + _fileLength, buff, len);
							_fileLength += len;
							std::string dst(buff, len);
							src += dst;
						}
						_fileSrc = new char[_fileLength];
						for (int i = 0; i < _fileLength; ++i)
							_fileSrc[i] = src[i];
					}
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
							std::ifstream file("../Network/html/autoindex.html");
							if (file.fail())
							{
								fileNotFound(it->second.root);
								break;
							}
							std::string one_line;
							while (getline(file, one_line))
							{
								if (one_line.find("<$ListOfFiles>") !=
									std::string::npos)
									break;
								one_line += "\n";
								src += one_line;
							}
							while ((ent = readdir(dir)) != NULL)
							{
								std::string pre = "\t\t\t<li class=\"files\"><a href=\"";
								pre += it->first;
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
							_fileSrc = new char[_fileLength];
							for (int i = 0; i < _fileLength; ++i)
							{
								_fileSrc[i] = src[i];
							}
						}
					}
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
				uri = it->second.root;
				if (uri.rfind('/') == (uri.length() - 1))
					uri.erase(uri.length() - 1);
				uri += _request.getUri();
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
						_fileLength = atoi(_request.getValueMapHeader(
								"Content-Length").c_str());
						_fileSrc = new char[_fileLength];
						for (int i = 0; i < _fileLength; ++i)
							_fileSrc[i] = body[i];
						break;
						file.close();
					}
				}
				if (buf.st_mode & S_IFREG)
				{
					int fd = open(uri.c_str(), O_RDWR);
					if (fd < 0)
					{
						//TODO: add diff error
						fileNotFound(it->second.root);
						break;
					}
					else
					{
						char buff[1025];
						std::string src;
						int len;
//						char dst[3000000];
						while ((len = read(fd, buff, 1024)) > 0)
						{
//							(char*)memmove(dst + _fileLength, buff, len);
							_fileLength += len;
							std::string dst(buff, len);
							src += dst;
						}
						int contLength = atoi(_request.getValueMapHeader("Content-Length").c_str());
						if (contLength > 0)
						{
							std::string body = _request.getBody();
							src += body;
							_fileLength += contLength;
						}
						_fileSrc = new char[_fileLength];
						for (int i = 0; i < _fileLength; ++i)
							_fileSrc[i] = src[i];
						write(fd, _fileSrc + (_fileLength - contLength), contLength);
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
	if (it != _errorPage.end())
		path = root + it->second;
	else
		path = "../Network/html/404.html";
	std::ifstream file(path);
	if (file.fail())
		nullptr;
	else
	{
		std::string line, src;
		while (getline(file, line))
		{
			src += line;
			src += "\n";
		}
		_fileLength = src.length();
		_fileSrc = new char[_fileLength];
		for (int i = 0; i < _fileLength; ++i)
			_fileSrc[i] = src[i];
	}
}
