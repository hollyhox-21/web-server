#include "ServerSocket.hpp"

ServerSocket::ServerSocket(const char *domain, int port)
	throw() : SocketBase(domain, port) {
	int yes = 1;
	setsockopt(getSocket(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
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
		std::cout << "" << std::endl;
		// throw ListenException(strerror(errno));
}

int		ServerSocket::accept() {
	sockaddr_in address = getAddress();
	int newSocket = ::accept(getSocket(), (struct sockaddr*)&address, (socklen_t *)&address);
	if (newSocket < 0)
		std::cout << "" << std::endl;
		// throw SocketException(strerror(errno));
	return newSocket;
}

ServerSocket::ServerSocket(const ServerSocket &s)
{

}
