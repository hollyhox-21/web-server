#include "Server.hpp"

Server::Server(std::string const & host, int port, std::map<int, std::string> errorPages, std::map<std::string, Location> locations) :
	_serverSocket(ServerSocket(host.c_str(), port)) {
		_host = host;
		_port = port;
		_errorPages = errorPages;
		_locations = locations;
	}

	void Server::run () {
		std::cout << "Starting..." << std::endl;
		while (true)
		{
			FD_ZERO(&_readFds);
			// FD_SET(_serverSocket.getSocket(), &_read_fds);
			// for (unsigned long i = 0; i < _clients.size(); i++)
			// {
			// 	FD_SET(_clients[i]->getSocket(), &_read_fds);
			// }


			
			_clients.push_back(new Client(_serverSocket.accept(), this));
		}
	}

	void Server::connectEvent(Client connection) {
		connection.getSocket();
		std::cout << "Connected" << std::endl;



		// Я тут считываю в баффер
		char buffer[30000];
		recv(connection.getSocket(), buffer, 30000, 0);
		std::cout << buffer << std::endl;
		// А тут генерирую реквест
		std::string b(buffer);
		// Request r(b);
		sendEvent(connection, "message");
	}
	void Server::disconnectEvent(Client connection) {
		close(connection.getSocket());
		std::cout << "Disconnected" << std::endl;
	}
	void Server::sendEvent(Client connection, std::string value) {
		connection.sendMsg("HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 12\nContent-Type: text/html\nClient: Closed\n\nHello world!");
		std::cout << "Msg: " << value << std::endl;
		disconnectEvent(connection);
	}
	void Server::exceptionEvent(Client connection, std::exception e) {
		connection.getSocket();
		std::cout << "Error: " << e.what() << std::endl;
	}