#ifndef IEVENTHANDLER_HPP
# define IEVENTHANDLER_HPP

# include <iostream>
class Client;

class IEventHandler {
	public:
		virtual void connectEvent(Client & connection) = 0;
		virtual void disconnectEvent(Client & connection, int index) = 0;
		virtual void readEvent(Client & connection) = 0;
		virtual void sendEvent(Client & connection) = 0;
		virtual void exceptionEvent(Client & connection, std::exception e) = 0;
};

#endif