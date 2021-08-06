#include "ParseFile.hpp"

//===============PRINT_RES================
void	printLocations(std::map<std::string, Location> locations) {
	std::cout << "\ncount LOCATION " << locations.size() << std::endl;
	for (auto i = locations.begin(); i != locations.end() ; ++i) {
		std::cout << "\n";
		std::cout << "Location: " << i->first << std::endl;
		std::cout << "ROOT\t" << i->second.getRoot() << std::endl;
		std::cout << "CGI\t\t" << i->second.getPathCGI() << std::endl;
		std::cout << "INDEX\t" << i->second.getIndex() << std::endl;
		std::cout << "AUTOI\t" << i->second.getAutoIndex() << std::endl;
		std::cout << "LIMIT\t" << i->second.getLimit() << std::endl;
		std::map<std::string, bool> tmpMap = i->second.methods;
		for (auto j = tmpMap.begin(); j != tmpMap.end() ; ++j) {
			std::cout << "\t\t" << j->first << " -> " << j->second << std::endl;
		}
	}
}

void	printServ(t_server serv) {
	std::cout << "====SERVER_SETTING====" << std::endl;
	std::cout << serv.name << std::endl;
	std::cout << serv.host << std::endl;
	std::cout << serv.port << std::endl;
	std::map<int, std::string> tmpMap = serv.errorPages;
	for (auto i = tmpMap.begin(); i != tmpMap.end() ; ++i) {
		std::cout << i->first << " -> " << i->second << std::endl;
	}
	printLocations(serv.locations);
}

void	printServers(std::vector<t_server> servers) {
	for (auto i = servers.begin(); i != servers.end() ; ++i) {
		printServ(*i);
	}
}
//============INIT_STRUCTS=======
void initServer(t_server &t) {
	t.name = "";
	t.host = "";
	t.port = 0;
}

void	initLocation(Location &l) {
	l.root = "";
	l.pathCgi = "";
	l.index = "";
	l.autoindex = "off";
	l.limit_client_body_size = 0;
	l.methods["GET"] = true;
	l.methods["POST"] = true;
	l.methods["DELETE"] = true;
}

//============UTILS==============

void	trimSpaces(std::string &value) {
	value.erase(value.begin(),
				std::find_if(value.begin(), value.end(),
							 std::bind1st(std::not_equal_to<char>(), ' ')));
	value.erase(value.begin(),
				std::find_if(value.begin(), value.end(),
							 std::bind1st(std::not_equal_to<char>(), '\t')));
}

size_t	findNth(const std::string & str , unsigned int N, const std::string & find) {
	if (0 == N) { return std::string::npos; }
	size_t pos, from = 0;
	unsigned i = 0;
	while (i < N) {
		pos = str.find(find, from);
		if (std::string::npos == pos) { break; }
		from = pos + 1;
		++i;
	}
	return pos;
}

std::string key(std::string &str, char delim) {
	return str.substr(0, str.find(delim));
}

std::string value(std::string &str, char delim) {
	return str.substr(str.find(delim) + 1);
}

void	mapingErrorPage(std::map<int, std::string> &ep, std::string &str, char delim) {
	ep.insert(std::pair<int, std::string>(atoi(key(str, delim).c_str()), value(str, delim)));
}

void	mapingMethods(std::map<std::string, bool> &methods, std::string &str) {
	if (str.find("GET") == std::string::npos) {
		methods["GET"] = false;
	}
	if (str.find("POST") == std::string::npos) {
		methods["POST"] = false;
	}
	if (str.find("DELETE") == std::string::npos) {
		methods["DELETE"] = false;
	}
}

