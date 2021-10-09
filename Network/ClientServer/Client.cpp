#include "Client.hpp"

Client::Client(int socket) {
	_socket = socket;
	_state = END;
	_req = NULL;
	_res = NULL;
}

Client::~Client() {
	if (_req) {
		delete (_req);
	}
	if (_res) {
		delete (_res);
	}
	close(_socket);
}

Client::STATE Client::start() {
	_state = HEADERS;
	_req = new Request();
	return (_state);
}

Client::STATE Client::end() {
	_state = END;
	if (_req) {
		delete (_req);
		_req = NULL;
	}
	if (_res) {
		delete (_res);
		_res = NULL;
	}
	_header.clear();
	_body.clear();
	_chunkedBody.clear();
	return (_state);
}


void	Client::recvChunked() {
	char			*p;
	std::size_t		contentLength = 0;

	std::cout << "Chunked" << std::endl;
	while (_chunkedBody.find("\r\n") != std::string::npos) {
		std::size_t index = _chunkedBody.find("\r\n") + 2;
		std::string size = _chunkedBody.substr(0, index);
		_chunkedBody.erase(0, index);
		contentLength = strtol(size.c_str(), & p, 16) + 2;
		_body.append(_chunkedBody.substr(0, contentLength));
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
		_req->parsRequest(_header);
		return (_state = BODY);
	}
	return (_state);
}

Client::STATE	Client::recvBody() {
	int nDataLength;

	if (_req->getMethod() != "GET" && _req->getMethod() != "HEAD") {
		std::cout << "------BODY-------\n";
		// if (_req->getValueMapHeader("Content-Length") == "")
		// 	return -1;
		if ((_req->getValueMapHeader("Transfer-Encoding") == "chunked")
										 || (int)_body.length() < atoi(_req->getValueMapHeader("Content-Length").c_str())) {
			nDataLength = recv(getSocket(), _buffer, BUFFER_SIZE, 0);
			if (nDataLength == 0)
				return (_state = END);
			else if (nDataLength == -1)
				return (_state = BODY);
			_buffer[nDataLength] = 0;
			std::cout << "Ya chitau " << nDataLength << "\n";
			// std::cout << _buffer;
			// std::cout << "--------------------\n";
			if (_req->getValueMapHeader("Transfer-Encoding") == "chunked")
				_chunkedBody.append(_buffer);
			else
				_body.append(_buffer);
			// std::cout << "Body: \""<< _chunkedBody << "\"\n";
			std::cout << "Body len " << _body.length() << "\n";
			if (std::string::npos != _chunkedBody.find("0\r\n\r\n"))
				std::cout << "find\n";
			std::cout << "Len: " << _chunkedBody.length() << "\n-----------------\n";
		} if (_chunkedBody.find("0\r\n\r\n") != std::string::npos) {
			_state = END;
			if (_req->getValueMapHeader("Transfer-Encoding") == "chunked")
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
	if (_state == END) {
		start();
	}
	if (_state == HEADERS) {
		if (recvHeaders() == HEADERS)
			return (HEADERS);
	}
	if (_state == BODY) {
		if (recvBody() == BODY)
			return (BODY);
	}
	_req->parsBody(_body);
	std::cout << "------konec------\n";
	return (_state = SEND);
}

Client::STATE		Client::sendMsg() {
	char * message = _res->getMessage();
	unsigned long messageLength = _res->getMessageLength();
	int result = 0;
	if (messageLength > 0) {
		if (messageLength > CHUNK_SIZE) {
			result = send(_socket, message, CHUNK_SIZE, 0);
		} else {
			result = send(_socket, message, messageLength, 0);
		}
		if (result == 0) {
			return (CLOSE);
		} if (result < 0) {
			end();
			return (ERROR);
		}
		_res->setMessage(strdup(message + result));
		_res->setMessageLength(messageLength - result);
		std::cout << "res: " << result << std::endl << message << std::endl;

	}
	if (messageLength == 0) {
		// while (true) {
		// 	;
		// }
		return end();
	}
	return _state;
}

int		Client::getSocket() {
	return _socket;
}

Client::STATE		Client::getState() {
	return _state;
}


void	Client::setResponse(t_server &serverSettings) {
	_res = new Response(*_req, serverSettings);
}

void	Client::setState(STATE state) {
	_state = state;
}
