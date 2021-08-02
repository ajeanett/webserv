#ifndef WEBSERV_IRESPONDER_HPP
#define WEBSERV_IRESPONDER_HPP

#include <iostream>
#include "Parse/ServerData.hpp"

class ParserConfig;
class Response;
class Request;

class IResponder
{

public:
	virtual ~IResponder() {}
	virtual std::string respond(Request const &request, ParserConfig const &config, ServerData const &serverData) const = 0;

};

#endif //WEBSERV_IRESPONDER_HPP
