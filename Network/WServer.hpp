#ifndef WSERVER_HPP
# define WSERVER_HPP

# include <map>
# include "Models/Location.hpp"

typedef struct	s_server {
	std::string						name;
	std::string						host;
	int								port;
	std::map<int, std::string>		errorPages;
	std::map<std::string, Location>	locations;
}				t_server;

#endif