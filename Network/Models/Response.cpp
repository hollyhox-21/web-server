#include <vector>
#include "Response.hpp"
#include <fcntl.h>

Response::~Response()
{
}

Response::Response(Request &request, std::map<int, std::string> &errorPage, std::map<std::string, Location> &locations) : _request(request), _errorPage(errorPage), _locations(locations)
{
	if (request.getMethod().find("GET", 0, 3) != std::string::npos)
		responseOnGet();
	else if (request.getMethod().find("POST", 0, 4) != std::string::npos)
		responseOnPost();
	else if (request.getMethod().find("DELETE", 0, 6) != std::string::npos)
		responseOnDelete();
}

void bad_file()

void Response::responseOnGet()
{
	std::string uri = _request.getUri();
	while (!uri.empty())
	{
		for (std::map<std::string, Location>::iterator it = _locations.begin();
			 it != _locations.end(); ++it)
		{
			if (it->first.find(uri) != std::string::npos)
			{
				uri = it->second.root;
				if (uri.rfind('/') == (uri.length() - 1))
					uri.erase(uri.length() - 1);
				uri += _request.getUri();
				struct stat buf;
				if (::stat(uri.c_str(), &buf) != 0)
				{
					fileNotFound();
					uri = "";
					break;
				}
				if (buf.st_mode & S_IFREG)
				{
					int fd = open(uri.c_str(), O_RDONLY);
					if (fd < 0)
					{
						fileNotFound();
						uri = "";
						break;
					}
					else
					{
						char buff[1024];
						std::string src;
						int len;
						while ((len = read(fd, buff, 1024)) > 0)
						{
							_fileLength += len;
							std::string dst(buff, len);
							src += dst;
						}
						_fileSrc = new char[_fileLength];
						for (int i = 0; i < _fileLength; ++i)
						{
							_fileSrc[i] = src[i];
						}
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
								fileNotFound();
								uri = "";
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
				uri = "";
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

}

void Response::responseOnDelete()
{

}

std::pair<char *, int> Response::toFront()
{
//	std::string respons;
//	for (int i = 0; i < _autoindex->size(); ++i)
//	{
//		std::string line = (*_autoindex)[i];
//		respons += line;
//	}
//	char *res = new char[respons.size()];
	return std::pair<char *, int>(_fileSrc, _fileLength);
}

void Response::fileNotFound()
{
	std::ifstream file("../Network/html/404.html");
	if (file.fail())
		nullptr;
	else
	{
		std::string line, src;
		while (getline(file, line))
			src += line;
		_fileLength = src.length();
		_fileSrc = new char[_fileLength];
		for (int i = 0; i < _fileLength; ++i)
		{
			_fileSrc[i] = src[i];
		}
	}
}
