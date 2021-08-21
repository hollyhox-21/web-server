#include "Response.hpp"


int Response::findLocation(std::map<std::string, Location>::iterator *it)
{
	std::string uri = _request.getUri();
	deleteMultiSl(uri);
	bool f = true;
	while (!uri.empty() && f)
	{
		for (*it = _serverSettings.locations.begin(); *it != _serverSettings.locations.end(); ++(*it))
			if ((*it)->first.find(uri) != std::string::npos)
				return 0;
			if (f && uri != "/" && !uri.empty())
			{
				uri.erase(uri.rfind('/') + 1, uri.length());
				if (uri != "/")
					uri.erase(uri.rfind('/'));
			}
	}
	return 1;
}

void Response::deleteMultiSl(std::string &uri)
{
	for (int i = 1; i < uri.length(); ++i)
		if (uri[i] == '/' && uri[i - 1] == '/')
			uri.erase(i,1);
}

void Response::generateAutoIndex(std::string &uri, std::map<std::string, Location>::iterator &it)
{
	DIR *dir;
	struct dirent *ent;
	std::string src;
	if ((dir = opendir(uri.c_str())) != nullptr)
	{
		char buffer[PATH_MAX];
		getcwd(buffer, sizeof(buffer));
		std::string path = buffer;
		if (path.find("/cmake-build-debug") != std::string::npos)
			path.erase(path.find("/cmake-build-debug"), 18);
		path += "/Network/html/autoindex.html"; // TODO: make file
		std::ifstream file(path);
		if (file.fail())
			fileNotFound(it->second.root);
		else
			genetateResponseAutoIn(dir, ent, src, file, path);
	}
}

void parseCgiResponse(std::string *a, std::string &header, std::string &body,
					  std::string &code, std::string &type)
{
	int pos;
	if ((pos = a->find("\r\n\r\n", 0)) != std::string::npos)
	{
		header = std::string(*a, 0, pos + 4);
		body = std::string(*a, pos + 4);
		if (header.find("Status: ", 0) != std::string::npos)
			code = a->substr(8, 3);
		if ((pos = a->find("Content-Type: ", 0)) != std::string::npos)
			type = a->substr(pos + 14, 24);
	}
}

void Response::createCgiResponse(std::string &uri)
{
	std::string *a = CgiService::getCgiResponse(_request, _serverSettings);
	_fileLength = atoi(_request.getValueMapHeader("Content-Length").c_str());
	std::cout << "\n\nPOST CGI:\n\n" << *a;
	std::string code, type, header, body;
	parseCgiResponse(a, header, body, code, type);
	_fileLength = body.length();
	body = makeHeader(uri, body, code, type);
	_fileSrc = new char[_fileLength + 1];
	_fileLength = body.length();
	for (unsigned long i = 0; i < _fileLength; ++i)
		_fileSrc[i] = body[i];
	_fileSrc[_fileLength] = 0;
					  }

void Response::createResponseWOCgi(std::string &uri, std::string code)
{
	int fd = open(uri.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0666);
	std::string src;
	int contLength = atoi(
			_request.getValueMapHeader("Content-Length").c_str());
	src += _request.getBody();
	_fileLength = src.length();
	src = makeHeader(uri, src, code, "");
	_fileLength = src.length();
	_fileSrc = new char[_fileLength + 1];
	for (unsigned long i = 0; i < _fileLength; ++i)
		_fileSrc[i] = src[i];
	_fileSrc[_fileLength] = 0;
	write(fd, _fileSrc + (_fileLength - contLength), _request.getBody().length());
	close(fd);
}

void Response::createSrc(std::map<std::string, Location>::iterator it, const std::string& code)
{
	std::string src;
	src = makeHeader(it->second.root, src, code, "");
	_fileLength = src.length();
	_fileSrc = new char[_fileLength + 1];
	for (unsigned long i = 0; i < _fileLength; ++i)
		_fileSrc[i] = src[i];
	_fileSrc[_fileLength] = 0;
}

