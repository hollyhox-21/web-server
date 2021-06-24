#ifndef IEVENTHANDLER_HPP
# define IEVENTHANDLER_HPP

# include <iostream>
class Connection;

class IEventHandler {
	public:
		virtual void connectEvent(Connection connection) = 0;
		virtual void disconnectEvent(Connection connection) = 0;
		virtual void sendEvent(Connection connection, std::string value) = 0;
		virtual void exceptionEvent(Connection connection, std::exception e) = 0;
};

#endif