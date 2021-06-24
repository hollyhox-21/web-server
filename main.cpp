#include "Network/Sockets/ServerSocket.hpp"
#include "Network/Connection.hpp"

class Handler : IEventHandler {
	public:
		Handler() {}
		void connectEvent(Connection connection) {
			connection.getSocket();
			std::cout << "Connected" << std::endl;
			char buffer[30000];
			read(connection.getSocket(), buffer, 30000);
			std::cout << buffer << std::endl;
			sendEvent(connection, "message");
		}
		void disconnectEvent(Connection connection) {
			connection.getSocket();
			std::cout << "Disconnected" << std::endl;
		}
		void sendEvent(Connection connection, std::string value) {
			connection.sendMsg("HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 12\nContent-Type: text/html\nConnection: Closed\n\nHello world!");
			close(connection.getSocket());
			std::cout << "Msg: " << value << std::endl;
			disconnectEvent(connection);
		}
		void exceptionEvent(Connection connection, std::exception e) {
			connection.getSocket();
			std::cout << "Error: " << e.what() << std::endl;
		}
};

int main() {
	std::cout << "Starting..." << AF_INET << std::endl;
	ServerSocket *a = new ServerSocket(AF_INET, SOCK_STREAM, 0, 8000, INADDR_ANY, SOMAXCONN);
	while (true)
	{
		new Connection(a->accept(), (IEventHandler*)new Handler());
	}
	return 0;
}