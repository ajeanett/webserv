#ifndef WEBSERV_DELETERESPONDER_HPP
#define WEBSERV_DELETERESPONDER_HPP

#include "HTTP/Responders/AResponder.hpp"

class DeleteResponder : public AResponder
{

public:
	DeleteResponder();
	DeleteResponder(DeleteResponder const &src);
	~DeleteResponder();
	DeleteResponder &operator=(DeleteResponder const &src);

	std::string respond(Request const &request, ServerData const &serverData, Response &response) const;

};


#endif //WEBSERV_DELETERESPONDER_HPP
