#ifndef WEBSERV_HTTPERROR_HPP
#define WEBSERV_HTTPERROR_HPP

#include <iostream>

class HTTPError : public std::exception
{

public:
	virtual const char *what() const throw();
	static std::string getMessage(std::string const &code);

};


#endif //WEBSERV_HTTPERROR_HPP
