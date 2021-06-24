#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

#include "SocketBase.hpp"

class ServerSocket : public SocketBase {
	private:
		int		_backlog;
	public:
		ServerSocket(int domain, int type, int protocol, int port, u_long interface, int _backlog) throw();
		ServerSocket(ServerSocket const & s);
		~ServerSocket();

		void	connect();
		void	startListening();
		int		accept();
};

#endif