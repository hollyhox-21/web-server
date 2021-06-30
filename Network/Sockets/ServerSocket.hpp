#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

#include "SocketBase.hpp"

class ServerSocket : public SocketBase {
	private:
	public:
		ServerSocket() { };
		ServerSocket(const char *domain, int port) throw();
		ServerSocket(ServerSocket const &s);
		virtual ~ServerSocket();

		void	connect();
		void	startListening();
		int		accept();
};

#endif