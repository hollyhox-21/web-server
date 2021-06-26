#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include "Client.hpp"
#include "../Sockets/ServerSocket.hpp"
// #include "../Models/Request.hpp"
#include "../Models/Location.hpp"

class Server : public IEventHandler {
	private:
		ServerSocket					_serverSocket;
		std::string						_name;
		std::string						_host;
		int								_port;
		std::map<int, std::string>		_errorPages;
		std::vector<Client*>			_clients;
		std::map<std::string, Location>	_locations;
		fd_set							_readFds;

	public:
		Server(): _serverSocket(ServerSocket()) { };
		Server(std::string const & host, int port, std::map<int, std::string> errorPages, std::map<std::string, Location> locations);
		virtual ~Server() { };
		void run ();
		void setServerSocket(ServerSocket &);
		void setName(std::string);
		void setHost(std::string);
		void setPort(int);
		void setErrorPages(std::map<int, std::string> &);
		void setClients(std::vector<Client*> &);
		void setLocations(std::map<std::string, Location> &);
		void setRead_fds(int);
		void connectEvent(Client connection);
		void disconnectEvent(Client connection);
		void sendEvent(Client connection, std::string value);
		void exceptionEvent(Client connection, std::exception e);
};