#include <vector>
// #include "Network/ClientServer/Server.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int main(int ac, char **av)
{
    std::vector< std::vector<std::string> > servs;
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
            // std::cout << line << std::endl;
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
           std::cout << servs[i].size() << std::endl;
           for (int j = 0; j < servs[i].size(); ++j)
           {
               std::cout << servs[i][j] << std::endl;
           }
    }
    return 0;
}