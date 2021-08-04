#ifndef WEBSERV_POSTRESPONDER_HPP
#define WEBSERV_POSTRESPONDER_HPP

#include "HTTP/Responders/AResponder.hpp"

class PostResponder : public AResponder
{

public:
	PostResponder();
	PostResponder(PostResponder const &src);
	~PostResponder();
	PostResponder &operator = (PostResponder const &src);

	std::string respond(Request const &request, ServerData const &serverData) const;

};


#endif //WEBSERV_POSTRESPONDER_HPP