void	getValueLocation(std::string & str, Location & loc) {
	if (str.find("location:") != std::string::npos) {
		std::string value = str.substr(str.find("location:") + strlen("location:"));
		trimSpaces(value);
		loc.path = value;
	}
	if (str.find("limits_client_body_size:") != std::string::npos) {
		std::string value = str.substr(str.find("limits_client_body_size:") + strlen("limits_client_body_size:"));
		trimSpaces(value);
		loc.limit_client_body_size = atoi(value.c_str());
	}
	if (str.find("autoindex:") != std::string::npos) {
		std::string value = str.substr(str.find("autoindex:") + strlen("autoindex:"));
		trimSpaces(value);
		loc.autoindex = value;
	}
	if (str.find("index:") != std::string::npos) {
		std::string value = str.substr(str.find("index:") + strlen("index:"));
		trimSpaces(value);
		loc.index = value;
	}
	if (str.find("root:") != std::string::npos) {
		std::string value = str.substr(str.find("root:") + strlen("root:"));
		trimSpaces(value);
		loc.root = value;
	}
	if (str.find("path_cgi:") != std::string::npos) {
		std::string value = str.substr(str.find("path_cgi:") + strlen("path_cgi:"));
		trimSpaces(value);
		loc.pathCgi = value;
	}
	if (str.find("allow_methods:") != std::string::npos) {
		str.erase(0, str.find(' ') + 1);
		mapingMethods(loc.methods, str);
	}
}

void	eraseValueForLocation(std::string &settingLoc, std::map<std::string, Location> &locs) {
	Location l;
	initLocation(l);
	
	std::string pathLoc;
	
	settingLoc += '\n';
	while (settingLoc.length() != 0) {
		std::string str = settingLoc.substr(0, settingLoc.find('\n'));
		getValueLocation(str, l);
		settingLoc.erase(0, settingLoc.find('\n') + 1);
	}
	locs.insert(std::pair<std::string, Location>(l.path, l));
}

void eraseValueForServer(std::string &str, t_server & server) {
	if (str.find("host:") != std::string::npos) {
		std::string value = str.substr(str.find("host:") + strlen("host:"));
		trimSpaces(value);
		server.host = value;
	}
	if (str.find("server_name:") != std::string::npos) {
		std::string value = str.substr(str.find("server_name:") + strlen("server_name:"));
		trimSpaces(value);
		server.name = value;
	}
	if (str.find("port:") != std::string::npos) {
		std::string value = str.substr(str.find("port:") + strlen("port:"));
		trimSpaces(value);
		server.port = atoi(value.c_str());
	}
	if (str.find("error_page:") != std::string::npos) {
		str.erase(0, str.find(' ') + 1);
		std::map<int, std::string> errorPage;
		while(str.length() != 0) {
			trimSpaces(str);
			std::string subString = str.substr(0, str.find(' '));
			str.erase(0, str.find(' '));
			mapingErrorPage(errorPage, subString, '=');
		}
		server.errorPages = errorPage;
	}
}

//==========PARSER=================
std::map<std::string, Location>	parsLocation(std::string &blockLoc) {
	std::map<std::string, Location> locs;
	std::string settingLocs = blockLoc.substr(0, blockLoc.find("server:"));
	blockLoc.erase(0, settingLocs.length());
	while (settingLocs.length() != 0) {
		std::string str = settingLocs.substr(0, settingLocs.find("\n\n"));
		settingLocs.erase(0, str.length() + 1);
		eraseValueForLocation(str, locs);
		if (settingLocs == "\n")
			break;
	}
	return locs;
}

void	parsServer(t_server & server, std::string &blockBuf) {
	std::string strServ = "server:\n";
	std::string settingServ;
	
	blockBuf.erase(0, strServ.length());
	settingServ = blockBuf.substr(0, blockBuf.find("\n\n"));
	settingServ	+= "\n";
	blockBuf.erase(0, settingServ.length() + 1);
	while (settingServ.length() != 0) {
		std::string str = settingServ.substr(0, settingServ.find('\n'));
		eraseValueForServer(str, server);
		settingServ.erase(0, settingServ.find('\n') + 1);
	}
	server.locations = parsLocation(blockBuf);
}

