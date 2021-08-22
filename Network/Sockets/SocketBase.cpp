#include "SocketBase.hpp"

SocketBase::SocketBase(const char *domain, int port) throw() {
	_address.sin_family = AF_INET;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = inet_addr(domain);
	std::cout << "Creating Socket" << std::endl;
	setSocket( _socket = socket(AF_INET, SOCK_STREAM, 0));
}

SocketBase::~SocketBase() {
	close(_socket);
}

struct sockaddr_in SocketBase::getAddress() {
	return _address;
}

int SocketBase::getSocket() {
	return _socket;
}

void SocketBase::setSocket(int socket) {
	int yes = 1;
	int flags;

	if ((flags = fcntl(socket, F_GETFL, 0)) < 0)
	    throw SocketException("Error F_GETFL");
	flags |= O_NONBLOCK;
	if (fcntl(socket, F_SETFL, flags) < 0)
	    throw SocketException("Error F_SETFL");
	if ((_socket = socket) < 0 || setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		throw SocketException("Socket didn't created");
}
