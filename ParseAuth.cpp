#include "ParseAuth.hpp"

std::vector<std::string>		parseAuth() {
	std::vector<std::string>	auth;
	std::string					buffer;
	std::ifstream				authFile("./src/auth.conf"); // окрываем файл для чтения
	if (authFile.is_open())
	{
		while (getline(authFile, buffer))
		{
			if (!buffer.empty())
				auth.push_back(buffer);
		}
	}
	authFile.close(); 
	for (std::vector<std::string>::iterator i = auth.begin(); i < auth.end(); i++) {
		std::cout << *i << std::endl;
	}
	return auth;
}