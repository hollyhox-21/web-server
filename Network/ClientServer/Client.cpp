#include "Client.hpp"

Client::Client(int socket, IEventHandler *evntHnd) {
	_socket = socket;
	_eventHandler = evntHnd;
	_eventHandler->connectEvent(*this);
}

Client::~Client()
{
}

void	Client::sendMsg(std::string const & value) {
	write(_socket, value.c_str(), value.length());
}


int Client::getSocket() {
	return _socket;
}


