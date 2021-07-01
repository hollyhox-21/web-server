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
		std::string		_message;
	public:
		Client(int socket);
		~Client();

		int				recvMsg();
		int 			sendMsg(std::string const & value);
		void			changeStage();

		std::string		getMessage();
		int				getSocket();
		bool			getStage();
};

#endif