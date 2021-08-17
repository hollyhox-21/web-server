# include "CgiService.hpp"

char * setString(std::string & buffer) {
	char *str = new char[buffer.length() + 1];
	strcpy(str, buffer.c_str());
	str[buffer.length()] = 0;
	return str;
}

char ** CgiService::getCgiEnv(Request & req, t_server & serverSettings) {
	char **env = new char*[19];
	
	std::string buffer;
	buffer = "AUTH_TYPE=basic";
	env[0] = strdup(buffer.c_str());
	buffer = "CONTENT_LENGTH=" + std::to_string((int)req.getBody().length());
	env[1] = strdup(buffer.c_str());
	buffer = "CONTENT_TYPE=text/html";// + req.getValueMapHeader("Content-Type");
	env[2] = strdup(buffer.c_str());
	buffer = "GATEWAY_INTERFACE=CGI/1.1";
	env[3] = strdup(buffer.c_str());
	buffer = "PATH_INFO=" + req.getUri() + "/";
	env[5] = strdup(buffer.c_str());
	buffer = "PATH_TRANSLATED=" + req.getUri() + "/";
	env[6] = strdup(buffer.c_str());
	buffer = "QUERY_STRING=" + req.getUri() + "/?";
	env[7] = strdup(buffer.c_str());
	buffer = "REMOTE_ADDR=" + serverSettings.host + "";
	env[8] = strdup(buffer.c_str());
	buffer = "REMOTE_IDENT=basic";
	env[9] = strdup(buffer.c_str());
	buffer = "REMOTE_USER=basic";
	env[10] = strdup(buffer.c_str());
	buffer = "REQUEST_METHOD=" + req.getMethod() + "";
	env[11] = strdup(buffer.c_str());
	buffer = "REQUEST_URI=" + req.getUri() + "/";
	env[12] = strdup(buffer.c_str());
	buffer = "SCRIPT_NAME=" + req.getUri() + "/";
	env[13] = strdup(buffer.c_str());
	buffer = "SERVER_NAME=" + serverSettings.name + "";
	env[14] = strdup(buffer.c_str());
	buffer = "SERVER_PORT=" + std::to_string(serverSettings.port) + "";
	env[15] = strdup(buffer.c_str());
	buffer = "SERVER_PROTOCOL=HTTP/1.1";
	env[16] = strdup(buffer.c_str());
	buffer = "SERVER_SOFTWARE=SVJ/228";
	env[17] = strdup(buffer.c_str());
	buffer = "HTTP_X_SECRET_HEADER_FOR_TEST=1";
	env[4] = strdup(buffer.c_str());
	env[18] = NULL;
	return env;
}


std::string * CgiService::getCgiResponse(Request & req, t_server & serverSettings) {
	std::cout << "\nStart CGI" << std::endl;
	std::string pathCgi = "/Users/hapryl/Desktop/Projects/web-server/CGI/cgi_tester";

	char ** env = getCgiEnv(req, serverSettings);
	int i = -1;
	while (env[++i] != NULL)
		std::cout << env[i] << std::endl;
	//------------------------


	int savedFd[2];
	FILE *file[2];
	int fd[2];
	pid_t pid;
	std::string *newBody = new std::string();


	if ((savedFd[IN] = dup(STDIN_FILENO)) == -1 || (savedFd[OUT] = dup(STDOUT_FILENO)) == -1)
		throw std::runtime_error(std::string("Can't create file descriptor"));;
	if (!(file[IN] = tmpfile()) || !(file[OUT] = tmpfile()))
		throw std::runtime_error(std::string("Can't create temporary file"));
	if ((fd[IN] = fileno(file[IN])) == -1 || (fd[OUT] = fileno(file[OUT])) == -1)
		throw std::runtime_error(std::string("Can't create file descriptor"));

	write(fd[IN], req.getBody().c_str(), req.getBody().size());
	lseek(fd[IN], SEEK_SET, SEEK_SET);

	pid = fork();
	if (pid == -1) {
		throw std::runtime_error(std::string("Can't fork process"));
	}
	else if (pid == 0) {
		dup2(fd[IN], STDIN_FILENO);
		dup2(fd[OUT], STDOUT_FILENO);
		char * const * nlPointer = NULL;
		if (execve(pathCgi.c_str(), nlPointer, env) == -1)
			throw std::runtime_error(std::string("Execve crashed!"));
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else
	{
		char buffer[1024];
		waitpid(-1, NULL, 0);
		lseek(fd[OUT], SEEK_SET, SEEK_SET);
		ssize_t bytes = 1;
		while (bytes > 0) {
			bzero(buffer, 1024);
			bytes = read(fd[OUT], buffer, 1024);
			*newBody += buffer;
		}
	}
	dup2(savedFd[IN], STDIN_FILENO);
	dup2(savedFd[OUT], STDOUT_FILENO);
	fclose(file[IN]);
	fclose(file[OUT]);
	close(fd[IN]);
	close(fd[OUT]);
	close(savedFd[IN]);
	close(savedFd[OUT]);
	if (pid == 0)
		exit(0);
	size_t pos;
	std::string cgiHeader;
	// if ((pos = newBody.find("\r\n\r\n", 0)) != std::string::npos) {
	// 	cgiHeader = std::string(newBody, 0, pos + 4);
	// 	newBody = std::string(newBody, pos + 4);
	// 	if (cgiHeader.find("Status: ", 0) != std::string::npos)
	// 		std::cout << "status code " << (std::atoi(cgiHeader.substr(8, 3).c_str()));
	// 	if ((pos = cgiHeader.find("Content-Type: ", 0)) != std::string::npos)
	// 		std::cout << "Content-Type " << (cgiHeader.substr(pos + 14, 24));
	// 	std::cout << "Header " << (cgiHeader);
	// 	std::cout << "Body " << newBody;
	// 	//std::cout << "" << ((size_t)_bodySize - cgiHeader.size());
	// }
	while (env[++i] != NULL)
		delete[] env[i];
	delete[] env;
	return newBody;
}