#include "Request.hpp"
#include "Responders/IResponder.hpp"
#include <vector>

Request::Request(const std::string &request) :
	_request(request),
	_requestPosition(0)
{
}

Request::Request() : _requestPosition(0)
{
}

Request::~Request()
{
}

void Request::parse_request()
{

	// Ищем первую строку

	this->_requestPosition = 0;

	size_t findPosition = _request.find("\r\n", this->_requestPosition);

	if (findPosition != std::string::npos)
	{
		this->_currentLine = _request.substr(this->_requestPosition, findPosition - this->_requestPosition);
		this->_requestPosition = findPosition + 2;
	}
	else
	{
		perror("ERROR. Invalid message.");
	}

	size_t _linePosition = 0;
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

void Request::parse_headers()
{
	// Ищем хедеры и заносим их в мапу
	size_t findPosition = _request.find("\r\n\r\n", _requestPosition);

	if (findPosition != std::string::npos)
	{
		_currentLine = _request.substr(_requestPosition, findPosition - _requestPosition);
		_requestPosition = findPosition + 4;
	}
	else
	{
		perror("ERROR. Invalid message.");
	}

	size_t _linePosition = 0;

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
}

void Request::parse_body()
{
	// заносим боди. В get его не будет. Get - body нет. Put - body тут.
	// если длина боди больше, чем длина content-lenght, то заносим только content-lenght
	// взять из мапы хедеров
	if (_headers.find("Content-Length") != _headers.end())
	{
		int i = std::stoi(_headers["Content-Length"]);
		_body = _request.substr(this->_requestPosition, i);
	}
	else
		_body = _request.substr(this->_requestPosition);

//	_body = _request.erase(0, this->_requestPosition); альтернативный вариант получения body, посмотреть потом, что быстрее - substr или erase

	size_t	start_chunk_size = 0;
	size_t	start_body = 0;
	size_t	end_body = 0;
	size_t	size_body = 0;

	size_t	chunk_size_length;
	size_t	chunk_size;
	char  **tmp = NULL;
//	std::string new_body = "";
	if (_headers.find("Transfer-Encoding") != _headers.end() && _headers["Transfer-Encoding"] == "chunked")
	{
		while ((start_body = _body.find("\r\n", start_chunk_size)) != std::string::npos)
		{
//			chunk_size = strtol(_body.substr(start_chunk_size, start_body - start_chunk_size).c_str());
//			std::string s = "3e8";
			chunk_size = std::strtol(_body.substr(start_chunk_size, start_body - start_chunk_size).c_str(), tmp, 16);
			std::cout << chunk_size << std::endl;
			start_body += 2;
			_body.erase(0, start_body);
			end_body = _body.find("\r\n", 0);
			if (chunk_size != end_body)
			{
				_error = "400";
				return;
			}
			_body.erase(end_body, 2);
		}
	}
}

void Request::parse(const std::string &request_str)
{
	_request = request_str;
	try
	{
		parse_request();
		parse_headers();
		parse_body();
	}
	catch (HTTPError &e)
	{
		_error = e.what();
	}
}

std::string Request::respond(ParserConfig const &config, ServerData const &serverData) const
{
	if (!_error.empty())
	{
		if (_error == "400")
			return (Response().error(_error, "Bad Request"));
		if (_error == "404")
			return (Response().error(_error, "Not Found"));
		if (_error == "405")
			return (Response().error(_error, "Method Not Allowed"));
		if (_error == "500")
			return (Response().error(_error, "Internal Server Error"));
	}

	IResponder *responder;

	if (_method == "GET")
		responder = new GetResponder();
	else if (_method == "POST")
		responder = new PostResponder();
	else if (_method == "PUT")
		responder = new PutResponder();
//	else if (_startLine.find("method")->second == "DELETE")
//		responder = new DeleteResponder();
//	else if (_startLine.find("method")->second == "PUT")
//		responder = new PutResponder();
	else if (_method == "HEAD")
		return (Response().error("405", "Method Not Allowed"));
	else
		return (Response().error("405", "Method Not Allowed"));
	std::string response = responder->respond(*this, config, serverData);
	delete responder;
	return (response);
}

void Request::clear()
{
	_request.clear();
	_currentLine.clear();
	_method.clear();
	_location.clear();
	_version.clear();
	_headers.clear();
	_body.clear();
	_requestPosition = 0;
	_error.clear();
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

void Request::setError(std::string const &error)
{
	_error = error;
}
