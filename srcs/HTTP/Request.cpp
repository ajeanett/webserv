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

int	Request::parse_request(ServerData const &data)
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


	findPosition = _request.find("\r\n\r\n", _requestPosition);

	if (findPosition != std::string::npos)
	{
		_currentLine = _request.substr(_requestPosition, findPosition - _requestPosition);
		_requestPosition = findPosition + 4;
	}
	else
	{
		perror("ERROR. Invalid message.");
		_error = "400";
		return 400;
	}
	/*
	 * Функция определяет текущий локейшен  сервере, проверяет наличие max_client_buffer_size, сверяет с длиной body текущего request`а
	 * Если body больше буффера, то возвращает ошибку 413;
	 * */
	if (_method == "POST")
	{
		LocationData const *currentLocation = nullptr;

		const std::vector<LocationData> &locations = data.getLocationData();
		for (std::vector<LocationData>::const_reverse_iterator it = locations.rbegin(); it != locations.rend(); ++it)
		{
			std::string location = it->getLocationPath();
			if (location.length() != 0 && location[0] != '/')
				location.insert(0, "/");
			if (location.compare(0, location.length(), _location, 0, location.length()) == 0)
			{
				currentLocation = &(*it);
				if (currentLocation->getClientBufferBodySize() != 0 && currentLocation->getClientBufferBodySize() < (_request.length() - _requestPosition))
				{
					_error = "413";
					return 413;
				}  // размер боди

				//	if ()
//	if (this->conf.getBodySize() != 0 && this->conf.getBodySize() < this->req.getBody().size())
//		throw BaseException("Payload Too Large", 413);
				break;
			}
		}
		if (currentLocation == nullptr)
		{
			_error = "404";
			return 404;
		}
	}
	return 0;
}

void Request::parse_headers()
{
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

	size_t	start_chunk_size = 0;
	size_t	start_body = 0;
	size_t	end_body = 0;
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
}

void Request::parse(const std::string &request_str, ServerData const &data)
{
	_request = request_str;
	try
	{
		parse_request(data);
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
		if (_error == "413")
			return (Response().error(_error, "Payload Too Large"));
		if (_error == "500")
			return (Response().error(_error, "Internal Server Error"));
	}

	AResponder *responder;

	if (_method == "GET")
		responder = new GetResponder();
	else if (_method == "POST")
		responder = new PostResponder();
	else if (_method == "PUT")
		responder = new PutResponder();
//	else if (_startLine.find("method")->second == "DELETE")
//		responder = new DeleteResponder();
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
