#ifndef CONNECTIONEXCEPTION_HPP
# define CONNECTIONEXCEPTION_HPP

#include <iostream>
#include <string.h>
#include <errno.h>

class ConnectionException : public std::exception {
	private:
		std::string		_message;
	public:
		ConnectionException(char *msg) throw() : std::exception() { _message = msg; }
		virtual const char* what() const throw() {
			std::string total = std::string("Connection Exception: ") + _message;
			const char *result = strdup(total.c_str());
			return result;
		}
};

#endif