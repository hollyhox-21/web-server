#include <ctime>
#include <string>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <iterator>
#include <fcntl.h>
#include <unistd.h>


int main()
{
	int fd = open("/Users/ptycho/Documents/42cursus/web_server/new.html", O_RDWR);
	long len, lenght = 0;
	char buf[1025];
	while ((len = read(fd, &buf, 1024)) > 0)
		lenght += len;
	std::cout << lenght << std::endl;
	return 0;
}
