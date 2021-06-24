#ifndef SOCKETBASE_HPP
# define SOCKETBASE_HPP

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include "Exceptions/ServerExceptions.hpp"

class SocketBase {
	private:
		struct sockaddr_in	_address;
		int					_socket;

		void testConnection();
	protected:
		struct sockaddr_in	getAddress();
		int					getSocket();

		void				setSocket(int socket);
	public:
		SocketBase(int domain, int type, int protocol, int port, u_long interface) throw();
		SocketBase(SocketBase const & s);
		virtual ~SocketBase();

		virtual void	connect() = 0;
};

#endif