#include "Request.hpp"

Request::Request(int fd) {
    _printRequest(fd);
}

Request::~Request() {}

void Request::_printRequest(int fd) {
    char buffer[BUFSIZ];
    while (1) {
        int n = recv(fd, buffer, BUFSIZ, 0);
        write(1, buffer, n);
        if (n != BUFSIZ) {
            break;
        }
    }
}

int Request::_recvRequest(int fd) {
    char buffer[BUFSIZ];
    while (1) {
        int n = recv(fd, buffer, BUFSIZ, 0);
        _parsRequest(buffer, n);
        if (n != BUFSIZ) {
            break;
        }
    }
    return 0;
}

void Request::_parsRequest(char *buffer, int size) {
    static int state = 0;
    if (size == 0) {
        return;
    }

    switch (state) {
        case 0:

            break;
        case 1:

            break;
        case 2:

            break;
    }
}