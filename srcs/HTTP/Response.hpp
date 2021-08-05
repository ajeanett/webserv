#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "HTTP/Errors/HTTPError.hpp"

class Response
{

public:
	Response();
	Response(Response const &src);
	~Response();
	Response &operator = (Response const &src);

	std::string str() const;
	std::string error(std::string const &code, const std::string &statusMessage);
	std::string error(std::string const &code);
	std::string error(HTTPError const &error);

	void setVersion(const std::string &version);
	void setStatus(const std::string &statusCode, const std::string &message);
	void setBody(const std::string &body);
	std::map<std::string, std::string> &getHeaders();
	std::map<std::string, std::string> const &getHeaders() const;
	const std::string &getStatusCode() const;

private:
	std::string _version;
	std::string _statusCode;
	std::string _statusMessage;
	std::map<std::string, std::string> _headers;
	std::string _body;

};

#endif //WEBSERV_RESPONSE_HPP
