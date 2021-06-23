#ifndef SOCKETBASE_HPP
# define SOCKETBASE_HPP

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

class SocketBase {
	private:
		struct sockaddr_in	_address;
		int					_socket;
		int					_connection;

		void testConnection();
	public:
		SocketBase(int domain, int type, int protocol, int port, u_long interface);
		SocketBase(SocketBase const & s);
		virtual ~SocketBase();

		virtual int connect(int socket, struct sockaddr_in address) = 0;


};

#endif