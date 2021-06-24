#include "SocketBase.hpp"

SocketBase::SocketBase(int domain, int type, int protocol, int port, u_long interface) throw() {
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);

	std::cout << "Creating Socket" << std::endl;
	setSocket( _socket = socket(domain, type, protocol));
}

SocketBase::~SocketBase() { }

struct sockaddr_in SocketBase::getAddress() {
	return _address;
}

int SocketBase::getSocket() {
	return _socket;
}

void SocketBase::setSocket(int socket) {
	if ((_socket = socket) < 0)
		throw SocketException(strerror(errno));
}
