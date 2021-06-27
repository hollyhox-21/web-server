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
				if (_clients[i]->getStage() == true) {
					std::cout << "Stage reading..." << _clients[i]->getSocket() << std::endl;
					FD_SET(_clients[i]->getSocket(), &_readFds);
					_clients[i]->changeStage();
				} else {
					std::cout << "Stage writing..." << _clients[i]->getSocket() << std::endl;
					_clients[i]->changeStage();
					FD_SET(_clients[i]->getSocket(), &_writeFds);
				}
			}
			std::cout << "select" << std::endl;
			if (select(_fdMax + 1, &_readFds, &_writeFds, NULL, NULL) == -1) {
				perror("select");
				exit(4);
			}
			for (unsigned long i = 0; i < _clients.size(); i++) {
				if (FD_ISSET(_clients[i]->getSocket(), &_readFds)) {
					readEvent(*_clients[i]);
				} else if (FD_ISSET(_clients[i]->getSocket(), &_writeFds)) {
					sendEvent(*_clients[i], "");
					disconnectEvent(*_clients[i], i);
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
		std::cout << connection.recvMsg();
	}
	void Server::disconnectEvent(Client & connection, int index) {
		std::cout << "Disconnected " << connection.getSocket() << std::endl;
		delete(_clients[index]);
		_clients.erase(_clients.cbegin() + index);
	}
	void Server::sendEvent(Client & connection, std::string value) {
		std::cout << "Msg: " << connection.getSocket() << value << std::endl;
		connection.sendMsg("HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 12\nContent-Type: text/html\nClient: Closed\n\nHello world!");
	}
	void Server::exceptionEvent(Client & connection, std::exception e) {
		connection.getSocket();
		std::cout << "Error: " << e.what() << std::endl;
	}