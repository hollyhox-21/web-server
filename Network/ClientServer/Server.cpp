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
//			FD_ZERO(&_readFds);
//			FD_ZERO(&_writeFds);
//			FD_SET(_serverSocket.getSocket(), &_readFds);
//			_fdMax = _serverSocket.getSocket();
//			for (unsigned long i = 0; i < _clients.size(); i++)
//			{
//				if (_fdMax < _clients[i]->getSocket())
//					_fdMax = _clients[i]->getSocket();
//				FD_SET(_clients[i]->getSocket(), &_readFds);
//				if (_clients[i]->getStage() == true) {
//					std::cout << "Stage reading..." << _clients[i]->getSocket() << std::endl;
//				} else {
//					std::cout << "Stage writing..." << _clients[i]->getSocket() << std::endl;
//					_clients[i]->changeStage();
//					FD_SET(_clients[i]->getSocket(), &_writeFds);
//				}
//			}
//			std::cout << "select" << std::endl;
//			if (select(_fdMax + 1, &_readFds, &_writeFds, NULL, NULL) == -1) {
//				perror("select");
//				_exit(4);
//			}
//			for (unsigned long i = 0; i < _clients.size(); i++) {
//				if (FD_ISSET(_clients[i]->getSocket(), &_readFds)) {
//					readEvent(*_clients[i]);
//					_clients[i]->changeStage();
//				} else if (FD_ISSET(_clients[i]->getSocket(), &_writeFds)) {
//					sendEvent(*_clients[i], "");
//				}
//			}
//			if (FD_ISSET(_serverSocket.getSocket(), &_readFds))
//			{
//				Client *client = new Client(_serverSocket.accept());
//				_clients.push_back(client);
//				connectEvent(*client);
//			}
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
		connection.sendMsg("HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 12\nContent-Type: text/html\nClient: Closed\n\nHello world!");
	}
	void Server::exceptionEvent(Client & connection, std::exception e) {
		connection.getSocket();
		std::cout << "Error: " << e.what() << std::endl;
	}

void Server::setServerSocket(ServerSocket &serverSocket)
{
	_serverSocket = serverSocket;
}

void Server::setName(std::string name)
{
	_name = name;
}

void Server::setHost(std::string host)
{
	_host = host;
}

void Server::setPort(int port)
{
	_port = port;
}

void Server::setErrorPages(std::map<int, std::string> &errorPage)
{
	_errorPages = errorPage;
}

void Server::setClients(std::vector<Client *> &clients)
{
	_clients = clients;
}

void Server::setLocations(std::map<std::string, Location> &location)
{
	_locations = location;
}

void Server::connectEvent(Client connection)
{

}

void Server::disconnectEvent(Client connection)
{

}

void Server::readEvent(Client connection)
{

}

void Server::sendEvent(Client connection, std::string value)
{

}

void Server::exceptionEvent(Client connection, std::exception e)
{

}

const ServerSocket &Server::getServerSocket() const
{
	return _serverSocket;
}

const std::string &Server::getName() const
{
	return _name;
}

const std::string &Server::getHost() const
{
	return _host;
}

int Server::getPort() const
{
	return _port;
}

const std::map<int, std::string> &Server::getErrorPages() const
{
	return _errorPages;
}

const std::vector<Client *> &Server::getClients() const
{
	return _clients;
}

const std::map<std::string, Location> &Server::getLocations() const
{
	return _locations;
}

const fd_set &Server::getReadFds() const
{
	return _readFds;
}

const fd_set &Server::getWriteFds() const
{
	return _writeFds;
}

int Server::getFdMax() const
{
	return 0;
}

Server::Server(): _serverSocket(), _port(), _name(), _host()
{

}
