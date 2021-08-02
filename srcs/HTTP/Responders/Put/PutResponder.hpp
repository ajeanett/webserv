#ifndef WEBSERV_PUTRESPONDER_HPP
#define WEBSERV_PUTRESPONDER_HPP

#include "HTTP/Responders/IResponder.hpp"
#include "CGI/CGI.hpp"
#include "logging.hpp"

class PutResponder : public IResponder
{

public:
	PutResponder();
	PutResponder(PutResponder const &src);
	~PutResponder();
	PutResponder &operator=(PutResponder const &src);

	std::string respond(Request const &request, ParserConfig const &config, ServerData const &serverData) const;

};


#endif //WEBSERV_PUTRESPONDER_HPP
