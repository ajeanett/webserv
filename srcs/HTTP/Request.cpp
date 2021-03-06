#include "Request.hpp"
#include "Responders/AResponder.hpp"
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

int	Request::parse_request(t_req_data &buffer)
{
	static_cast<void>(buffer);
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
		perror("ERROR. BAD REQUEST");
		throw HTTPBadRequest();
	}

	size_t _linePosition = _currentLine.find_first_of(' ');
	if (_linePosition != std::string::npos)
		_method = _currentLine.substr(0, _linePosition);
	findPosition = _currentLine.find(' ', _linePosition + 1);
	if (_linePosition != std::string::npos)
		_location = _currentLine.substr(_linePosition + 1, findPosition - _linePosition - 1);
	_linePosition = findPosition;
	findPosition = _currentLine.size();
	if (_linePosition != std::string::npos)
		_version = _currentLine.substr(_linePosition + 1, findPosition - _linePosition - 1);

	findPosition = _request.find("\r\n\r\n", _requestPosition);

	if (findPosition != std::string::npos)
	{
		_currentLine = _request.substr(_requestPosition, findPosition - _requestPosition);
		_requestPosition = findPosition + 4;
	}
	else
	{
		perror("ERROR. Invalid message.");
		throw HTTPBadRequest();
	}
//	if (buffer._is_body)
//		_currentLine = _request.substr(_requestPosition, buffer._start_body - _requestPosition - 4);
//	else
//	{
//		throw HTTPBadRequest();
//	}
	return 0;
}

void Request::parse_headers(t_req_data &buffer)
{
	static_cast<void>(buffer);
	// Ищем хедеры и заносим их в мапу
	size_t	findPosition = 0;

	size_t _linePosition = 0;

	std::vector<std::string> headers_raw;

	while ((findPosition = _currentLine.find("\r\n", _linePosition)) != std::string::npos)
	{
		headers_raw.push_back(_currentLine.substr(_linePosition, findPosition - _linePosition));
		_linePosition = findPosition + 2;
	}
	headers_raw.push_back(_currentLine.substr(_linePosition));

	//  Печать хедеров для проверки
	// std::cout << "Vector Headers" << std::endl;
//	 for (it = headers_raw.begin(); it != headers_raw.end(); it++)
//		std::cout << *it << std::endl;
//	 for (it = headers_raw.begin(); it != headers_raw.end(); it++)
//	     std::cout << *it << std::endl;
	// std::cout << "Vector Headers END" << std::endl;

	std::string delim = ": ";
	std::string header_key;
	std::string header_value;

	for (std::vector<std::string>::iterator it = headers_raw.begin(); it != headers_raw.end(); it++)
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

void Request::parse_body(t_req_data &buffer, ServerData const &serverData)
{
	static_cast<void>(buffer);
	// заносим боди. В get его не будет. Get - body нет. Put - body тут.
	// если длина боди больше, чем длина content-lenght, то заносим только content-lenght
	// взять из мапы хедеров

	if (_headers.find("Content-Length") != _headers.end())
	{
		size_t i = std::stoi(_headers["Content-Length"]);
		_body = _request.substr(this->_requestPosition, i);
	}
	else
		_body = _request.substr(this->_requestPosition);

	size_t	start_chunk_size = 0;
	size_t	start_body;
	size_t	chunk_size;

	if (_headers.find("Transfer-Encoding") != _headers.end() && _headers["Transfer-Encoding"] == "chunked")
	{
		std::string tmp;
		while ((start_body = _body.find("\r\n", start_chunk_size)) != std::string::npos)
		{
			std::string head_chunk = _body.substr(start_chunk_size, start_body - start_chunk_size);
			chunk_size = std::strtol(head_chunk.c_str(), nullptr, 16);
			tmp += _body.substr(start_body + 2, chunk_size);
//			if (chunk_size != end_body - start_body && chunk_size != 0)
//				throw HTTPBadRequest();
			start_chunk_size += chunk_size + 2 + head_chunk.length() + 2;
		}
		_body = tmp;
	}

	/*
	 * Функция определяет текущий локейшен  сервере, проверяет наличие max_client_buffer_size, сверяет с длиной body текущего request`а
	 * Если body больше буффера, то возвращает ошибку 413;
	 */
	LocationData const *currentLocation = AResponder::getCurrentLocation(serverData.getLocationData(), _location, _method);
	if (currentLocation == nullptr)
		throw HTTPNotFound();
	else if (currentLocation->getClientBufferBodySize() != 0 && currentLocation->getClientBufferBodySize() < _body.length())
	{
		_error = "413";
	}
}

void Request::parse(t_req_data &buffer, ServerData const &serverData)
{
	_request = buffer.buffer;
	try
	{
		parse_request(buffer);
		parse_headers(buffer);
		parse_body(buffer, serverData);
	}
	catch (HTTPError &e)
	{
		_error = e.what();
	}
}

std::string Request::respond(ServerData const &serverData)
{
	if (!_error.empty())
		return (Response().error(_error));

	AResponder *responder;

	if (_method == "GET")
		responder = new GetResponder();
	else if (_method == "POST")
		responder = new PostResponder();
	else if (_method == "PUT")
		responder = new PutResponder();
	else if (_method == "DELETE")
		responder = new DeleteResponder();
	else
		return (Response().error("405"));
	std::string response = responder->respond(*this, serverData, _response);
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

const Response & Request::getResponse() const
{
	return (_response);
}
