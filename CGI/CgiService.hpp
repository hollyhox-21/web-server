#ifndef CGISERVICE_HPP
# define CGISERVICE_HPP

# include <sys/types.h>
# include <sys/wait.h>
# include <iostream>
# include "../Network/Models/Request.hpp"

class CgiService
{
	public:
		static std::string * getCgiResponse(Request & req);

	private:
		static char ** getCgiEnv(Request & req);

};

#endif