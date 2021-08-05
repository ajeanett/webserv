#include "Response.hpp"

Response::Response() : _version("HTTP/1.1")
{
	_headers["Server"] = "webserv";
}

Response::Response(Response const &src)
{
	*this = src;
}

Response::~Response()
{
}

Response &Response::operator = (Response const &src)
{
	if (this != &src)
	{
	}
	return (*this);
}

std::string Response::str() const
{
	std::string msg;

	msg += _version + ' ' + _statusCode + ' ' + _statusMessage + "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		msg += it->first + ": " + it->second + "\r\n";
	msg += "\r\n";
	msg += _body;
	return (msg);
}

std::string Response::error(const std::string &code, const std::string &message)
{
	_statusCode = code;
	_statusMessage = message;
	_body = "<!DOCTYPE html>\n"
			"<html lang=\"en\">\n"
			"<head>\n"
			"\t<meta charset=\"UTF-8\">\n"
			"\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
			"\t<title>Test Page</title>\n"
			"\t<link rel=\"stylesheet\" href=\"/styles.css\">\n"
			"</head>\n"
			"\n"
			"<body>\n"
			"\n"
			"<h1 style=\"color: #d44b25;\">" + code + ' ' + message + "</h1>\n"
			"<div class=\"back-img\" style=\"background-image: url('/img/" + code + ".jpg'); width: 500px;\"></div>\n"
			"<a class=\"go-home\" href=\"/\">\n"
			"\t← Home\n"
			"</a>\n"
			"\n"
			"</body>\n"
			"</html>";
//	_headers["Connection"] = "close";
	_headers["Content-Length"] = std::to_string(_body.length());
	return (str());
}

std::string Response::error(HTTPError const &error)
{
	return (this->error(error.what(), HTTPError::getMessage(_statusCode)));
}

std::string Response::error(const std::string &code)
{
	return (error(code, HTTPError::getMessage(code)));
}

const std::string &Response::getStatusCode() const
{
	return _statusCode;
}

void Response::setVersion(const std::string &version)
{
	_version = version;
}

void Response::setStatus(const std::string &statusCode, const std::string &statusMessage)
{
	_statusCode = statusCode;
	_statusMessage = statusMessage;
}

void Response::setBody(const std::string &body)
{
	_body = body;
}

std::map<std::string, std::string> &Response::getHeaders()
{
	return (_headers);
}

const std::map<std::string, std::string> &Response::getHeaders() const
{
	return (_headers);
}
