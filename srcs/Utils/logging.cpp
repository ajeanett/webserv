#include "logging.hpp"

void displayTimeStamp()
{
	char buf[1024];
	struct timeval t;
	struct tm* tm;

	gettimeofday(&t, NULL);
	tm = localtime(&t.tv_sec);
	std::strftime(buf, 1024, "%H:%M:%S", tm);

	std::cout << '[' << buf << "] ";
}
