#ifndef DATE_HPP
#define DATE_HPP

#include <time.h> // - для strftime
#include <sys/time.h> // - для gettimeofday
#include <string>

class Date
{
	private:
		char				_buf[1000];
		struct timeval		_t;
		struct tm*			_tm;
		static std::string	_ret;

public:
		Date(){}
		~Date(){}
		std::string get_time();

};

#endif