#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <map>

struct Location
{
	std::string						path;
	std::map<std::string, bool>		methods;
	std::string						root;
	std::pair<std::string, std::string>	pathCgi;
	std::string						index;
	std::string						autoindex;
	int								limit_client_body_size;
	void reset(){
		methods = std::map<std::string, bool>();
		root = std::string();
		pathCgi.first = std::string();
		pathCgi.second = std::string();
		index = std::string();
		autoindex = std::string();
	}
	
	std::string getRoot() { return root; }
	std::pair<std::string, std::string> getPathCGI() { return pathCgi; }
	std::string getIndex() { return index; }
	std::string getAutoIndex() { return autoindex; }
	int			getLimit() { return limit_client_body_size; }
	std::map<std::string, bool> getMethods() { return methods; }
};

#endif