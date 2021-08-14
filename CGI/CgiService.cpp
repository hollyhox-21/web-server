# include "CgiService.hpp"

char ** CgiService::getCgiEnv(Request & req, t_server & serverSettings) {
	char **env = new char*[19];
	
	std::string buffer;
	buffer = "SERVER_NAME=";
	buffer += serverSettings.name;
	env[0] = new char[buffer.length() + 1];
	strcpy(env[0], buffer.c_str());
	env[0][buffer.length()] = 0;
	buffer = "SERVER_SOFTWARE=webserv/0.0";
	env[1] = new char[buffer.length() + 1];
	strcpy(env[1], buffer.c_str());
	env[1][buffer.length()] = 0;
	buffer = "GATEWAY_INTERFACE=CGI/1.1";
	env[2] = new char[buffer.length() + 1];
	strcpy(env[2], buffer.c_str());
	env[2][buffer.length()] = 0;
	buffer = "SERVER_PROTOCOL=HTTP/1.1";
	env[3] = new char[buffer.length() + 1];
	strcpy(env[3], buffer.c_str());
	env[3][buffer.length()] = 0;
	buffer = "SERVER_PORT=" + std::to_string(serverSettings.port);
	env[4] = new char[buffer.length() + 1];
	strcpy(env[4], buffer.c_str());
	env[4][buffer.length()] = 0;
	buffer = "REQUEST_METHOD=" + req.getMethod();
	env[5] = new char[buffer.length() + 1];
	strcpy(env[5], buffer.c_str());
	env[5][buffer.length()] = 0;
	buffer = "PATH_INFO=" + req.getUri();
	env[6] = new char[buffer.length() + 1];
	strcpy(env[6], buffer.c_str());
	env[6][buffer.length()] = 0;
	buffer = "PATH_TRANSLATED=";
	buffer += "/home/zera/web-server/CGI/ubuntu_cgi_tester"; //might need some change, using config path/contentLocation
	env[7] = new char[buffer.length() + 1];
	strcpy(env[7], buffer.c_str());
	env[7][buffer.length()] = 0;
	buffer = "SCRIPT_NAME=";
	buffer += "ubuntu_cgi_tester"; //might need some change, using config path/contentLocation
	env[8] = new char[buffer.length() + 1];
	strcpy(env[8], buffer.c_str());
	env[8][buffer.length()] = 0;
	buffer = "QUERY_STRING=1+2";
	env[9] = new char[buffer.length() + 1];
	strcpy(env[9], buffer.c_str());
	env[9][buffer.length()] = 0;
	buffer = "REMOTE_HOST=" + req.getUri();
	env[10] = new char[buffer.length() + 1];
	strcpy(env[10], buffer.c_str());
	env[10][buffer.length()] = 0;
	buffer = "REMOTE_ADDR=";
	env[11] = new char[buffer.length() + 1];
	strcpy(env[11], buffer.c_str());
	env[11][buffer.length()] = 0;
	buffer = "AUTH_TYPE=basic";
	env[12] = new char[buffer.length() + 1];
	strcpy(env[12], buffer.c_str());
	env[12][buffer.length()] = 0;
	buffer = "REMOTE_USER=";
	env[13] = new char[buffer.length() + 1];
	strcpy(env[13], buffer.c_str());
	env[13][buffer.length()] = 0;
	buffer = "REMOTE_IDENT=";
	env[14] = new char[buffer.length() + 1];
	strcpy(env[14], buffer.c_str());
	env[14][buffer.length()] = 0;
	buffer = "CONTENT_TYPE=" + req.getValueMapHeader("Content-Type");
	env[15] = new char[buffer.length() + 1];
	strcpy(env[15], buffer.c_str());
	env[15][buffer.length()] = 0;
	buffer = "CONTENT_LENGTH=" + std::to_string(req.getBody().length());
	env[16] = new char[buffer.length() + 1];
	strcpy(env[16], buffer.c_str());
	env[16][buffer.length()] = 0;
	buffer = "REQUEST_URI=http://" + serverSettings.host + ":" + std::to_string(serverSettings.port) + req.getUri();
	env[17] = new char[buffer.length() + 1];
	strcpy(env[17], buffer.c_str());
	env[17][buffer.length()] = 0;
	buffer = "HTTP_X_SECRET_HEADER_FOR_TEST=1";
	env[18] = new char[buffer.length() + 1];
	strcpy(env[18], buffer.c_str());
	env[18][buffer.length()] = 0;
	env[19] = NULL;
	return env;
}


std::string * CgiService::getCgiResponse(Request & req, t_server & serverSettings) {
	std::cout << "\nStart CGI" << std::endl;
	std::string pathCgi = "/home/zera/web-server/CGI/ubuntu_cgi_tester";
	std::string *bufferStr = new std::string();
	char		buffer[1024] = {0};
	pid_t		pid;
	int			pipeFd[2];
	char * const * nll = NULL;
	char ** env = getCgiEnv(req, serverSettings);
	
	int i = -1;

	while (env[++i] != NULL)
		std::cout << env[i] << std::endl;
	pipe(pipeFd);
	if ((pid = fork()) == 0) {
		dup2(pipeFd[1], 1);
		close(pipeFd[0]);
		exit(execve(pathCgi.c_str(), nll, env));
	}
	else if (pid == -1) {
		std::cout << "Fork broken" << std::endl;
	}
	else {
		close(pipeFd[1]);
		int status;
		// lseek(pipeFd[0], 0, SEEK_SET);
		int ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, 1024);
			ret = read(pipeFd[0], buffer, 1024 - 1);
			*bufferStr += buffer;
			printf("Buffer: %s \n", buffer);
		}
		waitpid(pid, &status, 0);
		std::cout << status;
		close(pipeFd[0]);
	}
	std::cout << *bufferStr;
	if (pid == 0)
		exit(0);
	return bufferStr;
}