#include "Client.hpp"

Client::Client(int socket) {
	_socket = socket;
	_read = true;
}

Client::~Client() {
	std::cout << _socket << "Ya umer!" << std::endl;
	// close(_socket);
}

std::string Client::recvMsg() {
	char buffer[BUFFER_SIZE];
	std::string myString;
	int nDataLength;

	do {
		nDataLength = recv(getSocket(), buffer, BUFFER_SIZE, 0);
		if (nDataLength == -1)
		{
			perror("recv");
			return (std::string(""));
		}
		myString.append(buffer, nDataLength);
	} while (nDataLength == BUFFER_SIZE);
	return myString;
}

void	Client::sendMsg(std::string const & value) {
	write(_socket, value.c_str(), value.length());
}

int		Client::getSocket() {
	return _socket;
}

bool	Client::getStage() {
	return _read;
}

void	Client::changeStage() {
	_read = !_read;
}


