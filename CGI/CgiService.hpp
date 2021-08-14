#ifndef CGISERVICE_HPP
# define CGISERVICE_HPP

# include <sys/types.h>
# include <sys/wait.h>
# include <iostream>
# include <string.h>
# include "../Network/WServer.hpp"
# include "../Network/Models/Request.hpp"

class CgiService
{
	public:
		static std::string * getCgiResponse(Request & req, t_server & serverSettings);

	private:
		static char ** getCgiEnv(Request & req, t_server & serverSettings);

};

#endif