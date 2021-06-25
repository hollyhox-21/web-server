#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>

class Request {
	private:
		// GET, POST, DELETE
		std::string		_method;
		// path to obj
		std::string		_uri;
		// version
		std::string		_version;
		
	public:
		Request(std::string & request);
		~Request();

		std::string		getMethod() const { return _method; }
		std::string		getUri() const { return _uri; }
		std::string		getVersion() const { return _version; }
};

Request::Request(std::string & request) {
	std::string delimiter = " ";

	int i = 0;
	size_t pos = 0;
	std::string token;
	while ((pos = request.find(delimiter)) != std::string::npos) {
		token = request.substr(0, pos);
		request.erase(0, pos + delimiter.length());
		if (i == 0)
			_method = token;
		else if (i == 1)
			_uri = token;
		else if (i == 2)
			_version = token;

		i++;
	}
}

Request::~Request()
{
}


std::ostream &operator<<(std::ostream & out, const Request & request) {
	return out << request.getMethod() << " " << request.getUri() << " " << request.getVersion() << std::endl;
}

#endif