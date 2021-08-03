#include "PostResponder.hpp"
#include "HTTP/Response.hpp"
#include "HTTP/Request.hpp"

PostResponder::PostResponder() : AResponder()
{
}

PostResponder::PostResponder(PostResponder const &src) : AResponder()
{
	*this = src;
}

PostResponder::~PostResponder()
{
}

PostResponder &PostResponder::operator = (PostResponder const &src)
{
	if (this != &src)
	{
	}
	return (*this);
}

std::string PostResponder::respond(const Request &request, const ParserConfig &config, const ServerData &serverData) const
{
	Response response;
	std::map<std::string, std::string> &responseHeaders = response.getHeaders();

//	for (std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin(); it != request.getHeaders().end(); ++it)
//	{
//		std::cout << std::setw(30) << it->first << ": " << it->second << std::endl;
//	}
//	std::cout << "'" << request.getBody() << "'" << std::endl;

	LocationData const *currentLocation = AResponder::getCurrentLocation(serverData.getLocationData(), request.getLocation(), "POST");

	if (currentLocation == nullptr)
		return (response.error("404", "Not Found"));
	std::vector<std::string> const &locationMethods = currentLocation->getMethods();
	if (!locationMethods.empty() && std::find(locationMethods.begin(), locationMethods.end(), request.getMethod()) == locationMethods.end())
		return (response.error("405", "Method Not Allowed"));

	response.setStatus("200", "OK");
	std::string content;
	if (!currentLocation->getCgiPath().empty())
	{
		CGI cgi(request, serverData, currentLocation->getCgiPath(), currentLocation->getCgiExtension());
		cgi.runCGI();
		content = cgi.getBody();
		responseHeaders["Content-Length"] = std::to_string(content.length());
		if (cgi.getHeaders().find("Status") != cgi.getHeaders().end())
		{
			std::string status = cgi.getHeaders().find("Status")->second;
			std::string code = status.substr(0, 3);
			status.erase(0, 4);
			response.setStatus(code, status);
		}
		for (std::map<std::string, std::string>::const_iterator it = cgi.getHeaders().begin(); it != cgi.getHeaders().end(); ++it)
			responseHeaders[it->first] = it->second;
	}
	else
	{
		std::string uri = currentLocation->getRoot() + request.getLocation();
		if (uri.empty())
			return (response.error("404", "Not Found"));
		struct stat s;
		if (stat(uri.c_str(), &s) < 0)
			return (response.error("404", "Not Found"));
		else
			response.setStatus("200", "OK");
		if (s.st_mode & S_IFDIR)
		{
			if (uri.length() != 0 && uri[uri.length() - 1] != '/')
				uri += '/';
			uri += currentLocation->getIndex();
		}
		if (stat(uri.c_str(), &s) < 0)
			return (response.error("404", "Not Found"));

		std::ifstream ifs(uri, std::ifstream::in);
		std::stringstream content_stream;
		content_stream << ifs.rdbuf();
		ifs.close();
		content = content_stream.str();
		responseHeaders["Content-Length"] = std::to_string(content.length());
	}

//	response.getHeaders()["Content-Type"] = "text/plain";
//	size_t n = uri.find_last_of('.');
//	if (n != std::string::npos)
//	{
//		std::string type;
//		std::string extension;
//		type = "text";
//		extension = uri.substr(n + 1, uri.length() - n);
//		response.getHeaders()["Content-Type"] = type + '/' + extension;
//		std::cout << "result: " << response.getHeaders()["Content-Type"] << std::endl;
//	}

	responseHeaders["Connection"] = "close";
	response.setBody(content);

	return (response.str());
}
