#include "Request.hpp"

Request::Request(std::string &buffer, size_t size) {
	std::cout << buffer;
	_parsRequest(buffer, size);
}

Request::~Request() {}

void Request::printRequest() {
	printf("%s\n%s\n%s", _method.c_str(), _uri.c_str(), _proto.c_str());
}

int Request::_recvRequest(int fd) {
    char buffer[BUFSIZ];
    while (1) {
        int n = recv(fd, buffer, BUFSIZ, 0);
//        _parsRequest(buffer, n);
        if (n != BUFSIZ) {
            break;
        }
    }
    return 0;
}

size_t Request::_findNth(const std::string & str , unsigned int N, const std::string & find) {
	if (0 == N) { return std::string::npos; }
	size_t pos, from = 0;
	unsigned i = 0;
	while (i < N) {
		pos = str.find(find, from);
		if (std::string::npos == pos) { break; }
		from = pos + 1;
		++i;
	}
	return pos;
}

void Request::_parsRequest(std::string & buffer, int size) {
    if (size == 0) {
        return;
    }
    
    std::string tmpHeader = buffer.substr(0, buffer.find(CRLF_END));
//    std::cout << tmpHeader;
//	std::string firstStr = buffer.substr(0, buffer.find("\r\n"));
//	_method = firstStr.substr(0, _findNth(firstStr, 1, " "));
//	_uri = firstStr.substr(_findNth(firstStr, 1, " ") + 1, _findNth(firstStr, 2, " ") - _findNth(firstStr, 1, " ") - 1);
//	_proto = firstStr.substr(_findNth(firstStr, 2, " ") + 1);
//	buffer.erase(0, buffer.find("\r\n") + 2);
//	int i = 1;
//	while (_findNth(buffer, i, ":") != std::string::npos) {
//		std::string key = _findeKey();
//		std::string value = _findValue();
//		_headers.insert(std::pair<std::string, std::string>(key, value));
//	}
//	printRequest();
}