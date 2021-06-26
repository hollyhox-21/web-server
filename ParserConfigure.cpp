#include <vector>
#include "Network/ClientServer/Server.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int main(int ac, char **av)
{
	std::vector<Server> serv = std::vector<Server>();
	if (ac != 2)
		return 0;
	std::ifstream file(av[1]);
	if (file.fail())
	{
		std::cout << "Error: file is wrong" << std::endl;
		return -1;
	}
	std::string line;
	int sflg = 0, lflg = 0;
	// Server s;
	while (getline(file, line))
	{
		if (line == "server:")
		{
			// if (sflg == 1)
				// serv.push_back(s);
			// s = Server();
			sflg = 1;
			lflg = 0;
			std::cout << "server find" << std::endl;
		}
		else if (sflg == 1 && line[0] == '\t')
		{
			lflg = 1;
			line.erase(0, 1);
			if (line.find("server_name:") != std::string::npos)
			{
				line.erase(0, line.find(":") + 1);
				line.erase(0, line.find_first_not_of(" \t"));
				std::cout << line << std::endl;
				int i;
				if ((i = line.find(" ")) != -1)
				{
					std::string between = line;
					between.erase(0, i);
					std::cout << between << std::endl;
					between.erase(0, between.find_first_not_of(" "));		
					std::cout << between << std::endl;
					if (!between.empty())
					{
						std::cout << "Error: wrong argument in config" << std::endl;
						return -1;
					}
					else
						line.erase(i, line.length());
					std::cout << line << std::endl;
				}
				std::cout << line << std::endl;
				// s.setName(line);
			}
			else if (line.find("host:") != std::string::npos)
			{
				line.erase(0, line.rfind(" ") + 1);				
				// s.setHost(line);
			}
		}
		else if (sflg == 1 && lflg == 1 && line[0] == '\t' && line[1] == '\t')
		{

		}
	}
	return 0;
}