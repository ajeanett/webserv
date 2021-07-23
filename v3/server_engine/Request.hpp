#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "ServerEngine.hpp"

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
    Request(std::string request);
    virtual ~Request();
    void parse_request(void);
    void parse_headers(void);
    void parse_body(void);
};



#endif
