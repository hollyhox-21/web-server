#include <ctime>
#include <string>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <iterator>


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

std::string getdate()
{
	std::string s = "Date: Mon, 27 Jul 2009 12:28:53 GMT";
	std::time_t t = std::time(0);
	std::tm *now = std::localtime(&t);
	std::string date;
	date +="Date: ";
	date += week[now->tm_wday];
	date += ",";
	date += now->tm_mday;
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
	date += ' ';
	date += now->tm_zone;
	date += "\r\n";
	return date;
}

int main()
{
	std::string s = "Date: Mon, 27 Jul 2009 12:28:53 GMT";
	std::time_t t = std::time(0);
	std::tm *now = std::localtime(&t);
	std::string date;
	date +="Date: ";
	date += week[now->tm_wday];
	date += ",";
	date += now->tm_mday;
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
	date += ' ';
	date += now->tm_zone;
	std::cout << date << std::endl;
	//	std::cout << "Date: " << week[now->tm_wday] << ", " << now->tm_mday << ' ' << month[now->tm_mon] << ' ' << (now->tm_year + 1900) << ' '
//	 << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << ' ' << now->tm_zone << std::endl;
	return 0;
}

//class timefmt
//{
//public:
//	timefmt(std::string fmt)
//			: format(fmt) { }
//
//	friend std::ostream& operator <<(std::ostream &, timefmt const &);
//
//private:
//	std::string format;
//};
//
//std::ostream& operator <<(std::ostream& os, timefmt const& mt)
//{
//	std::ostream::sentry s(os);
//
//	if (s)
//	{
//		std::time_t t = std::time(0);
//		std::tm const* tm = std::localtime(&t);
//		std::ostreambuf_iterator<char> out(os);
//
//		std::use_facet<std::time_put<char> >(os.getloc())
//				.put(out, os, os.fill(),
//					 tm, &mt.format[0], &mt.format[0] + mt.format.size());
//	}
//
//	os.width(0);
//
//	return os;
//}
//
//int main()
//{
//	std::cout << timefmt("%c");
//}