t_server parsBuf(std::string &buf) {
	t_server server;
	initServer(server);
	
	std::string	blockServer;
	size_t posSecondServ;
	if ((posSecondServ = findNth(buf, 2, "server:"))) {
		blockServer = buf.substr(0, posSecondServ);
		parsServer(server, blockServer);
		buf.erase(0, posSecondServ);
	}
	else {
		blockServer = buf.substr(0, buf.length());
		buf.erase(0, buf.length());
	}
	return (server);
}
int	startParser(char *fileName, std::vector<t_server> &servers) {
	std::ifstream	parsFile(fileName);
	std::string		buf;
	std::string		bufFile;

	if (parsFile.is_open()) {
		std::cout << "START\tpars" << std::endl;
		while (std::getline(parsFile, buf)) {
			bufFile += buf + "\n";
		}
		while (bufFile.find("server:") != std::string::npos) {
			t_server serv = parsBuf(bufFile);
			servers.push_back(serv);
		}
	}
	else if (!parsFile.is_open()) {
		return (1);
	}
	parsFile.close();
	return (0);
}

//int main(int ac, char **av) {
//	std::vector<t_server> structServers;
//	if (ac == 2) {
//		if (!startParser(av[1], structServers)){
//			std::vector<Server> Servers;
//			for (int i = 0; i < structServers.size(); ++i) {
//				Server obj(structServers[i]);
//				Servers.push_back(obj);
//			}
//		pthread_t s;
//		for (unsigned long i = 0; i < Servers.size(); ++i)
//		{
//			std::cout << i << std::endl;
//			Servers[i].ready();
//			pthread_create(&s, NULL, &runServer, &Servers[i]);
//		}
//		while (1)
//			;
//		}
//		else {
//			std::cout << "ERROR parse config" << std::endl;
//			return (1);
//		}
//	}
//	else {
//		std::cout << "wrong arguments" << std::endl;
//		return (1);
//	}
//	return (0);
//}

