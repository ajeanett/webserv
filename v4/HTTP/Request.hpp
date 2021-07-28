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
#include "../parse_config/ParserConfig.hpp"
#include "Response.hpp"

class Request
{
private:
    /* data */
    std::string _request;
    std::string _currentLine;
    std::map<std::string, std::string> _startLine; // первая строка запроса, три элемента в мапе -  method, location, version of http
    std::map<std::string, std::string> _headers; // хедеры
    std::string                        _body; // боди, есть не всегда
    size_t                             _requestPosition;
	void parse_request();
	void parse_headers();
	void parse_body();

public:
    Request();
    Request(const std::string &request);
    virtual ~Request();
	void parse(std::string const &request_str);
	std::string respond(ParserConfig const &config, ServerData const &serverData) const;

	const std::map<std::string, std::string> &getStartLine() const;
	const std::map<std::string, std::string> &getHeaders() const;
	const std::string &getBody() const;
};



#endif
