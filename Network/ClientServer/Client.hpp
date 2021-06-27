#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "IEventHandler.hpp"
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>

# define BUFFER_SIZE 256

class Client {
	private:
		int				_socket;
		bool			_read;
	public:
		Client(int socket);
		~Client();

		std::string	recvMsg();
		void		sendMsg(std::string const & value);
		bool		getStage();
		void		changeStage();

		int			getSocket();
};

#endif