#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>

class Response
{
	private:
		std::string		_version;
		int				_statusCode;
		char			*_time;
		std::string		_name;

	public:
		Response(/* args */);
		~Response();
};

Response::Response(/* args */)
{
}

Response::~Response()
{
}


#endif