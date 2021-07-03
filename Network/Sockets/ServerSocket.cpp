#include "ServerSocket.hpp"

ServerSocket::ServerSocket(const char *domain, int port)
	throw() : SocketBase(domain, port) {
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
		std::cout << "" << std::endl;
		// throw ConnectionException(strerror(errno));
}

void	ServerSocket::startListening() {
	if (listen(getSocket(), SOMAXCONN))
		throw ListenException("Bad socket");
}

int		ServerSocket::accept() {
	sockaddr_in address = getAddress();
	socklen_t len = sizeof (address);
	int newSocket = ::accept(getSocket(), (struct sockaddr*)&address, &len);
	if (newSocket < 0)
		throw ConnectionException("Bad socket");
	return newSocket;
}
