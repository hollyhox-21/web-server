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
           for (int j = 0; j < servs[i].size(); ++j)
		   {
           		std::string line = servs[i][j];
			   if (line == "server:")
			   {
				   // if (sflg == 1)
				   // {
				   // 	s.setLocations(location);
				   // 	serv.push_back(s);
				   // }
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
					   line = pars_one_arg(line);
					   std::cout << line << std::endl;
					   // s.setName(line);
				   }
				   else if (line.find("host:") != std::string::npos)
				   {
					   line = pars_one_arg(line);
					   std::cout << line << std::endl;
					   // s.setHost(line);
				   }
				   else if (line.find("port:") != std::string::npos)
				   {
					   line = pars_one_arg(line);
					   std::cout << line << std::endl;
					   // s.setPort(atoi(const_cast<char*>(line.c_str())));
				   }
				   else if (line.find("location:") != std::string::npos)
				   {
					   lflg = 1;
					   line = pars_one_arg(line);
					   std::cout << line << std::endl;
					   locPath = line;
				   }
			   }
			   else if (sflg == 1 && lflg == 1 && line[0] == '\t' && line[1] == '\t')
			   {
			   }
		   }
	}
    }
    return 0;
}