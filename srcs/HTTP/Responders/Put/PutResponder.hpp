#ifndef WEBSERV_PUTRESPONDER_HPP
#define WEBSERV_PUTRESPONDER_HPP

#include "HTTP/Responders/AResponder.hpp"
#include "CGI/CGI.hpp"
#include "logging.hpp"

class PutResponder : public AResponder
{

public:
	PutResponder();
	PutResponder(PutResponder const &src);
	~PutResponder();
	PutResponder &operator=(PutResponder const &src);

	std::string respond(Request const &request, ServerData const &serverData, Response &response) const;

};


#endif //WEBSERV_PUTRESPONDER_HPP
