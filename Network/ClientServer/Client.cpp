#include "Client.hpp"

Client::Client(int socket) {
	_socket = socket;
	_read = true;
	_state = HEADERS;
}

Client::~Client() {
	close(_socket);
}

void	Client::recvChunked() {
	char			*p;
	std::size_t		contentLength = 0;

	std::cout << "Chunked" << std::endl;
	while (_chunkedBody.find("\r\n") != std::string::npos) {
		std::size_t index = _chunkedBody.find("\r\n") + 2;
		std::string size = _chunkedBody.substr(0, index);
		_chunkedBody.erase(0, index);
		// std::cout << "Size: " << size << std::endl;
		contentLength = strtol(size.c_str(), & p, 16) + 2;
		// std::cout << "Content-Length: " << contentLength << std::endl;
		_body.append(_chunkedBody.substr(0, contentLength));
		// std::cout << "Body: " << _body << std::endl;
		index = _chunkedBody.find("\r\n") + 2;
		_chunkedBody.erase(0, index);
	}
}


Client::STATE	Client::recvHeaders() {
	int			nDataLength;
	std::size_t	konec;

	std::cout << "------HEADERS-------\n";

	nDataLength = recv(getSocket(), _buffer, BUFFER_SIZE, 0);
	if (nDataLength == 0) 
		return (_state = CLOSE);
	else if (nDataLength == -1)
		return (_state = HEADERS);
	_buffer[nDataLength] = 0;
	std::cout << "Сырой буфер\n" << _buffer << "--------------------\n";
	_header.append(_buffer, nDataLength);
	if ((konec = _header.find("\r\n\r\n")) != std::string::npos) {
		std::cout << "Сырые хедеры\n" << _header << "--------------------\n";
		_body = _header.substr(konec + 4);
		_chunkedBody = _body;
		std::cout << _body;
		_req.parsRequest(_header);
		return (_state = BODY);
	}
	return (_state);
}

Client::STATE	Client::recvBody() {
	int nDataLength;

	if (_req.getMethod() != "GET" && _req.getMethod() != "HEAD") {
		std::cout << "------BODY-------\n";
		// if (_req.getValueMapHeader("Content-Length") == "")
		// 	return -1;
		if ((_req.getValueMapHeader("Transfer-Encoding") == "chunked")
										 || (int)_body.length() < atoi(_req.getValueMapHeader("Content-Length").c_str())) {
			nDataLength = recv(getSocket(), _buffer, BUFFER_SIZE, 0);
			if (nDataLength == 0)
				return (_state = END);
			else if (nDataLength == -1)
				return (_state = BODY);
			_buffer[nDataLength] = 0;
			std::cout << "Ya chitau " << nDataLength << "\n";
			std::cout << _buffer;
			std::cout << "--------------------\n";
			if (_req.getValueMapHeader("Transfer-Encoding") == "chunked")
				_chunkedBody.append(_buffer);
			else
				_body.append(_buffer);
			std::cout << "Body: \""<< _chunkedBody << "\"\n";
			if (std::string::npos != _chunkedBody.find("0\r\n\r\n"))
				std::cout << "find\n";
			std::cout << "Len: " << _chunkedBody.length() << "\n-----------------\n";
		} if (_chunkedBody.find("0\r\n\r\n") != std::string::npos) {
			_state = END;
			if (_req.getValueMapHeader("Transfer-Encoding") == "chunked")
				recvChunked();
			// std::cout << "Body: " << _body << std::endl;
			return (_state);
		}
	} else {
		return (_state = END);
	}
	return (_state);
}

Client::STATE		Client::recvMsg() {
	if (_state == HEADERS) {
		if (recvHeaders() == HEADERS)
			return (HEADERS);
	}
	if (_state == BODY) {
		if (recvBody() == BODY)
			return (BODY);
	}
	_req.parsBody(_body);
	std::cout << "konec";
	return END;
}

int		Client::sendMsg() {
	int result = send(_socket, _res->toFront().first, _res->toFront().second, 0);
	_req = Request();
	delete(_res);
	_header.clear();
	_body.clear();
	_chunkedBody.clear();
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
