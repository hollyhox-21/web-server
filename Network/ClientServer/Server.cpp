#include "Server.hpp"

Server::Server(std::string const & host, int port) :
	_serverSocket(ServerSocket(host.c_str(), port)) {
		_host = host;
		_port = port;
		_fdMax = _serverSocket.getSocket();
	}

Server::Server(std::string const & host, int port, std::map<int, std::string> errorPages, std::map<std::string, Location> locations) :
	_serverSocket(ServerSocket(host.c_str(), port)) {
		_host = host;
		_port = port;
		_errorPages = errorPages;
		_locations = locations;
		_fdMax = _serverSocket.getSocket();
	}

	void Server::run () {
		std::cout << "Starting..." << std::endl;
		while (true)
		{
			FD_ZERO(&_readFds);
			FD_ZERO(&_writeFds);
			FD_SET(_serverSocket.getSocket(), &_readFds);
			_fdMax = _serverSocket.getSocket();
			for (unsigned long i = 0; i < _clients.size(); i++)
			{
				if (_fdMax < _clients[i]->getSocket())
					_fdMax = _clients[i]->getSocket();
				FD_SET(_clients[i]->getSocket(), &_readFds);
				if (_clients[i]->getStage() == true) {
					std::cout << "Stage reading..." << _clients[i]->getSocket() << std::endl;
				} else {
					std::cout << "Stage writing..." << _clients[i]->getSocket() << std::endl;
					_clients[i]->changeStage();
					FD_SET(_clients[i]->getSocket(), &_writeFds);
				}
			}
			std::cout << "select" << std::endl;
			if (select(_fdMax + 1, &_readFds, &_writeFds, NULL, NULL) == -1) {
				perror("select");
				_exit(4);
			}
			for (unsigned long i = 0; i < _clients.size(); i++) {
				if (FD_ISSET(_clients[i]->getSocket(), &_readFds)) {
					readEvent(*_clients[i]);
					_clients[i]->changeStage();
				} else if (FD_ISSET(_clients[i]->getSocket(), &_writeFds)) {
					sendEvent(*_clients[i], "");
				}
			}
			if (FD_ISSET(_serverSocket.getSocket(), &_readFds))
			{
				Client *client = new Client(_serverSocket.accept());
				_clients.push_back(client);
				connectEvent(*client);
			}
		}
	}

	void Server::connectEvent(Client & connection) {
		std::cout << "Connected " << connection.getSocket() << std::endl;
	}
	void Server::readEvent(Client & connection) {
		std::cout << "Reading " << connection.getSocket() << std::endl;
		int ret = connection.recvMsg();
		if (ret == -2)
			std::cout << connection.getMessage();
		else if (ret == 0)
		{
			for (size_t i = 0; i < _clients.size(); i++) {
				if (_clients[i]->getSocket() == connection.getSocket())
					disconnectEvent(connection, i);
			}
		}
	}
	void Server::disconnectEvent(Client & connection, int index) {
		std::cout << "Disconnected " << connection.getSocket() << std::endl;
		delete(_clients[index]);
		_clients.erase(_clients.begin() + index);
	}
	void Server::sendEvent(Client & connection, std::string value) {
		std::cout << "Msg: " << connection.getSocket() << value << std::endl;
		int ret = connection.sendMsg("HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 12\nContent-Type: text/html\nClient: Closed\n\nHello world!");
		if (ret == 0)
		{
			for (size_t i = 0; i < _clients.size(); i++) {
				if (_clients[i]->getSocket() == connection.getSocket())
					disconnectEvent(connection, i);
			}
		}
	}
	void Server::exceptionEvent(Client & connection, std::exception e) {
		connection.getSocket();
		std::cout << "Error: " << e.what() << std::endl;
	}