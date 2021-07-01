#include "Server.hpp"

void Server::ready () {
	_serverSocket = new ServerSocket(_host.c_str(), _port);
}

void Server::run () {
	std::cout << "Starting..." << std::endl;
	while (true)
	{
		FD_ZERO(&_readFds);
		FD_ZERO(&_writeFds);
		FD_SET(_serverSocket->getSocket(), &_readFds);
		_fdMax = _serverSocket->getSocket();
		for (unsigned long i = 0; i < _clients.size(); i++)
		{

			if (_fdMax < _clients[i]->getSocket())
				_fdMax = _clients[i]->getSocket();
			FD_SET(_clients[i]->getSocket(), &_readFds);
			if (_clients[i]->getStage() == true) {
				std::cout << "Stage reading..." << _clients[i]->getSocket() << std::endl;
			} else {
				std::cout << "Stage writing..." << _clients[i]->getSocket() << std::endl;
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
			} else if (FD_ISSET(_clients[i]->getSocket(), &_writeFds)) {
				sendEvent(*_clients[i], "");
			}
		}
		if (FD_ISSET(_serverSocket->getSocket(), &_readFds))
		{
			Client *client = new Client(_serverSocket->accept());
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
	if (ret == -2) {
		connection.getRequest().printRequest();
		connection.getRequest().printMap();
		connection.changeStage();
	}
	else if (ret == 0) {
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
	if (ret == 0) {
		for (size_t i = 0; i < _clients.size(); i++) {
			if (_clients[i]->getSocket() == connection.getSocket())
				disconnectEvent(connection, i);
		}
	}
	else {
		connection.changeStage();
	}
}
void Server::exceptionEvent(Client & connection, std::exception e) {
	connection.getSocket();
	std::cout << "Error: " << e.what() << std::endl;
}
