#ifndef LISTENEXCEPTION_HPP
# define LISTENEXCEPTION_HPP

#include <iostream>
#include <string.h>
#include <errno.h>

class ListenException : public std::exception {
	private:
		char			*_result;
	public:
		ListenException(const char *msg) throw() : std::exception() {
			std::string total = std::string("Listen Exception: ") + msg;
			_result = strdup(total.c_str());
		}
		virtual ~ListenException() throw() {
			delete(_result);
		}
		virtual const char* what() const throw() {
			return _result;
		}
};

#endif