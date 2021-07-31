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
	std::ifstream ifs("./html_pages/" + code + ".html", std::ifstream::in);
	std::stringstream html;
	html << ifs.rdbuf();
	ifs.close();
	_body = html.str();
	return (str());
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
//	_headers["Content-Length"] = std::to_string(body.length());
	_body = body;
}

std::map<std::string, std::string> &Response::getHeaders()
{
	return (_headers);
}
