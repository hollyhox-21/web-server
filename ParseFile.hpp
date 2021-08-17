#ifndef PARSEFILE_HPP
# define PARSEFILE_HPP
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <algorithm>
#include <pthread.h>
#include <cstdlib>
#include "Network/Network.hpp"

	int	startParser(char *fileName, std::vector<t_server> &servers);
	void	printLocations(std::map<std::string, Location> locations);
	void	printServ(t_server serv);

#endif
