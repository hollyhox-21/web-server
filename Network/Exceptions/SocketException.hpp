#ifndef SOCKETEXCEPTION_HPP
# define SOCKETEXCEPTION_HPP

#include <iostream>
#include <string.h>
#include <errno.h>

//class SocketException : public std::exception {
//	private:
//		std::string		_message;
//	public:
//		SocketException(char *msg) throw() : std::exception() { _message = msg; }
//		virtual const char* what() const throw() {
//			std::string total = std::string("Socket Exception: ") + _message;
//			const char *result = strdup(total.c_str());
//			return result;
//		}
//};

#endif