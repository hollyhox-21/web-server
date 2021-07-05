#include <iostream>
#include <map>
#include <string>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <dirent.h>
#include <istream>
#include <ios>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
//	std::vector<std::string> _autoindex;
//	char *str = new char[6];
//	str[0] = '1';
//	str[1] = '2';
//	str[2] = '3';
//	str[3] = '\0';
//	str[4] = '3';
//	char *str1 = new char[6];
//	str1[0] = '1';
//	str1[1] = '2';
//	str1[2] = '3';
//	str1[3] = '\0';
//	str1[4] = '3';
//	std::string line(str, 6);
//	std::string line1(str1, 6);
//	std::string file(line + line);
//	_autoindex.push_back(file);
//	for (int i = 0; i < _autoindex[0].length(); ++i)
//	{
//		std::cout << _autoindex[0][i] << std::endl;
//	}
	int fd = open("/Users/ptycho/Documents/42cursus/web-server/Network/html/autoindex.html", O_RDONLY);
	if (fd < 0)
		nullptr;
	else
	{
		char *date;
		int ret;
		struct stat buf;
		if ((ret = ::stat("/Users/ptycho/Documents/42cursus/web-server/Network/html/autoindex.html", &buf))!=0)
		{
			fprintf(stderr, "stat failure error .%d", ret);
			abort();
		}
		date = asctime(localtime(&buf.st_ctime));
		printf("\n %s\n", date);
		if (buf.st_mode & S_IFDIR)
			printf("\n %d mode\n", buf.st_mode & S_IFREG);
		printf("\n %lld size\n", buf.st_size);
	}
////				std::ifstream file(it->second.root + name);
//	else
//	{
//		char buf[1024];
//		std::string src;
//		int len;
//		int _fileLength = 0;
//		while ((len = read(fd, buf, 1024)) > 0)
//		{
//			_fileLength += len;
//			std::string dst(buf, len);
//			src += dst;
//			std::cout << len << std::endl;
//		}
//		char *res = new char[_fileLength];
//		for (int i = 0; i < _fileLength; ++i)
//		{
//			res[i] = src[i];
//			write(1, &res[i], 1);
//		}
//	}











//	std::ifstream file("/Users/ptycho/Documents/42cursus/web-server");
//	file.seekg(0, std::ios::end);
//	std::cout << file.good() << std::endl;
//	std::string uri = std::string("/abcdfdr/index.html/");
//	if (uri.rfind('/') == (uri.length() - 1))
//		uri.erase(uri.rfind('/'));
//	std::string dir = std::string(uri), name = std::string(uri);
//	dir.erase(dir.rfind('/'), dir.length());
//	name.erase(0, name.rfind('/') + 1);
//	std::cout << dir << " " << name << std::endl;
//	uri.erase(uri.rfind('/') + 1, uri.length());
//	std::cout << uri << " " << uri.size() << " " << uri.length() << std::endl;
//	std::string one_line;
//	std::ifstream file("/Users/ptycho/Documents/42cursus/web-server/autoindex.html");
//	std::ofstream ofl("/Users/ptycho/Documents/42cursus/web-server/autoindex.html");
//	while (getline(file, one_line))
//	{
//		ofl << one_line << '\n';
//		if (one_line.find("<$ListOfFiles>") != std::string::npos)
//			break;
//	}
//	for (int i = 0; i < 5; ++i)
//	{
//		ofl << "s" << '\n';
//	}
//	while (getline(file, one_line))
//	{
//		ofl << one_line << '\n';
//	}
	return 0;
}