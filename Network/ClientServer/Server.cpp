#include "Server.hpp"

void Server::ready () {
	_serverSocket = new ServerSocket(_host.c_str(), _port);
}

void Server::run () {
	std::cout << "Starting..." << _serverSocket->getSocket() << std::endl;
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
			if (_clients[i]->getState() == Client::HEADERS || _clients[i]->getState() == Client::BODY || _clients[i]->getState() == Client::END) {
				std::cout << "Stage reading..." << _clients[i]->getSocket() << std::endl;
				FD_SET(_clients[i]->getSocket(), &_readFds);
			} else if (_clients[i]->getState() == Client::SEND) {
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
				sendEvent(*_clients[i]);
			}
		}
		if (FD_ISSET(_serverSocket->getSocket(), &_readFds))
		{
			try {
				Client *client = new Client(_serverSocket->accept());
				_clients.push_back(client);
				connectEvent(*client);
			} catch(std::exception &e) {
				perror("accept");
				std::cout << e.what() << std::endl;
			}
		}
	}
}

void Server::connectEvent(Client & connection) {
	std::cout << "Connected " << connection.getSocket() << std::endl;
}
void Server::readEvent(Client & connection) {
	std::cout << "Reading " << connection.getSocket() << std::endl;
	Client::STATE ret = connection.recvMsg();
	if (ret == Client::SEND) {
		std::cout << "--------------------\nГотовый рек:\n";
		connection.getRequest()->printRequest();
		connection.getRequest()->printMap();
		// std::cout << "Body: " << connection.getRequest().getBody() << std::endl;
		std::cout << "--------------------\n";
		connection.setResponse(getSettings());
	}
	else if (ret == Client::CLOSE) {
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
void Server::sendEvent(Client & connection) {
	std::cout << "Msg: " << connection.getSocket() << std::endl;
	Client::STATE ret = connection.sendMsg();
	if (ret == Client::CLOSE) {
		for (size_t i = 0; i < _clients.size(); i++) {
			if (_clients[i]->getSocket() == connection.getSocket())
				disconnectEvent(connection, i);
		}
	} else if (ret == Client::ERROR) {
		exceptionEvent(connection, std::exception());
	}
}
void Server::exceptionEvent(Client & connection, std::exception e) {
	connection.getSocket();
	std::cout << "Error: " << e.what() << std::endl;
}

const std::map<int, std::string> &Server::getErrorPages() const
{
	return _errorPages;
}

const std::map<std::string, Location> &Server::getLocations() const
{
	return _locations;
}
