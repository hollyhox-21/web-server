#include "SocketBase.hpp"

SocketBase::SocketBase(const char *domain, int port) throw() {
	_address.sin_family = AF_INET;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = inet_addr(domain);

	std::cout << "Creating Socket" << std::endl;
	setSocket( _socket = socket(AF_INET, SOCK_STREAM, 0));
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
