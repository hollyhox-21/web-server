#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "IEventHandler.hpp"
# include <stdio.h>
# include <cstring>
# include <sys/types.h>
# include <sys/socket.h>
# include "../Network.hpp"
# include "../Models/Response.hpp"

# define BUFFER_SIZE 256

class Client {

	public:

		enum STATE {
			HEADERS,
			BODY,
			SEND,
			END,
			CLOSE
		};

		Client(int socket);
		~Client();

		STATE			start();
		STATE			end();
		STATE			recvMsg();
		STATE 			sendMsg();
		STATE			recvHeaders();
		STATE			recvBody();

		int				getSocket();
		STATE			getState();
		Request*		getRequest() { return _req; }
		void			setResponse(t_server &serverSettings);

	private:
		STATE			_state;
		int				_socket;
		char			_buffer[BUFFER_SIZE + 1];
		std::string		_header;
		std::string		_body;
		std::string		_chunkedBody;
		Request			*_req;
		Response		*_res;

		void			recvChunked();
};

#endif