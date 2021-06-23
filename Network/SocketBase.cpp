#include "SocketBase.hpp"

SocketBase::SocketBase(int domain, int type, int protocol, int port, u_long interface) {
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);

	_socket = socket(domain, type, protocol);
	testConnection();

	_connection = connect(_socket, _address);
}

void SocketBase::testConnection() {
	if (socket < 0)
		throw new std::exception();
}


