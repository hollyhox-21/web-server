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

//void printConfigFile(std::vector<Server*> serv)
//{
//	for (int i = 0; i < serv.size(); ++i)
//	{
//		std::cout << "server:" << std::endl;
//		std::cout << "\tserver_name: " << serv[i]->getName() << std::endl;
//		std::cout << "\thost: " << serv[i]->getHost() << std::endl;
//		std::cout << "\tport: " << serv[i]->getPort() << std::endl;
//		std::map<int, std::string> ep = serv[i]->getErrorPages();
//		std::map<int, std::string>::iterator ite = ep.begin();
//		while (ite != ep.end())
//			std::cout << "\terror_page: " << ite->first << " " << ite++->second << std::endl;
//		std::map<std::string, Location> ms = serv[i]->getLocations();
//		std::map<std::string, Location>::iterator itl = ms.begin();
//		while (itl != ms.end())
//		{
//			std::cout << "\tlocation: " << itl->first << std::endl;
//			std::cout << "\t\tindex: " << itl->second.index << std::endl;
//			std::cout << "\t\tpath_cgi: " << itl->second.pathCgi << std::endl;
//			std::cout << "\t\troot: " << itl->second.root << std::endl;
//			std::map<std::string, bool> met = itl->second.methods;
//			std::map<std::string, bool>::iterator itm = met.begin();
//			if (met.size())
//				std::cout << "\t\tallow_methods: ";
//			while (itm != met.end())
//				std::cout << itm++->first << " ";
//			if (met.size())
//				std::cout << std::endl;
//			itl++;
//		}
//	}
//}

std::vector<Server*> parser(std::string path)
{
	std::vector<std::vector<std::string> > servs;
	std::vector<Server *> serv = std::vector<Server *>();
	std::string line;
	std::ifstream file(path);
	if (file.fail())
	{
		std::cout << "Error: file is wrong" << std::endl;
	}
	else
	{
		while (1)
		{
			std::vector<std::string> ser;
			while (getline(file, line) &&
				   line.find("server:") == std::string::npos)
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
		for (int i = 0; i < servs.size(); ++i)
		{
			if (i == 3)
				NULL;
			int lflg = 0;
			std::string locPath;
			std::map<std::string, Location> location;
			std::map<int, std::string> erPage;
			Location loc;
			Server *s = new Server();
			for (int j = 0; j < servs[i].size(); ++j)
			{
				line = servs[i][j];
				if (line[0] == ' ')
				{
					std::cout << "Space in config in line: " << i * j
							  << std::endl;
				}
				if (line[0] == '\t' && line[1] != '\t')
				{
					line.erase(0, 1);
					if (line.find("server_name:") != std::string::npos)
					{
						line = pars_one_arg(line);
						s->setName(line);
					} else if (line.find("host:") != std::string::npos)
					{
						line = pars_one_arg(line);
						s->setHost(line);
					} else if (line.find("port:") != std::string::npos)
					{
						line = pars_one_arg(line);
						s->setPort(atoi(line.c_str()));
					} else if (line.find("error_page:") != std::string::npos)
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
						erPage.insert(
								std::pair<int, std::string>(atoi(first.c_str()),
															second));
						s->setErrorPages(erPage);
					} else if (line.find("location:") != std::string::npos)
					{
						lflg = 1;
						line = pars_one_arg(line);
						locPath = line;
					}
				} else if (lflg == 1 && line[0] == '\t' && line[1] == '\t')
				{
					if (i == 3)
						NULL;
					if (line.find("root:") != std::string::npos)
					{
						line = pars_one_arg(line);
						loc.root = std::string(line);
					} else if (line.find("path_cgi:") != std::string::npos)
					{
						line = pars_one_arg(line);
						loc.pathCgi = std::string(line);
					} else if (line.find("index:") != std::string::npos)
					{
						line = pars_one_arg(line);
						loc.index = std::string(line);
					} else if (line.find("allow_methods:") != std::string::npos)
					{
						std::map<std::string, bool> methods = std::map<std::string, bool>();
						if (line.find("GET") != std::string::npos &&
							loc.index != "")
							methods.insert(
									std::make_pair<std::string, bool>("GET",
																	  1));
						else if (line.find("GET") != std::string::npos &&
								 loc.index == "")
							std::cout << "No indes for GET" << std::endl;
						if (line.find("POST") != std::string::npos)
							methods.insert(
									std::make_pair<std::string, bool>("POST",
																	  1));
						if (line.find("DELETE") != std::string::npos)
							methods.insert(
									std::make_pair<std::string, bool>("DELETE",
																	  1));
						loc.methods = methods;
					}
				}
				else if ((lflg == 1 && line.empty()) ||
						 (lflg == 1 && j == servs[i].size() - 1))
				{
					lflg = 0;
					location.insert(std::make_pair(locPath, loc));
					loc.reset();
					s->setLocations(location);
				}
			}
			serv.push_back(s);
		}
	}
	file.close();
	return serv;
}

int main(int ac, char **av)
{
	std::vector<Server*> serv = parser(av[1]);
	printConfigFile(serv);
	return 0;
}