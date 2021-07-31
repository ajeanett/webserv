#ifndef WEBSERV_HTTPERROR_HPP
#define WEBSERV_HTTPERROR_HPP

#include <iostream>

class HTTPError : public std::exception
{

public:
	virtual const char *what() const throw();

};


#endif //WEBSERV_HTTPERROR_HPP
