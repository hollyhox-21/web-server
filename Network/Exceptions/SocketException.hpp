#ifndef SOCKETEXCEPTION_HPP
# define SOCKETEXCEPTION_HPP

#include <iostream>
#include <string.h>
#include <errno.h>

class SocketException : public std::exception {
	private:
		char			*_result;
	public:
		SocketException(const char *msg) throw() : std::exception() {
			std::string total = std::string("Socket Exception: ") + msg;
			_result = strdup(total.c_str());
		}
		virtual ~SocketException() throw() {
			delete(_result);
		}
		virtual const char* what() const throw() {
			return _result;
		}
};

#endif