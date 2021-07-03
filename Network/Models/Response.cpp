#include <vector>
#include "Response.hpp"
#include <fcntl.h>

Response::~Response()
{
}

Response::Response(Request &request, std::map<int, std::string> &errorPage, std::map<std::string, Location> &locations) : _request(request), _errorPage(errorPage), _locations(locations)
{
	_autoindex = new std::vector<std::string>();
	if (request.getMethod().find("GET", 0, 3) != std::string::npos)
		responseOnGet();
	else if (request.getMethod().find("POST", 0, 4) != std::string::npos)
		responseOnPost();
	else if (request.getMethod().find("DELETE", 0, 6) != std::string::npos)
		responseOnDelete();
}

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
				int ret;
				struct stat buf;
				if ((ret = ::stat(uri.c_str(), &buf)) != 0)
				{
					std::cout << ret << std::endl;
				}
				if (buf.st_mode & S_IFREG)
				{
					int fd = open(uri.c_str(), O_RDONLY);
					if (fd < 0)
						nullptr;
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
								nullptr;
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
//		std::string dir;
//		std::string uri = _request.getUri();
//		if (uri.rfind('/') == (uri.length() - 1))
//			uri.erase(uri.length() - 1);
//		dir = uri;
//		while (!dir.empty())
//		{
//			for (std::map<std::string, Location>::iterator it = _locations.begin(); it != _locations.end(); ++it)
//			{
//				if (it->first.find(dir) != std::string::npos)
//				{
//					if (it->second.autoindex.find("on", 0, 2) != std::string::npos)
//					{
//						DIR *dir;
//						struct dirent *ent;
//						if ((dir = opendir(it->second.root.c_str())) != NULL)
//						{
//							std::ifstream file("../Network/html/autoindex.html");
//							if (file.fail())
//							{
//								nullptr;
//							}
//							std::string one_line;
//							while (getline(file, one_line))
//							{
//								if (one_line.find("<$ListOfFiles>") !=
//									std::string::npos)
//									break;
//								one_line.insert(one_line.size(), "\r\n");
//								_autoindex->push_back(one_line);
//							}
//							while ((ent = readdir(dir)) != NULL)
//							{
//								std::string pre = "\t\t\t<li class=\"files\"><a href=\"";
//								pre += it->first;
//								pre += "\">";
//								pre += ent->d_name;
//								pre += "</a></li>\r\n";
//								_autoindex->push_back(pre);
//							}
//							while (getline(file, one_line))
//							{
//								one_line.insert(one_line.size(), "\r\n");
//								_autoindex->push_back(one_line);
//							}
//						} else
//						{
//						}
////				throw Response::LocationIndexException();
//					}
//					break;
//				}
//			}
//		}
//		std::map<std::string, Location>::iterator it = _locations.begin();
}

void Response::responseOnPost()
{

}

void Response::responseOnDelete()
{

}

void Response::reple(char *source, char *dest)
{
//	std::ifstream file("/Users/ptycho/Documents/42cursus/web-server/autoindex.html");
//	if (file.fail())
//	{
//		nullptr;
//	}
//	std::string nefl = str;
//	nefl += ".replace";
//	std::ofstream ofl(nefl);
//	std::string s1 = av[2], s2 = av[3], one_line;
//	while (getline(file, one_line))
//	{
//		size_t found;
//		size_t startSearchPos = 0;
//		while ((found = (one_line.substr(startSearchPos).find(s1))) != std::string::npos)
//		{
//			one_line.replace(startSearchPos + found, s1.length(), s2);
//			startSearchPos += s2.length() + found;
//		}
//		ofl << one_line << "\n";
//	}
//	file.close();
//	ofl.close();

//	std::ifstream file(it->second.root + it->second.index);
//	if (it->second.autoindex.find("on", 0, 2) != std::string::npos)
//	{
//		DIR *dir;
//		struct dirent *ent;
//		if ((dir = opendir(it->second.root.c_str())) != NULL)
//		{
//			std::ifstream file("../Network/html/autoindex.html");
//			if (file.fail())
//			{
//				nullptr;
//			}
//			std::string one_line;
//			while (getline(file, one_line))
//			{
//				if (one_line.find("<$ListOfFiles>") !=
//					std::string::npos)
//					break;
//				one_line.insert(one_line.size(), "\r\n");
//				_autoindex->push_back(one_line);
//			}
//			while ((ent = readdir(dir)) != NULL)
//			{
//				std::string pre = "\t\t\t<li class=\"files\"><a href=\"";
//				pre += it->first;
//				pre += "\">";
//				pre += ent->d_name;
//				pre += "</a></li>\r\n";
//				_autoindex->push_back(pre);
//			}
//			while (getline(file, one_line))
//			{
//				one_line.insert(one_line.size(), "\r\n");
//				_autoindex->push_back(one_line);
//			}
//		} else
//		{
//		}
////				throw Response::LocationIndexException();
//	} else{}
//	break;

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
