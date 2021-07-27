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
public:
    Request();
    Request(const std::string &request);
    virtual ~Request();
    void parse_request(void);
    void parse_headers(void);
    void parse_body(void);
};



#endif
