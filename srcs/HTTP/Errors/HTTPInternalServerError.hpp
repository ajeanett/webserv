#ifndef WEBSERV_HTTPINTERNALSERVERERROR_HPP
#define WEBSERV_HTTPINTERNALSERVERERROR_HPP

#include "HTTPError.hpp"

class HTTPInternalServerError : public HTTPError
{

public:
	virtual const char *what() const throw();

};


#endif //WEBSERV_HTTPINTERNALSERVERERROR_HPP
