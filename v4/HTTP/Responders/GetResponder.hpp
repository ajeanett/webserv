#ifndef WEBSERV_GETRESPONDER_HPP
#define WEBSERV_GETRESPONDER_HPP

#include "IResponder.hpp"

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
