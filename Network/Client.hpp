#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "IEventHandler.hpp"
# include <unistd.h>

class Client {
	private:
		int				_socket;
		IEventHandler	*_eventHandler;
	public:
		Client(int socket, IEventHandler *evntHnd);
		~Client();

		void	sendMsg(std::string const & value);

		int getSocket();
};

#endif