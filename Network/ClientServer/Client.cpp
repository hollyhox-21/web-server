#include "Client.hpp"

Client::Client(int socket) {
	_socket = socket;
	_read = true;
}

Client::~Client() {
	std::cout << _socket << "Ya umer!" << std::endl;
	close(_socket);
}

int		Client::recvMsg() {
	char buffer[BUFFER_SIZE];
	int nDataLength;

	nDataLength = recv(getSocket(), buffer, BUFFER_SIZE, 0);
	if (nDataLength == -1) {
		return (-1);
	}
	_message.append(buffer, nDataLength);
	bool end = true;
	for (size_t i = 0; i < 4; i++) {
		if (i % 2 == 0 && (int)_message[_message.length() - i - 1] == 10 && end == true)
			end = true;
		else if (i % 2 == 1 && (int)_message[_message.length() - i - 1] == 13 && end == true)
			end = true;
		else
			end = false;
	}
	if (end == true) {
		std::cout << std::endl << "Konec file" << std::endl;
		return -2;
	}
	return nDataLength;
}

int		Client::sendMsg(std::string const & value) {
	return send(_socket, value.c_str(), value.length(), 0);
}

void	Client::changeStage() {
	_read = !_read;
}

std::string		Client::getMessage() {
	return _message;
}

int		Client::getSocket() {
	return _socket;
}

bool	Client::getStage() {
	return _read;
}


