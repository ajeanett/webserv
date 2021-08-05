#ifndef WEBSERV_ARESPONDER_HPP
#define WEBSERV_ARESPONDER_HPP

#include "Parse/ServerData.hpp"
#include <list>

class ParserConfig;
class Response;
class Request;

class AResponder
{

public:
	AResponder();
	AResponder(AResponder const &src);
	virtual ~AResponder();
	AResponder &operator=(AResponder const &src);

	virtual std::string respond(Request const &request, ServerData const &serverData, Response &response) const = 0;
	static LocationData const *getCurrentLocation(std::vector<LocationData> const &locations, std::string const &requestLocation, std::string const &method) ;

};

#endif //WEBSERV_ARESPONDER_HPP
