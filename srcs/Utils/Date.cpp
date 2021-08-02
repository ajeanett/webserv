#include "Date.hpp"

std::string Date::get_time()
{
	std::string d;

	gettimeofday(&_t, NULL);
	_tm = gmtime(&_t.tv_sec);
	strftime(_buf, 1000, "%a, %_d %b %Y %H:%M:%S GMT", _tm);

	d = std::string(_buf);

	return (d);

}
