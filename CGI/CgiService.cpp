# include "CgiService.hpp"

char ** CgiService::getCgiEnv(Request & req) {
	char **env = new char*[19];
	
	std::string buffer;
	buffer = "AUTH_TYPE=basic";
	env[0] = new char[buffer.length() + 1];
	strcpy(env[0], buffer.c_str()); //Security needed to execute php-cgi
	env[0][buffer.length()] = 0;
	buffer = "GATEWAY_INTERFACE=CGI/1.1";
	env[1] = new char[buffer.length() + 1];
	strcpy(env[1], buffer.c_str());
	env[1][buffer.length()] = 0;
	buffer = "SCRIPT_NAME=";
	buffer += "/home/zera/web-server/CGI/ubuntu_cgi_tester";
	env[2] = new char[buffer.length() + 1];
	strcpy(env[2], buffer.c_str());
	env[2][buffer.length()] = 0;
	buffer = "SCRIPT_NAME=";
	buffer += "/home/zera/web-server/CGI/ubuntu_cgi_tester";
	env[3] = new char[buffer.length() + 1];
	strcpy(env[3], buffer.c_str());
	env[3][buffer.length()] = 0;
	buffer = "REQUEST_METHOD=" + req.getMethod();
	env[4] = new char[buffer.length() + 1];
	strcpy(env[4], buffer.c_str());
	env[4][buffer.length()] = 0;
	buffer = "CONTENT_LENGTH=" + std::to_string(req.getBody().length());
	env[5] = new char[buffer.length() + 1];
	strcpy(env[5], buffer.c_str());
	env[5][buffer.length()] = 0;
	buffer = "CONTENT_TYPE=" + req.getValueMapHeader("Content-Type");
	env[6] = new char[buffer.length() + 1];
	strcpy(env[6], buffer.c_str());
	env[6][buffer.length()] = 0;
	buffer = "PATH_INFO=";
	buffer += "/home/zera/web-server/CGI/ubuntu_cgi_tester"; //might need some change, using config path/contentLocation
	env[7] = new char[buffer.length() + 1];
	strcpy(env[7], buffer.c_str());
	env[7][buffer.length()] = 0;
	buffer = "PATH_TRANSLATED=";
	buffer += "/home/zera/web-server/CGI/ubuntu_cgi_tester"; //might need some change, using config path/contentLocation
	env[8] = new char[buffer.length() + 1];
	strcpy(env[8], buffer.c_str());
	env[8][buffer.length()] = 0;
	buffer = "QUERY_STRING=/home/zera/web-server/CGI/ubuntu_cgi_tester?1+2";
	env[9] = new char[buffer.length() + 1];
	strcpy(env[9], buffer.c_str());
	env[9][buffer.length()] = 0;
	buffer = "REMOTE_ADDR=";
	buffer += "127.0.0.1";
	env[10] = new char[buffer.length() + 1];
	strcpy(env[10], buffer.c_str());
	env[10][buffer.length()] = 0;
	buffer = "REMOTE_IDENT=basic" + req.getValueMapHeader("Authorization");
	env[11] = new char[buffer.length() + 1];
	strcpy(env[11], buffer.c_str());
	env[11][buffer.length()] = 0;
	buffer = "REMOTE_USER=basic" + req.getValueMapHeader("Authorization");
	env[12] = new char[buffer.length() + 1];
	strcpy(env[12], buffer.c_str());
	env[12][buffer.length()] = 0;
	buffer = "REQUEST_URI=";
	buffer += req.getMethod() + " " + req.getUri() + " " + req.getProto();
	env[13] = new char[buffer.length() + 1];
	strcpy(env[13], buffer.c_str());
	env[13][buffer.length()] = 0;
	// if (headers.find("Hostname") != headers.end())
	// 	this->_env["SERVER_NAME"] = req.getValueMapHeader("Hostname");
	// else
	// 	this->_env["SERVER_NAME"] = this->_env["REMOTEaddr"];
	buffer = "SERVER_PORT=";
	buffer += std::to_string(8000);
	env[14] = new char[buffer.length() + 1];
	strcpy(env[14], buffer.c_str());
	env[14][buffer.length()] = 0;
	buffer = "SERVER_PROTOCOL=";
	buffer += "HTTP/1.1";
	env[15] = new char[buffer.length() + 1];
	strcpy(env[15], buffer.c_str());
	env[15][buffer.length()] = 0;
	buffer = "SERVER_SOFTWARE=";
	buffer += "Weebserv/1.0";
	env[16] = new char[buffer.length() + 1];
	strcpy(env[16], buffer.c_str());
	env[16][buffer.length()] = 0;
	buffer = "HTTP_X_SECRET_HEADER_FOR_TEST=1";
	env[17] = new char[buffer.length() + 1];
	strcpy(env[17], buffer.c_str());
	env[17][buffer.length()] = 0;
	env[18] = NULL;
	return env;
}


std::string * CgiService::getCgiResponse(Request & req) {
	std::cout << "Start CGI" << std::endl;
	std::string pathCgi = "/home/zera/web-server/CGI/ubuntu_cgi_tester";
	std::string *bufferStr = new std::string();
	char		buffer[1024] = {0};
	pid_t		pid;
	int			pipeFd[2];
	char * const * nll = NULL;
	char ** env = getCgiEnv(req);
	
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