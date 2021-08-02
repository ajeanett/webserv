#ifndef WEBSERV_GETRESPONDER_HPP
#define WEBSERV_GETRESPONDER_HPP

#include "HTTP/Responders/IResponder.hpp"
#include "HTTP/Autoindex.h"
#include "CGI/CGI.hpp"
#include "logging.hpp"

class GetResponder : public IResponder
{

public:
	GetResponder();
	GetResponder(GetResponder const &src);
	~GetResponder();
	GetResponder &operator = (GetResponder const &src);

	std::string respond(Request const &request, ParserConfig const &config, ServerData const &serverData) const;

};

#endif //WEBSERV_GETRESPONDER_HPP
