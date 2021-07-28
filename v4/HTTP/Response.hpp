#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include <iostream>
#include <map>

class Response
{

public:
	Response();
	Response(Response const &src);
	~Response();
	Response &operator = (Response const &src);
	std::string str() const;

	void setVersion(const std::string &version);
	void setStatus(const std::string &statusCode, const std::string &statusMessage);
	void setBody(const std::string &body);
	std::map<std::string, std::string> &getHeaders();

private:
	std::string _version;
	std::string _statusCode;
	std::string _statusMessage;
	std::map<std::string, std::string> _headers;
	std::string _body;

};

#endif //WEBSERV_RESPONSE_HPP
