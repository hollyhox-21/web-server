#include "Client.hpp"

Client::Client(int socket) {
	_socket = socket;
	_read = true;
}

Client::~Client() {
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
		// std::cout << _message << std::endl;
		_req.parsRequest(_message);
		if (_req.getMethod() != "GET") {
			int		contentLenght = atoi(_req.getValueMapHeader(std::string("Content-Length")).c_str());
			char bufferBody[contentLenght];
			nDataLength = recv(getSocket(), bufferBody, contentLenght, 0);
			body.append(bufferBody, nDataLength);
			_req.parsBody(body);
		}
		return -2;
	}
	return nDataLength;
}

int		Client::sendMsg() {
	int result = send(_socket, _res->toFront().first, _res->toFront().second, 0);
	delete(_res);
	return result;
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

void	Client::setResponse(std::map<int, std::string> & errPage, std::map<std::string, Location> & locations) {
	_res = new Response(_req, errPage, locations);
	std::cout << "\n\nResponse\n";
	write(1, _res->toFront().first, _res->toFront().second);
}
