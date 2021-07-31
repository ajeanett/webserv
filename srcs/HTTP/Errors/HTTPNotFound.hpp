#ifndef WEBSERV_HTTPNOTFOUND_HPP
#define WEBSERV_HTTPNOTFOUND_HPP

#include "HTTPError.hpp"

class HTTPNotFound : public HTTPError
{

public:
	virtual const char *what() const throw();

};


#endif //WEBSERV_HTTPNOTFOUND_HPP
