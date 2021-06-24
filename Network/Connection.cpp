#include "Connection.hpp"

Connection::Connection(int socket, IEventHandler *evntHnd) {
	_socket = socket;
	_eventHandler = evntHnd;
	_eventHandler->connectEvent(*this);
}

Connection::~Connection()
{
}

void	Connection::sendMsg(std::string const & value) {
	write(_socket, value.c_str(), value.length());
}


int Connection::getSocket() {
	return _socket;
}


