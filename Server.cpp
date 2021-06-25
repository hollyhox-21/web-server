#include "Network/Sockets/ServerSocket.hpp"
#include "Network/Client.hpp"
#include "Network/Request.hpp"
#include "Network/Location.hpp"
#include <vector>


class Server : public IEventHandler {
	private:
		ServerSocket					_serverSocket;
		std::string						_name;
		std::string						_host;
		int								_port;
		std::map<int, std::string>		_erorPages;
		std::vector<Client*>			_clients;
		std::map<std::string, Location>	_locations;

	public:
		Server() :
			_serverSocket(ServerSocket(_host.c_str(), _port)) {	}

		void run () {
			std::cout << "Starting..." << AF_INET << std::endl;
			while (true)
			{
				_clients.push_back(new Client(_serverSocket.accept(), this));
			}
		}

		void connectEvent(Client connection) {
			connection.getSocket();
			std::cout << "Connected" << std::endl;
			char buffer[30000];
			recv(connection.getSocket(), buffer, 30000, 0);
			std::string b(buffer);
			Request r(b);
			std::cout << r << std::endl;
			sendEvent(connection, "message");
		}
		void disconnectEvent(Client connection) {
			connection.getSocket();
			std::cout << "Disconnected" << std::endl;
		}
		void sendEvent(Client connection, std::string value) {
			connection.sendMsg("HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 12\nContent-Type: text/html\nClient: Closed\n\nHello world!");
			close(connection.getSocket());
			std::cout << "Msg: " << value << std::endl;
			disconnectEvent(connection);
		}
		void exceptionEvent(Client connection, std::exception e) {
			connection.getSocket();
			std::cout << "Error: " << e.what() << std::endl;
		}
};