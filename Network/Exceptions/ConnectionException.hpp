#ifndef CONNECTIONEXCEPTION_HPP
# define CONNECTIONEXCEPTION_HPP

#include <iostream>
#include <string.h>
#include <errno.h>

class ConnectionException : public std::exception {
	private:
		char			*_result;
	public:
		ConnectionException(const char *msg) throw() : std::exception() {
			std::string total = std::string("Connection Exception: ") + msg;
			_result = strdup(total.c_str());
		}
		virtual ~ConnectionException() throw() {
			delete(_result);
		}
		virtual const char* what() const throw() {
			return _result;
		}
};

#endif