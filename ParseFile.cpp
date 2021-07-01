#include <vector>
#include "Network/ClientServer/Server.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <pthread.h>

void *runServer(void *serv) {
	((Server*)serv)->run();
	return NULL;
}


std::string pars_one_arg(std::string &line)
{
	line.erase(0, line.find(":") + 1);
	line.erase(0, line.find_first_not_of(" \t"));
	int i;
	if ((i = line.find(" ")) != -1)
	{
		std::string between = line;
		between.erase(0, i);
		between.erase(0, between.find_first_not_of(" "));
		if (!between.empty())
		{
			std::cout << "Error: wrong argument in config" << std::endl;
			return "";
		}
		else
			line.erase(i, line.length());
	}
	return line;
}

int main(int ac, char **av)
{
	std::vector<std::vector<std::string> > servs;
	if (ac != 2)
		return 0;
	std::ifstream file(av[1]);
	if (file.fail())
	{
		std::cout << "Error: file is wrong" << std::endl;
		return -1;
	}
	std::string line;
	while (1)
	{
		std::vector<std::string> ser;
		while (getline(file, line) && line.find("server:") == std::string::npos)
		{
			ser.push_back(line);
		}
		if (file.fail())
		{
			servs.push_back(ser);
			break;
		}
		if (line.find("server:") != std::string::npos)
		{
			if (ser.size())
				servs.push_back(ser);
			continue;
		}
	}
	std::vector<Server*> serv = std::vector<Server*>();
	for (unsigned long i = 0; i < servs.size(); ++i)
	{
		if (i == 3)
			NULL;
		int lflg = 0;
		std::string locPath;
		std::map<std::string, Location> location;
		std::map<int, std::string> erPage;
		Location loc;
		Server *s = new Server();
		for (unsigned long j = 0; j < servs[i].size(); ++j)
		{
			line = servs[i][j];
			if (line[0] == ' '){
				std::cout << "Space in config in line: " << i * j << std::endl;
			}
			if (line[0] == '\t' && line[1] != '\t')
			{
				line.erase(0, 1);
				if (line.find("server_name:") != std::string::npos)
				{
					line = pars_one_arg(line);
					s->setName(line);
				}
				else if (line.find("host:") != std::string::npos)
				{
					line = pars_one_arg(line);
					s->setHost(line);
				}
				else if (line.find("port:") != std::string::npos)
				{
					line = pars_one_arg(line);
					s->setPort(atoi(line.c_str()));
				}
				else if (line.find("error_page:") != std::string::npos)
				{
					std::string first, second;
					line.erase(0, line.find(":") + 1);
					line.erase(0, line.find_first_not_of(" \t"));
					int l;
					if ((l = line.find(" ")) != -1)
					{
						first = std::string(line);
						second = std::string(line);
						first.erase(l, line.length());
						second.erase(0, l + 1);
						second.erase(0, second.find_first_not_of(" \t"));
						if ((l = second.find_first_of(" \t")) != -1)
							second.erase(l, second.length());
					}
					erPage.insert(std::pair<int, std::string>(atoi(first.c_str()), second));
					s->setErrorPages(erPage);
				}
				else if (line.find("location:") != std::string::npos)
				{
					lflg = 1;
					line = pars_one_arg(line);
					locPath = line;
				}
			}
			else if (lflg == 1 && line[0] == '\t' && line[1] == '\t')
			{
				if (i == 3)
					NULL;
				if (line.find("root:") != std::string::npos)
				{
					line = pars_one_arg(line);
					loc.root = std::string(line);
				}
				else if (line.find("path_cgi:") != std::string::npos)
				{
					line = pars_one_arg(line);
					loc.pathCgi = std::string(line);
				}
				else if (line.find("index:") != std::string::npos)
				{
					line = pars_one_arg(line);
					loc.index = std::string(line);
				}
				else if (line.find("allow_methods:") != std::string::npos)
				{
					std::map<std::string, bool> methods = std::map<std::string, bool>();
					if (line.find("GET") != std::string::npos && loc.index != "")
						methods.insert(std::make_pair<std::string, bool>("GET", 1));
					else if (line.find("GET") != std::string::npos && loc.index == "")
						return -1;
					if (line.find("POST") != std::string::npos)
						methods.insert(std::make_pair<std::string, bool>("POST", 1));
					if (line.find("DELETE") != std::string::npos)
						methods.insert(std::make_pair<std::string, bool>("DELETE", 1));
					loc.methods = methods;
				}
			}
			if (i == 3 && j == 23)
				NULL;
			else if ((lflg == 1 && line.empty()) || (lflg == 1 && j == servs[i].size() - 1))
			{
				lflg = 0;
				location.insert(std::make_pair(locPath, loc));
				loc.reset();
				s->setLocations(location);
			}
		}
		serv.push_back(s);
	}
	pthread_t s;
	for (unsigned long i = 0; i < serv.size(); ++i)
	{
		std::cout << i << std::endl;
		serv[i]->ready();
		pthread_create(&s, NULL, &runServer, serv[i]);
	}
	file.close();
	while (1)
	 ;
	return 0;
}