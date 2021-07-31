#include "Request.hpp"
#include "Responders/IResponder.hpp"
#include <vector>

Request::Request(const std::string &request) : _request(request) //
{
    this->_requestPosition = 0;
}

Request::Request(){
    this->_requestPosition = 0;
}

Request::~Request()
{
}

void Request::parse_request(void){


    // Ищем первую строку

    this->_requestPosition = 0;

    size_t	findPosition = _request.find("\r\n", this->_requestPosition);

    if (findPosition  != std::string::npos)
    {
        this->_currentLine = _request.substr(this->_requestPosition, findPosition  - this->_requestPosition);
        this->_requestPosition = findPosition + 2;
    }
    else
    {
        perror("ERROR. Invalid message.");
    }

    size_t	_linePosition = 0;
    // ifs , ss
    _linePosition = _currentLine.find_first_of(' ');
    if (_linePosition != std::string::npos)
        _method = _currentLine.substr(0, _linePosition);
    findPosition = _currentLine.find(' ', _linePosition + 1);
    if (_linePosition != std::string::npos)
        _location = _currentLine.substr(_linePosition + 1, findPosition - _linePosition - 1);
    _linePosition = findPosition;
    findPosition = _currentLine.size();
    if (_linePosition != std::string::npos)
        _version = _currentLine.substr(_linePosition + 1, findPosition - _linePosition - 1);
//         // Печать стартовой строки для проверки
//         std::map<std::string,std::string>::iterator it;
//         std::cout << "MAP First Line" << std::endl;
//         for (it=_startLine.begin(); it!=_startLine.end(); it++)
//             std::cout << it->first << " " << it->second << std::endl;
//         std::cout << "END of MAP First Line" << std::endl;
}

void Request::parse_headers(void){

    // Ищем хедеры и заносим их в мапу
    size_t	findPosition = _request.find("\r\n\r\n", this->_requestPosition);

    if (findPosition  != std::string::npos)
    {
        this->_currentLine = _request.substr(this->_requestPosition, findPosition  - this->_requestPosition);
        this->_requestPosition = findPosition + 4;
    }
    else
    {
        perror("ERROR. Invalid message.");
    }

    size_t	_linePosition = 0;

    std::vector<std::string> headers_raw;

    while ((findPosition = _currentLine.find("\r\n", _linePosition)) != std::string::npos)
    {
        headers_raw.push_back(_currentLine.substr(_linePosition, findPosition - _linePosition));
        _linePosition = findPosition + 2;
    }
    headers_raw.push_back(_currentLine.substr(_linePosition));

	//  Печать хедеров для проверки
	std::vector<std::string>::iterator it;
	// std::cout << "Vector Headers" << std::endl;
	// for (it = headers_raw.begin(); it != headers_raw.end(); it++)
	//     std::cout << *it << std::endl;
	// std::cout << "Vector Headers END" << std::endl;

	std::string delim = ": ";
	std::string header_key;
	std::string header_value;


    for (it = headers_raw.begin(); it != headers_raw.end(); it++)
    {
        _linePosition = 0;
        if ((findPosition = (*it).find(delim, _linePosition)) != std::string::npos)
        {
            header_key = (*it).substr(_linePosition, findPosition - _linePosition);
            _linePosition = findPosition + 2;
        }
        header_value = (*it).substr(_linePosition);
        _headers[header_key] = header_value;
    }

    // std::map<std::string,std::string>::iterator itm;
    // std::cout << "HEADERS in MAP" << std::endl;
    // for (itm = _headers.begin(); itm != _headers.end(); itm++)
    //     std::cout <<"Key: " <<itm->first << " Value: " << itm->second << std::endl;
    // std::cout << "HEADERS in MAP END" << std::endl;
}

void Request::parse_body(void){ // 

    // заносим боди. В get его не будет. Get - body нет. Put - body тут.
    // если длина боди больше, чем длина content-lenght, то заносим только content-lenght
    // взять из мапы хедеров
    if (_headers.find("content-lenght") != _headers.end())
    {
        int i = atoi(_headers["content-lenght"].c_str());
        _body = _request.substr(this->_requestPosition, i);
    }
    else
        _body = _request.substr(this->_requestPosition);
        //  Печать боди для проверки
    // std::cout << "BODY" << std::endl << _body<< std::endl << "BODY END" << std::endl;

}

class HTTPException : public std::exception
{
public:
	void respond();
};

void Request::parse(const std::string &request_str)
{
	_request = request_str;
	try
	{
		parse_request();
		parse_headers();
		parse_body();
	}
	catch(HTTPException &e)
	{
		e.respond();
	}
}

std::string Request::respond(ParserConfig const &config, ServerData const &serverData) const
{
	IResponder *responder;

	if (_method == "GET")
		responder = new GetResponder();
//	else if (_startLine.find("method")->second == "POST")
//		responder = new PostResponder();
//	else if (_startLine.find("method")->second == "DELETE")
//		responder = new DeleteResponder();
//	else if (_startLine.find("method")->second == "PUT")
//		responder = new PutResponder();
	else
		return (Response().error("405", "Method Not Allowed"));
	std::string response = responder->respond(*this, config, serverData);
	delete responder;
	return (response);
}

const std::string &Request::getMethod() const
{
	return (_method);
}

const std::string &Request::getVersion() const
{
	return (_version);
}

const std::string &Request::getLocation() const
{
	return (_location);
}

const std::map<std::string, std::string> &Request::getHeaders() const
{
	return (_headers);
}

const std::string &Request::getBody() const
{
	return (_body);
}
