#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include "Client.hpp"
#include "../Sockets/ServerSocket.hpp"
<<<<<<< HEAD
=======
// #include "../Models/Request.hpp"
>>>>>>> origin/pars_config
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
<<<<<<< HEAD
		fd_set							_writeFds;
		int								_fdMax;

	public:
		Server(std::string const & host, int port);
=======

	public:
		Server(): _serverSocket(ServerSocket()) { };
>>>>>>> origin/pars_config
		Server(std::string const & host, int port, std::map<int, std::string> errorPages, std::map<std::string, Location> locations);
		virtual ~Server() { };
		void run ();
<<<<<<< HEAD

		void setErorPages(std::map<int, std::string> errorPages) { _errorPages = errorPages; }
		void setLocations(std::map<std::string, Location> locations) { _locations = locations; }

=======
		void setServerSocket(ServerSocket &);
		void setName(std::string);
		void setHost(std::string);
		void setPort(int);
		void setErrorPages(std::map<int, std::string> &);
		void setClients(std::vector<Client*> &);
		void setLocations(std::map<std::string, Location> &);
>>>>>>> origin/pars_config
		void connectEvent(Client connection);
		void disconnectEvent(Client connection);
		void readEvent(Client connection);
		void sendEvent(Client connection, std::string value);
		void exceptionEvent(Client connection, std::exception e);
};