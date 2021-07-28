#ifndef WEBSERV_IRESPONDER_HPP
#define WEBSERV_IRESPONDER_HPP

#include <iostream>
#include "../../parse_config/ParserConfig.hpp"
#include "../Response.hpp"
#include "../Request.hpp"

class Request;

class IResponder
{

public:
	virtual ~IResponder() {}
	virtual std::string respond(Request const &request, ParserConfig const &config, ServerData const &serverData) const = 0;

};

#endif //WEBSERV_IRESPONDER_HPP