#include "ValidationRequest.hpp"

int		validateRequest(Request & request, std::vector<std::string> & auth) {
	if (request.getValueMapHeader("Content-Length") == "")
		return 411;
	else if (request.getBody().length() != size_t(atoi(request.getValueMapHeader("Content-Length").c_str())))
		return 400;
	else if (request.getMethod() != "GET") {
		for (std::vector<std::string>::iterator i = auth.begin(); i < auth.end(); i++) {
			if (request.getValueMapHeader("Authorization").find(*i) != std::string::npos)
				return 0;
		}
		return 401;
	}
	return 0;
}