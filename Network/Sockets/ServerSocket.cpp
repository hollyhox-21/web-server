#include "ServerSocket.hpp"

ServerSocket::ServerSocket(int domain, int type, int protocol, int port, u_long interface, int backlog)
	throw() : SocketBase(domain, type, protocol, port, interface) {
	_backlog = backlog;
	try {
		std::cout << "Connecting" << std::endl;
		connect();
	}
	catch (std::exception const & e) {
		std::cout << e.what() << std::endl;
	}
	try {
		std::cout << "Listening" << std::endl;
		startListening();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
}

ServerSocket::~ServerSocket() { }

void	ServerSocket::connect() {
	sockaddr_in address = getAddress();
	if (bind(getSocket(), (struct sockaddr*)&address, sizeof(address)))
		throw ConnectionException(strerror(errno));
}

void	ServerSocket::startListening() {
	if (listen(getSocket(), _backlog))
		throw ListenException(strerror(errno));
}

int		ServerSocket::accept() {
	sockaddr_in address = getAddress();
	int newSocket = ::accept(getSocket(), (struct sockaddr*)&address, (socklen_t *)&address);
	if (newSocket < 0)
		throw SocketException(strerror(errno));
	return newSocket;
}