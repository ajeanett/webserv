#ifndef WEBSERV_HTTPBADREQUEST_HPP
#define WEBSERV_HTTPBADREQUEST_HPP

#include "HTTPError.hpp"

class HTTPBadRequest : public HTTPError
{

public:
	virtual const char *what() const throw();

};


#endif //WEBSERV_HTTPBADREQUEST_HPP
