#include "Client.hpp"

Client::Client(int socket) {
	_socket = socket;
	_read = true;
}

Client::~Client() {
	close(_socket);
}

void	Client::recvChunked() {
	int				nDataLength;
	long			contentLenght = 0;
	char			*p;
	char			buffer[BUFFER_SIZE];
	std::string		bufferLenght;
	std::string		body;
	const char		*konec;


	do {
		do {
			nDataLength = recv(getSocket(), buffer, BUFFER_SIZE, 0);
			bufferLenght.append(buffer);
		} while ((konec = strstr(_message.c_str(), (const char*)"\r\n")) != NULL);
		bufferLenght = bufferLenght.substr(0, konec - bufferLenght.c_str());
		contentLenght = strtol(bufferLenght.c_str(), & p, 16);
		char	bufferBody[contentLenght + 2];
		nDataLength = recv(getSocket(), bufferBody, contentLenght + 2, 0);
		body.append(konec + 2);
		body.append(bufferBody);
	} while (contentLenght > 0);
	_req.parsBody(body);
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
		_req.parsRequest(_message);
		if (_req.getMethod() != "GET") {
			if (_req.getValueMapHeader("Transfer-Encoding") == "chunked") {
				recvChunked();
			}
			else if (_req.getValueMapHeader("Content-Length") == "")
				return -2;
			else if ((int)body.length() < atoi(_req.getValueMapHeader("Content-Length").c_str())){
				std::cout << "begining" << std::endl;
				int		contentLenght = atoi(_req.getValueMapHeader(std::string("Content-Length")).c_str());
				char	bufferBody[contentLenght];
				nDataLength = recv(getSocket(), bufferBody, contentLenght, 0);
				body.append(bufferBody, nDataLength);
			}
			int lenasf = body.length();
			if ((int)body.length() == atoi(_req.getValueMapHeader("Content-Length").c_str())){
				_req.parsBody(body);
			}
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

void	Client::setResponse(t_server &serverSettings) {
	_res = new Response(_req, serverSettings);
//	if (_res->toFront().first != NULL && std::string(_res->toFront().first).find("text/html") != std::string::npos) {
	std::cout << "\n\nResponse\n";
	write(1, _res->toFront().first, _res->toFront().second);
//	}
}