std::string Response::makeHeader(std::string &uri, std::string &src, const std::string& code, const std::string& type)
{
	std::string header;
	header += "HTTP/1.1 ";
	header += code;
	header += "\r\n";
	header += "Connection: Keep-Alive\r\n";
	header += "Content-Length: ";
	header += std::to_string(_fileLength);
	header += "\r\n";
	if (!type.empty())
		header += "Content-Type: " + type;
	else if (uri.rfind(".html") != std::string::npos)
		header += "Content-Type: text/html\r\n";
	else if (uri.rfind(".png") != std::string::npos)
		header += "Content-Type: image/png\r\n";
	else if (uri.rfind(".jpg") != std::string::npos)
		header += "Content-Type: image/jpg\r\n";
	else if (uri.rfind(".jpeg") != std::string::npos)
		header += "Content-Type: image/jpg\r\n";
	else if (uri.rfind(".ico") != std::string::npos)
		header += "Content-Type: image/ico\r\n";
	else
		header += "Content-Type: text/html\r\n";
	header += "Date: ";
	header += getdate();
	header += "Server: WebServer By Monsters\r\n";
	header += "Last-Modified: ";
	header += getdate();
	header += "\r\n";
	header += src;
	_fileLength += header.length();
	return header;
}

char month[12][4] = {"Jan",
					 "Feb",
					 "Mar",
					 "Apr",
					 "May",
					 "Jun",
					 "Jul",
					 "Aug",
					 "Sep",
					 "Oct",
					 "Nov",
					 "Dec"};

char week[7][4] = {
		"Mod",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat",
		"Sun"
};

std::string Response::getdate()
{
	std::time_t t = std::time(nullptr);
	std::tm *now = std::localtime(&t);
	std::string date;
	date += week[now->tm_wday - 1];
	date += ", ";
	date += std::to_string(now->tm_mday);
	date += ' ';
	date += month[now->tm_mon];
	date += ' ';
	date += std::to_string(now->tm_year + 1900);
	date += ' ';
	date += std::to_string(now->tm_hour);
	date += ':';
	date += std::to_string(now->tm_min);
	date += ':';
	date += std::to_string(now->tm_sec);
	date += " GMT\r\n";
	return date;
}

void Response::errorFileExist()
{
	char buffer[PATH_MAX];
	getcwd(buffer, sizeof(buffer));
	std::string path = buffer;
	if (path.find("/cmake-build-debug") != std::string::npos)
		path.erase(path.find("/cmake-build-debug"), 18);
	path += "/Network/html/404.html";
	int fd = open(path.c_str(), O_RDONLY);
	char buff[1025];
	std::string src;
	long len;
	while ((len = read(fd, buff, 1024)) > 0)
	{
		_fileLength += len;
		std::string dst(buff, len);
		src += dst;
	}
	src = makeHeader(path, src, "404 Not Found", "");
	_fileLength = src.length();
	_fileSrc = new char[_fileLength + 1];
	for (unsigned long i = 0; i < _fileLength; ++i)
		_fileSrc[i] = src[i];
	_fileSrc[_fileLength] = 0;
}

void Response::errorFileNotExist(std::string &path)
{
	char buff[1025];
	std::string src;
	long len;
	int fd;
	while ((len = read(fd, buff, 1024)) > 0)
	{
		_fileLength += len;
		std::string dst(buff, len);
		src += dst;
	}
	src = makeHeader(path, src, "404 Not Found", "");
	_fileLength = src.length();
	_fileSrc = new char[_fileLength + 1];
	for (unsigned long i = 0; i < _fileLength; ++i)
		_fileSrc[i] = src[i];
	_fileSrc[_fileLength] = 0;
}

