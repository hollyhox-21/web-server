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
	const char	*konec;
	if ((konec = strstr(_message.c_str(), (const char*)"\r\n\r\n")) != NULL) {
		std::string body = std::string(konec + 4);
		_message = _message.substr(0, konec + 4 - _message.c_str());
		std::cout << _message << std::endl;
		_req.parsRequest(_message, nDataLength);
		if (_req.getMethod() != "GET") {
			int		contentLenght = atoi(_req.getValueMapHeader(std::string("Content-Length")).c_str());
			char bufferBody[contentLenght];
			nDataLength = recv(getSocket(), bufferBody, contentLenght, 0);
			body.append(bufferBody, nDataLength);
			std::cout << "Body: " << body <<  " " << nDataLength << " " << contentLenght << std::endl;
		}
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


