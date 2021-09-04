#include "Client.hpp"
#define BUF_SIZE 100000

Client::Client(int socket) {
	_socket = socket;
	_read = true;
	_state = HEADERS;
}

Client::~Client() {
	close(_socket);
}

void	Client::recvChunked() {
	int				nDataLength;
	long			contentLenght = 0;
	char			*p;
	char			buffer[BUFFER_SIZE + 1];
	std::string		bufferLenght;
	std::string		body;
	std::string     bufferIzlishek;
	char		    *konec;

	do {
		do {
			nDataLength = recv(getSocket(), buffer, BUFFER_SIZE, 0);
			if (nDataLength > 0) {
			    buffer[nDataLength] = 0;
			    bufferLenght.append(buffer, nDataLength);
			}
		} while ((konec = (char*)strstr(bufferLenght.c_str(), (const char*)"\r\n")) == NULL);
		std::cout << "Posle pervogo while " << bufferLenght << std::endl;
		konec += 2;
		bufferIzlishek = std::string(konec);
//		bufferIzlishek += strdup(konec);
		bufferLenght = bufferLenght.substr(0, konec - bufferLenght.c_str());
		contentLenght = strtol(bufferLenght.c_str(), & p, 16) + 2;
		int skolkoschitat = contentLenght - bufferIzlishek.length();
		std::cout << "Content length " << bufferLenght << " | " << contentLenght << std::endl;
		std::cout << "Skolko schitat " << skolkoschitat << std::endl;
		char	bufferBody[skolkoschitat];
		if (bufferIzlishek.length() > 2)
            body.append(bufferIzlishek);
		if (skolkoschitat > 0) {
		    while (skolkoschitat > 0 && nDataLength > 0) {
				nDataLength = read(getSocket(), &bufferBody, skolkoschitat);
				body.append(bufferBody);
				std::cout << "Skolko schital " << nDataLength << std::endl;
				skolkoschitat -= nDataLength;
			} 
			if (nDataLength == -1)
				strerror(errno);
		}
		bufferLenght = "";
	} while (contentLenght > 2);
	std::cout << "--------------------\nBody after chunk:\n" << body << "--------------------" << std::endl;
	_req.parsBody(body);
}


Client::STATE	Client::recvHeaders() {
	char buffer[BUFFER_SIZE];
	int nDataLength;
	const char	*konec;

	nDataLength = recv(getSocket(), buffer, BUFFER_SIZE, 0);
	// if (nDataLength == -1) {
	// 	return (-1);
	// }
	_header.append(buffer, nDataLength);
	if ((konec = strstr(_header.c_str(), (const char*)"\r\n\r\n")) != NULL) {
		std::cout << "--------------------\nСырые хедеры\n" << _header << "--------------------\n";
		_body = _header.substr(0, konec + 4 - _header.c_str());
		_req.parsRequest(_header);
		return (_state = HEADERS);
	}
	return (_state);
}

Client::STATE	Client::recvBody() {
	int nDataLength;

	if (_req.getMethod() != "GET" && _req.getMethod() != "HEAD") {
		// if (_req.getValueMapHeader("Content-Length") == "")
		// 	return -1;
		if ((_req.getValueMapHeader("Transfer-Encoding") == "chunked" && _req.getBody().find("0\r\n\r\n") == std::string::npos) || 
					(int)_body.length() < atoi(_req.getValueMapHeader("Content-Length").c_str())){
			char	bufferBody[BUF_SIZE];
			nDataLength = recv(getSocket(), bufferBody, BUF_SIZE, 0);
			_body.append(bufferBody, nDataLength);
		} else {
			_state = END;
			if (_req.getValueMapHeader("Transfer-Encoding") == "chunked")
				std::cout << ""; // parsing чанк
		}
	}
	return (_state);
}

Client::STATE		Client::recvMsg() {

	switch (_state)
	{
		case HEADERS:
			return recvHeaders();
		case BODY:
			return recvBody();
		default:
			return END;
	}
}

int		Client::sendMsg() {
	int result = send(_socket, _res->toFront().first, _res->toFront().second, 0);
	_req = Request();
	delete(_res);
	_header.clear();
	_body.clear();
	_state = HEADERS;
	return result;
}

void	Client::changeStage() {
	_read = !_read;
}

int		Client::getSocket() {
	return _socket;
}

bool	Client::getStage() {
	return _read;
}

Client::STATE		Client::getState() {
	return _state;
}


void	Client::setResponse(t_server &serverSettings) {
	_res = new Response(_req, serverSettings);
//	if (_res->toFront().first != NULL && std::string(_res->toFront().first).find("text/html") != std::string::npos) {
	// std::cout << "\n\nResponse\n";
	// write(1, _res->toFront().first, _res->toFront().second);
//	}
}
