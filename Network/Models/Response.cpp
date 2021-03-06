#include <vector>
#include "Response.hpp"
#include <fcntl.h>

Response::~Response()
{
	delete _fileSrc;
}

Response::Response(Request &request, t_server &serverSettings): _request(request), _serverSettings(serverSettings), _fileLength(0), _fileSrc(nullptr)
{
	if (request.getMethod().find("GET", 0, 3) != std::string::npos)
		responseOnGet();
	else if (request.getMethod().find("POST", 0, 4) != std::string::npos)
		responseOnPost();
	else if (request.getMethod().find("DELETE", 0, 6) != std::string::npos)
		responseOnDelete();
	else if (request.getMethod().find("PUT", 0, 3) != std::string::npos)
		responseOnPut();
	else if (request.getMethod().find("HEAD", 0, 4) != std::string::npos)
		responseOnHead();
}

int Response::responseOnGet()
{
	std::map<std::string, Location>::iterator it;
	int cod = findLocation(&it);
	if (it->second.redirect.first == 301 && !it->second.redirect.second.empty())
		cod = rediraction(&it);
	if (cod == 1)
		return returnErrors();
	if (!it->second.methods["GET"])
		return methodnotallowed(it->second.root);
	std::string uri = it->second.root + "/";
		uri += _request.getUri().substr(_request.getUri().find(it->first) + it->first.length());
	deleteMultiSl(uri);
	struct stat buf;
	if (::stat(uri.c_str(), &buf) != 0)
		fileNotFound(it->second.root);
	else if (buf.st_mode & S_IFREG)
		generateResponse(uri, it);
	else if (buf.st_mode & S_IFDIR)
	{
		if (it->second.autoindex.find("on", 0, 2) != std::string::npos)
			generateAutoIndex(uri, it);
		else if (it->second.autoindex.find("off", 0, 2) != std::string::npos)
			generateResponse(uri + "/" + it->second.index, it);
	}
	return 0;
}

int Response::responseOnPost()
{
	std::map<std::string, Location>::iterator it;
	int cod = findLocation(&it);
	if (it->second.redirect.first == 301 && !it->second.redirect.second.empty())
		cod = rediraction(&it);
	if (cod == 1)
		return returnErrors();
	if (!it->second.methods["POST"])
		return methodnotallowed(it->second.root);
	std::string uri = it->second.root + "/";
	uri += _request.getUri().substr(_request.getUri().find(it->first) + it->first.length());
	deleteMultiSl(uri);
	struct stat buf;
	::stat(uri.c_str(), &buf);
	if (it->second.getCgi().first == ".bla" && !it->second.getCgi().second.empty())
		createCgiResponse(uri);
	else if (buf.st_mode & S_IFDIR)
		fileNotFound(it->second.root);
	else
		createResponseWOCgi(uri, "200 OK");
	return 0;
}

int Response::responseOnDelete()
{
	std::map<std::string, Location>::iterator it;
	int cod = findLocation(&it);
	if (it->second.redirect.first == 301 && !it->second.redirect.second.empty())
		cod = rediraction(&it);
	if (cod == 1)
		return returnErrors();
	if (!it->second.methods["DELETE"])
		return methodnotallowed(it->second.root);
	std::string uri = it->second.root + "/";
	uri += _request.getUri().substr(_request.getUri().find(it->first) + it->first.length());
	deleteMultiSl(uri);
	struct stat buf;
	if (::stat(uri.c_str(), &buf) != 0)
		fileNotFound(it->second.root);
	if (buf.st_mode & S_IFREG)
	{
		if (!std::remove(uri.c_str()))
		{
			std::string body = "<html>\n\t<body>\n"
							   "\t\t<h1>File deleted.</h1>\n"
							   "\t</body>\n</html>";
			_fileLength = body.length();
			std::string header = makeHeader(uri, body, "200 OK", "");
			_fileLength = header.length();
			_fileSrc = new char[_fileLength + 1];
			for (size_t i = 0; i < _fileLength; ++i)
				_fileSrc[i] = header[i];
			_fileSrc[_fileLength] = 0;
		}
		else
			fileNotFound(it->second.root);
	}
	else if (buf.st_mode & S_IFDIR)
		fileNotFound(it->second.root);
	return 0;
}

int Response::responseOnPut()
{
	std::map<std::string, Location>::iterator it;
	int cod = findLocation(&it);
	if (it->second.redirect.first == 301 && !it->second.redirect.second.empty())
		cod = rediraction(&it);
	if (cod == 1)
		return returnErrors();
	if (!it->second.methods["PUT"])
		return methodnotallowed(it->second.root);
	std::string uri = it->second.root + "/";
	uri += _request.getUri().substr(_request.getUri().find(it->first) + it->first.length());
	deleteMultiSl(uri);
	struct stat buf;
	if (::stat(uri.c_str(), &buf) != 0)
		createResponseWOCgi(uri, "201 Created");
	if (buf.st_mode & S_IFREG)
		createResponseWOCgi(uri, "200 OK");
	else if (buf.st_mode & S_IFDIR)
		fileNotFound(it->second.root);
	return 0;
}

int Response::responseOnHead()
{
	std::map<std::string, Location>::iterator it;
	int cod = findLocation(&it);
	if (it->second.redirect.first == 301 && !it->second.redirect.second.empty())
		cod = rediraction(&it);
	if (cod == 1)
		return returnErrors();
	if (!it->second.methods["HEAD"])
		return methodnotallowed(it->second.root);
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
	return 0;
}


std::pair<char *, int> Response::toFront()
{
	return std::pair<char *, int>(_fileSrc, _fileLength);
}
