#ifndef LISTENEXCEPTION_HPP
# define LISTENEXCEPTION_HPP

#include <iostream>
#include <string.h>
#include <errno.h>

//class ListenException : public std::exception {
//	private:
//		std::string		_message;
//	public:
//		ListenException(char *msg) throw() : std::exception() { _message = msg; }
//		virtual const char* what() const throw() {
//			std::string total = std::string("Listen Exception: ") + _message;
//			const char *result = strdup(total.c_str());
//			return result;
//		}
//};

#endif