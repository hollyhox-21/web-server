#include <vector>
#include "Response.hpp"

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
//	std::map<std::string, Location>::iterator it = _locations.begin();/**/
	for (std::map<std::string, Location>::iterator it = _locations.begin(); it != _locations.end(); ++it)
	{
		if (it->first.find(_request.getUri()) != std::string::npos)
		{
//			std::vector<std::string> autoindex;
			std::ifstream file(it->second.root + it->second.index);
			if (file.fail() && it->second.autoindex.find("on", 0, 2) != std::string::npos)
			{
				DIR *dir;
				struct dirent *ent;
				if ((dir = opendir(it->second.root.c_str())) != NULL) {
					std::ifstream file("/Users/ptycho/Documents/42cursus/web-server/autoidex.html");
					if (file.fail())
					{
						nullptr;
					}
					std::string one_line;
					while (getline(file, one_line))
					{
						if (one_line.find("<$ListOfFiles>") != std::string::npos)
							break;
						one_line.insert(one_line.size(), "\r\n");
						_autoindex->push_back(one_line);
//						std::cout << one_line;
					}
					while ((ent = readdir (dir)) != NULL) {
						std::string pre = "\t\t\t<li class=\"files\"><a href=\"";
						pre += it->first;
						pre += "\">";
						pre += ent->d_name;
						pre += "</a></li>\r\n";
						_autoindex->push_back(pre);
//						std::cout << pre;
					}
					while (getline(file, one_line))
					{
						one_line.insert(one_line.size(), "\r\n");
						_autoindex->push_back(one_line);
//						std::cout << one_line;
					}
				}
				else
				{
				}
//				throw Response::LocationIndexException();
			}
			else
			{

			}
//			_autoindex = &autoindex;
			it = _locations.end();
			--it;
		}
	}
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
}

char *Response::toFront()
{
	std::string respons;
	for (int i = 0; i < _autoindex->size(); ++i)
	{
		std::string line = (*_autoindex)[i];
		respons += line;
	}
//	char *res = new char[respons.size()];
	return const_cast<char *>(respons.c_str());
}
