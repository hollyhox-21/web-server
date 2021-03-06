#ifndef SERVER_HPP
# define SERVER_HPP

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include "../Network.hpp"

class Server : public IEventHandler{
	private:
		ServerSocket					*_serverSocket;
		std::string						_name;
		std::string						_host;
		int								_port;
		std::map<int, std::string>		_errorPages;
		std::vector<Client*>			_clients;
		std::map<std::string, Location>	_locations;
		fd_set							_readFds;
		fd_set							_writeFds;
		int								_fdMax;
		t_server						_serverSettings;

	public:
		Server(t_server & server) {
			_serverSettings = server;
			_name = server.name;
			_host = server.host;
			_port = server.port;
			_errorPages = server.errorPages;
			_locations = server.locations;
		}
		
		~Server() { delete( _serverSocket); }
		void ready();

	const std::map<int, std::string> &getErrorPages() const;

	void run ();

		void setName(std::string const & name) { _name = name; }
		void setHost(std::string const & host) { _host = host; }
		void setPort(int port) { _port = port; }
		void setErrorPages(std::map<int, std::string> errorPages) { _errorPages = errorPages; }
		void setLocations(std::map<std::string, Location> locations) { _locations = locations; }

		t_server&	getSettings() { return _serverSettings; }

		void connectEvent(Client & connection);
		void disconnectEvent(Client & connection, int index);
		void readEvent(Client & connection);
		void sendEvent(Client & connection);
		void exceptionEvent(Client & connection, std::exception e);

	const std::map<std::string, Location> &getLocations() const;
};

#endif