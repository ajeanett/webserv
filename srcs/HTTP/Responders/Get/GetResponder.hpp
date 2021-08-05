#ifndef WEBSERV_GETRESPONDER_HPP
#define WEBSERV_GETRESPONDER_HPP

#include "HTTP/Responders/AResponder.hpp"
#include "HTTP/Autoindex.h"
#include "CGI/CGI.hpp"
#include "logging.hpp"

class GetResponder : public AResponder
{

public:
	GetResponder();
	GetResponder(GetResponder const &src);
	~GetResponder();
	GetResponder &operator = (GetResponder const &src);

	std::string respond(Request const &request, ServerData const &serverData, Response &response) const;

};

#endif //WEBSERV_GETRESPONDER_HPP
