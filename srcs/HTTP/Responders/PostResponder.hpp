#ifndef WEBSERV_POSTRESPONDER_HPP
#define WEBSERV_POSTRESPONDER_HPP

#include "IResponder.hpp"

class PostResponder : public IResponder
{

public:
	PostResponder();
	PostResponder(PostResponder const &src);
	~PostResponder();
	PostResponder &operator = (PostResponder const &src);

	std::string respond(Request const &request, ParserConfig const &config, ServerData const &serverData) const;

};


#endif //WEBSERV_POSTRESPONDER_HPP
