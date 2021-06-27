#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <map>

struct Location
{
	std::map<std::string, bool>	methods;
	std::string					root;
	std::string					pathCgi;
	std::string					index;
};


#endif