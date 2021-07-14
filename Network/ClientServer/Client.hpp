#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "IEventHandler.hpp"
# include <stdio.h>
# include <cstring>
# include <sys/types.h>
# include <sys/socket.h>
# include "../Models/Request.hpp"
# include "../Models/Response.hpp"

# define BUFFER_SIZE 256

class Client {
	private:
		int				_socket;
		bool			_read;
		std::string		_message;
		Request			_req;
		Response		*_res;

		void			recvChunked();
	public:
		Client(int socket);
		~Client();

		int				recvMsg();
		int 			sendMsg();
		void			changeStage();

		std::string		getMessage();
		int				getSocket();
		bool			getStage();
		Request&		getRequest() { return _req; }
		void			setResponse(std::map<int, std::string> & errPage, std::map<std::string, Location> & locations);
};

#endif