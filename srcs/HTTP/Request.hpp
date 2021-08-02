#ifndef REQUEST_HPP
#define REQUEST_HPP

// #include "ServerEngine.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <set>
#include <iostream>
#include <fstream>
#include <map>
#include "../Parse//ParserConfig.hpp"
#include "Response.hpp"
#include "HTTP/Responders/Get/GetResponder.hpp"
#include "HTTP/Responders/Post/PostResponder.hpp"
#include "HTTP/Responders/Put/PutResponder.hpp"
#include "HTTP/Errors/HTTPError.hpp"
#include "HTTP/Errors/HTTPNotFound.hpp"
#include "HTTP/Errors/HTTPBadRequest.hpp"
#include "Autoindex.h"

class Request
{

private:
	/* data */
	std::string _request;
	std::string _currentLine;
	std::string _method;
	std::string _location;
	std::string _version;
	std::map<std::string, std::string> _headers;
	std::string _body;
	size_t _requestPosition;
	std::string _error;

	int parse_request(ServerData const &data);
	void parse_headers();
	void parse_body();

public:
	Request();
	Request(const std::string &request);
	virtual ~Request();

	void parse(std::string const &request_str, ServerData const &data);
	std::string respond(ParserConfig const &config, ServerData const &serverData) const;
	void clear();

	const std::string &getMethod() const;
	const std::string &getLocation() const;
	const std::string &getVersion() const;
	const std::map<std::string, std::string> &getHeaders() const;
	const std::string &getBody() const;
	void setError(std::string const &error);

};

#endif
