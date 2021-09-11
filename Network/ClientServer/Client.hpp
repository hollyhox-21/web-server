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
			END,
			CLOSE
		};

		Client(int socket);
		~Client();

		STATE			recvMsg();
		int 			sendMsg();
		void			changeStage();
		STATE			recvHeaders();
		STATE			recvBody();

		int				getSocket();
		bool			getStage();
		STATE			getState();
		Request&		getRequest() { return _req; }
		void			setResponse(t_server &serverSettings);

	private:
		STATE			_state;
		int				_socket;
		bool			_read;
		char			_buffer[BUFFER_SIZE + 1];
		std::string		_header;
		std::string		_body;
		std::string		_chunkedBody;
		Request			_req;
		Response		*_res;

		void			recvChunked();
};

#endif