void Response::fileNotFound(std::string root)
{
	std::map<int, std::string>::iterator it = _serverSettings.errorPages.begin();
	for (; it != _serverSettings.errorPages.end() && it->first != 404; ++it) {	}
	std::string path;
	if (root.rfind('/') == (root.length() - 1))
		root.erase(root.length() - 1);
	path = root + it->second;
	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		errorFileExist();
	else
		errorFileNotExist(path);
}

void	Response::generateResponse(std::string uri, std::map<std::string, Location>::iterator it)
{
	deleteMultiSl(uri);
	int fd = open(uri.c_str(), O_RDONLY);
	if (fd < 0)
	{
		fileNotFound(it->second.root);
		return;
	} else
	{
		char buff[1025];
		std::string src;
		long len;
		while ((len = read(fd, buff, 1024)) > 0)
		{
			_fileLength += len;
			std::string dst(buff, len);
			src += dst;
		}
		_fileLength = src.length();
		src = makeHeader(uri, src, "200 OK", "");
		_fileLength = src.length();
		_fileSrc = new char[_fileLength + 1];
		for (unsigned long i = 0; i < _fileLength; ++i)
			_fileSrc[i] = src[i];
		_fileSrc[_fileLength] = 0;
		close(fd);
	}
}

void Response::genetateResponseAutoIn(DIR *dir, struct dirent *ent, std::string src, std::ifstream &file, std::string path)
{
	std::string one_line;
	while (getline(file, one_line))
	{
		if (one_line.find("<$ListOfFiles>") != std::string::npos)
			break;
		one_line += "\n";
		src += one_line;
	}
	while ((ent = readdir(dir)) != nullptr)
	{
		std::string pre = "\t\t\t<li class=\"files\"><a href=\"";
		std::string path_dir = _request.getUri();
		if (path_dir[path_dir.length() - 1] == '/')
			pre += _request.getUri() + ent->d_name;
		else
			pre += _request.getUri() + '/' + ent->d_name;
		pre += "\">";
		pre += ent->d_name;
		pre += "</a></li>\n";
		src += pre;
	}
	while (getline(file, one_line))
	{
		one_line += "\n";
		src += one_line;
	}
	_fileLength = src.length();
	src = makeHeader(path, src, "200 OK", "");
	_fileLength = src.length();
	_fileSrc = new char[_fileLength + 1];
	for (unsigned long i = 0; i < _fileLength; ++i)
		_fileSrc[i] = src[i];
	_fileSrc[_fileLength] = 0;
}

void Response::methodnotallowed(std::string root)
{
	std::map<int, std::string>::iterator it = _serverSettings.errorPages.begin();
	for (; it != _serverSettings.errorPages.end() && it->first != 405; ++it) {	}
	std::string path;
	if (root.rfind('/') == (root.length() - 1))
		root.erase(root.length() - 1);
	path = root + it->second;
	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		fileNotFound(root);
	else
	{
		char buff[1025];
		std::string src;
		long len;
		while ((len = read(fd, buff, 1024)) > 0)
		{
			_fileLength += len;
			std::string dst(buff, len);
			src += dst;
		}
		src = makeHeader(path, src, "405 Method Not Allowed", "");
		_fileLength = src.length();
		_fileSrc = new char[_fileLength + 1];
		for (unsigned long i = 0; i < _fileLength; ++i)
			_fileSrc[i] = src[i];
		_fileSrc[_fileLength] = 0;
	}
}

void Response::responseOnHead()
{
	std::map<std::string, Location>::iterator it;
	int cod = findLocation(&it);
	if (cod || !it->second.methods["HEAD"])
	{
		methodnotallowed(it->second.root);
		return;
	}
	responseOnGet();
	std::string resp = std::string(_fileSrc, _fileLength);
	unsigned long len = resp.find(CRLF_END) + 4;
	char *oldResp = _fileSrc;
	_fileSrc = new char[len + 1];
	for (int i = 0; i < len; ++i)
		_fileSrc[i] = oldResp[i];
	_fileLength = len;
	_fileSrc[len] = 0;
	delete[] oldResp;
}
