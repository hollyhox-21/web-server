#include "ParseFile.hpp"

void *runServer(void *serv) {
	((Server*)serv)->run();
	return NULL;
}

int main(int ac, char **av) {
	std::vector<t_server> structServers;
	if (ac == 2) {
		if (!startParser(av[1], structServers)){
			std::vector<Server> Servers;
			for (int i = 0; i < structServers.size(); ++i) {
				Server obj(structServers[i]);
				Servers.push_back(obj);
			}
			pthread_t s;
			for (unsigned long i = 0; i < Servers.size(); ++i)
			{
				std::cout << i << std::endl;
				Servers[i].ready();
				pthread_create(&s, NULL, &runServer, &Servers[i]);
			}
			while (1)
				;
		}
		else {
			std::cout << "ERROR parse config" << std::endl;
			return (1);
		}
	}
	else {
		std::cout << "wrong arguments" << std::endl;
		return (1);
	}
	return (0);
}