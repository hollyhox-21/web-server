#ifndef SOCKETBASE_HPP
# define SOCKETBASE_HPP

# include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "../Exceptions/ServerExceptions.hpp"

class SocketBase {
	private:
		struct sockaddr_in	_address;
		int					_socket;

		void testConnection();
	protected:
		struct sockaddr_in	getAddress();

		void				setSocket(int socket);
	public:
		SocketBase() {}
		SocketBase(const char *domain, int port) throw();
		SocketBase(SocketBase const & s);
		virtual ~SocketBase();

		virtual void	connect() = 0;
		int				getSocket();
};

#endif