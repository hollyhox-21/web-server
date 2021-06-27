#include <vector>
 #include "Network/ClientServer/Server.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

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
	int fser = 0;
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
	std::vector<Server> serv = std::vector<Server>();
	for (int i = 0; i < servs.size(); ++i)
	{
		int lflg = 0;
		std::string locPath;
		std::map<std::string, Location> location;
		Location loc;
		Server s;
		for (int j = 0; j < servs[i].size(); ++j)
		{
			std::string line = servs[i][j];
			if (line[0] == '\t' && line[1] != '\t')
			{
				line.erase(0, 1);
				if (line.find("server_name:") != std::string::npos)
				{
					line = pars_one_arg(line);
					std::cout << line << std::endl;
					s.setName(line);
				}
				else if (line.find("host:") != std::string::npos)
				{
					line = pars_one_arg(line);
					std::cout << line << std::endl;
					 s.setHost(line);
				}
				else if (line.find("port:") != std::string::npos)
				{
					line = pars_one_arg(line);
					std::cout << line << std::endl;
					 s.setPort(atoi(const_cast<char*>(line.c_str())));
				}
				else if (line.find("location:") != std::string::npos)
				{
					lflg = 1;
					line = pars_one_arg(line);
					std::cout << line << std::endl;
					locPath = line;
				}
			}
			else if (lflg == 1 && line[0] == '\t' && line[1] == '\t')
			{
				if (line.find("root:") != std::string::npos)
				{
					line = pars_one_arg(line);
					std::cout << line << std::endl;
					loc.root = line;
					 s.setHost(line);
				}
				else if (line.find("path_cgi:") != std::string::npos)
				{
					line = pars_one_arg(line);
					std::cout << line << std::endl;
					loc.pathCgi = line;
					 s.setHost(line);
				}
				else if (line.find("index:") != std::string::npos)
				{
					line = pars_one_arg(line);
					std::cout << line << std::endl;
					loc.index = line;
					 s.setHost(line);
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
					std::cout << line << std::endl;
					loc.methods = methods;
				}
			}
			else if (lflg == 1 && line == "")
			{
				lflg = 0;
				location.insert(std::make_pair(locPath, loc));
			}
		}
	}
	return 0;
}