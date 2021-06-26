#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include "Client.hpp"
#include "../Sockets/ServerSocket.hpp"
#include "../Models/Location.hpp"

class Server : public IEventHandler {
	private:
		ServerSocket					_serverSocket;
		std::string						_name;
		std::string						_host;
		int								_port;
		std::map<int, std::string>		_erorPages;
		std::vector<Client*>			_clients;
		std::map<std::string, Location>	_locations;
		fd_set							_readFds;
		fd_set							_writeFds;
		int								_fdMax;

	public:
		Server(std::string const & host, int port, std::map<int, std::string> errorPages, std::map<std::string, Location> locations);

		void run ();

		void connectEvent(Client connection);
		void disconnectEvent(Client connection);
		void readEvent(Client connection);
		void sendEvent(Client connection, std::string value);
		void exceptionEvent(Client connection, std::exception e);
};