//std::string pars_one_arg(std::string &line)
//{
//	line.erase(0, line.find(":") + 1);
//	line.erase(0, line.find_first_not_of(" \t"));
//	int i;
//	if ((i = line.find(" ")) != -1)
//	{
//		std::string between = line;
//		between.erase(0, i);
//		between.erase(0, between.find_first_not_of(" "));
//		if (!between.empty())
//		{
//			std::cout << "Error: wrong argument in config" << std::endl;
//			return "";
//		}
//		else
//			line.erase(i, line.length());
//	}
//	return line;
//}
//
//std::vector<Server*> parser(std::string path)
//{
//	std::vector<std::vector<std::string> > servs;
//	std::vector<Server *> serv = std::vector<Server *>();
//	std::string line;
//	std::ifstream file(path);
//	if (file.fail())
//	{
//		std::cout << "Error: file is wrong" << std::endl;
//	}
//	else
//	{
//		while (1)
//		{
//			std::vector<std::string> ser;
//			while (getline(file, line) &&
//				   line.find("server:") == std::string::npos)
//			{
//				ser.push_back(line);
//			}
//			if (file.fail())
//			{
//				servs.push_back(ser);
//				break;
//			}
//			if (line.find("server:") != std::string::npos)
//			{
//				if (ser.size())
//					servs.push_back(ser);
//				continue;
//			}
//		}
//		for (unsigned long i = 0; i < servs.size(); ++i)
//		{
//			if (i == 3)
//				NULL;
//			int lflg = 0;
//			std::string locPath;
//			std::map<std::string, Location> location;
//			std::map<int, std::string> erPage;
//			Location loc;
//			Server *s = new Server();
//			for (unsigned long j = 0; j < servs[i].size(); ++j)
//			{
//				line = servs[i][j];
//				if (line[0] == ' ')
//				{
//					std::cout << "Space in config in line: " << i * j
//							  << std::endl;
//				}
//				if (line[0] == '\t' && line[1] != '\t')
//				{
//					line.erase(0, 1);
//					if (line.find("server_name:") != std::string::npos)
//					{
//						line = pars_one_arg(line);
//						s->setName(line);
//					} else if (line.find("host:") != std::string::npos)
//					{
//						line = pars_one_arg(line);
//						s->setHost(line);
//					} else if (line.find("port:") != std::string::npos)
//					{
//						line = pars_one_arg(line);
//						s->setPort(atoi(line.c_str()));
//					} else if (line.find("error_page:") != std::string::npos)
//					{
//						std::string first, second;
//						line.erase(0, line.find(":") + 1);
//						line.erase(0, line.find_first_not_of(" \t"));
//						int l;
//						if ((l = line.find(" ")) != -1)
//						{
//							first = std::string(line);
//							second = std::string(line);
//							first.erase(l, line.length());
//							second.erase(0, l + 1);
//							second.erase(0, second.find_first_not_of(" \t"));
//							if ((l = second.find_first_of(" \t")) != -1)
//								second.erase(l, second.length());
//						}
//						erPage.insert(
//								std::pair<int, std::string>(atoi(first.c_str()),
//															second));
//						s->setErrorPages(erPage);
//					} else if (line.find("location:") != std::string::npos)
//					{
//						lflg = 1;
//						line = pars_one_arg(line);
//						locPath = line;
//					}
//				} else if (lflg == 1 && line[0] == '\t' && line[1] == '\t')
//				{
//					if (i == 3)
//						NULL;
//					if (line.find("root:") != std::string::npos)
//					{
//						line = pars_one_arg(line);
//						loc.root = std::string(line);
//					} else if (line.find("path_cgi:") != std::string::npos)
//					{
//						line = pars_one_arg(line);
//						loc.pathCgi = std::string(line);
//					}
//					else if (line.find("autoindex:", 2, 10) != std::string::npos)
//					{
//						line = pars_one_arg(line);
//						loc.autoindex = std::string(line);
//					}
//					else if (line.find("index:", 2, 6) != std::string::npos)
//					{
//						line = pars_one_arg(line);
//						loc.index = std::string(line);
//					}
//
//					else if (line.find("allow_methods:") != std::string::npos)
//					{
//						std::map<std::string, bool> methods = std::map<std::string, bool>();
//						if (line.find("GET") != std::string::npos &&
//							loc.index != "")
//							methods.insert(
//									std::make_pair<std::string, bool>("GET",
//																	  1));
//						else if (line.find("GET") != std::string::npos &&
//								 loc.index == "")
//							std::cout << "No indes for GET" << std::endl;
//						if (line.find("POST") != std::string::npos)
//							methods.insert(
//									std::make_pair<std::string, bool>("POST",
//																	  1));
//						if (line.find("DELETE") != std::string::npos)
//							methods.insert(
//									std::make_pair<std::string, bool>("DELETE",
//																	  1));
//						loc.methods = methods;
//					}
//				}
//				else if ((lflg == 1 && line.empty()) ||
//						 (lflg == 1 && j == servs[i].size() - 1))
//				{
//					lflg = 0;
//					location.insert(std::make_pair(locPath, loc));
//					loc.reset();
//					s->setLocations(location);
//				}
//			}
//			serv.push_back(s);
//		}
//	}
//	file.close();
//	return serv;
//}

//int main(int ac, char **av)
//{
//	(void)ac;
//	std::vector<Server*> serv;
//	serv = parser(av[1]);
//	pthread_t s;
//	for (unsigned long i = 0; i < serv.size(); ++i)
//	{
//		std::cout << i << std::endl;
//		serv[i]->ready();
//		pthread_create(&s, NULL, &runServer, serv[i]);
//	}
//	while (1)
//		;
//}

