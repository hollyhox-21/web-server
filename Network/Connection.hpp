#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "IEventHandler.hpp"
# include <unistd.h>

class Connection {
	private:
		int				_socket;
		IEventHandler	*_eventHandler;
	public:
		Connection(int socket, IEventHandler *evntHnd);
		~Connection();

		void	sendMsg(std::string const & value);

		int getSocket();
};

#endif