//int main(int ac, char **av)
//{
//	std::vector<std::vector<std::string> > servs;
//	if (ac != 2)
//		return 0;
//	std::ifstream file(av[1]);
//	if (file.fail())
//	{
//		std::cout << "Error: file is wrong" << std::endl;
//		return -1;
//	}
//	std::string line;
//	while (1)
//	{
//		std::vector<std::string> ser;
//		while (getline(file, line) && line.find("server:") == std::string::npos)
//		{
//			ser.push_back(line);
//		}
//		if (file.fail())
//		{
//			servs.push_back(ser);
//			break;
//		}
//		if (line.find("server:") != std::string::npos)
//		{
//			if (ser.size())
//				servs.push_back(ser);
//			continue;
//		}
//	}
//	std::vector<Server*> serv = std::vector<Server*>();
//	for (unsigned long i = 0; i < servs.size(); ++i)
//	{
//		if (i == 3)
//			NULL;
//		int lflg = 0;
//		std::string locPath;
//		std::map<std::string, Location> location;
//		std::map<int, std::string> erPage;
//		Location loc;
//		Server *s = new Server();
//		for (unsigned long j = 0; j < servs[i].size(); ++j)
//		{
//			line = servs[i][j];
//			if (line[0] == ' '){
//				std::cout << "Space in config in line: " << i * j << std::endl;
//			}
//			if (line[0] == '\t' && line[1] != '\t')
//			{
//				line.erase(0, 1);
//				if (line.find("server_name:") != std::string::npos)
//				{
//					line = pars_one_arg(line);
//					s->setName(line);
//				}
//				else if (line.find("host:") != std::string::npos)
//				{
//					line = pars_one_arg(line);
//					s->setHost(line);
//				}
//				else if (line.find("port:") != std::string::npos)
//				{
//					line = pars_one_arg(line);
//					s->setPort(atoi(line.c_str()));
//				}
//				else if (line.find("error_page:") != std::string::npos)
//				{
//					std::string first, second;
//					line.erase(0, line.find(":") + 1);
//					line.erase(0, line.find_first_not_of(" \t"));
//					int l;
//					if ((l = line.find(" ")) != -1)
//					{
//						first = std::string(line);
//						second = std::string(line);
//						first.erase(l, line.length());
//						second.erase(0, l + 1);
//						second.erase(0, second.find_first_not_of(" \t"));
//						if ((l = second.find_first_of(" \t")) != -1)
//							second.erase(l, second.length());
//					}
//					erPage.insert(std::pair<int, std::string>(atoi(first.c_str()), second));
//					s->setErrorPages(erPage);
//				}
//				else if (line.find("location:") != std::string::npos)
//				{
//					lflg = 1;
//					line = pars_one_arg(line);
//					locPath = line;
//				}
//			}
//			else if (lflg == 1 && line[0] == '\t' && line[1] == '\t')
//			{
//				if (i == 3)
//					NULL;
//				if (line.find("root:") != std::string::npos)
//				{
//					line = pars_one_arg(line);
//					loc.root = std::string(line);
//				}
//				else if (line.find("path_cgi:") != std::string::npos)
//				{
//					line = pars_one_arg(line);
//					loc.pathCgi = std::string(line);
//				}
//				else if (line.find("index:") != std::string::npos)
//				{
//					line = pars_one_arg(line);
//					loc.index = std::string(line);
//				}
//				else if (line.find("allow_methods:") != std::string::npos)
//				{
//					std::map<std::string, bool> methods = std::map<std::string, bool>();
//					if (line.find("GET") != std::string::npos && loc.index != "")
//						methods.insert(std::make_pair<std::string, bool>("GET", 1));
//					else if (line.find("GET") != std::string::npos && loc.index == "")
//						return -1;
//					if (line.find("POST") != std::string::npos)
//						methods.insert(std::make_pair<std::string, bool>("POST", 1));
//					if (line.find("DELETE") != std::string::npos)
//						methods.insert(std::make_pair<std::string, bool>("DELETE", 1));
//					loc.methods = methods;
//				}
//			}
//			if (i == 3 && j == 23)
//				NULL;
//			else if ((lflg == 1 && line.empty()) || (lflg == 1 && j == servs[i].size() - 1))
//			{
//				lflg = 0;
//				location.insert(std::make_pair(locPath, loc));
//				loc.reset();
//				s->setLocations(location);
//			}
//		}
//		serv.push_back(s);
//	}
//	pthread_t s;
//	for (unsigned long i = 0; i < serv.size(); ++i)
//	{
//		std::cout << i << std::endl;
//		serv[i]->ready();
//		pthread_create(&s, NULL, &runServer, serv[i]);
//	}
//	file.close();
//	while (1)
//	 ;
//	return 0;
//}
