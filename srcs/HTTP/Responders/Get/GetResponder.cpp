#include <sys/stat.h>
#include "GetResponder.hpp"

GetResponder::GetResponder()
{
}

GetResponder::GetResponder(GetResponder const &src)
{
	*this = src;
}

GetResponder::~GetResponder()
{
}

GetResponder &GetResponder::operator=(GetResponder const &src)
{
	if (this != &src)
	{
	}
	return (*this);
}

std::string GetResponder::respond(Request const &request, ServerData const &serverData, Response &response) const
{
	LocationData const *currentLocation = AResponder::getCurrentLocation(serverData.getLocationData(), request.getLocation(), "GET");

//	for (std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin(); it != request.getHeaders().end(); ++it)
//	{
//		std::cout << std::setw(30) << it->first << ": " << it->second << std::endl;
//	}
//	std::cout << "'" << request.getBody() << "'" << std::endl;

	response.getHeaders()["Connection"] = "close";
	if (currentLocation == nullptr)
		return (response.error("404", "Not Found"));
	if (!currentLocation->getRedirect().empty())
	{
		response.setStatus("301", "Moved Permanently");
		response.getHeaders()["Location"] = currentLocation->getRedirect();//"https://www.google.com/";
		std::cout << "currentLocation->getRedirect()" << currentLocation->getRedirect() << std::endl;
		return (response.str());
	}
	std::string uri = currentLocation->getRoot() + request.getLocation().substr(currentLocation->getLocationPath().length());

	response.getHeaders()["Content-Type"] = "text/html";
	size_t n = uri.find_last_of('.');
	if (n != std::string::npos)
	{
		std::string type;
		std::string extension;
		extension = uri.substr(n + 1, uri.length() - n);
		type = "text";
		if (extension == "jpg" || extension == "jpeg" || extension == "png")
			type = "image";
		if (extension != "html" && extension != "css" && extension != "jpg" && extension != "jpeg" && extension != "png")
			extension = "html";
		response.getHeaders()["Content-Type"] = type + '/' + extension;
	}

	if (currentLocation->getIndex().empty() && currentLocation->getAutoindex() && request.getLocation()[request.getLocation().length() - 1] == '/')
	{
		Autoindex autoIndex(uri);
		std::string content = autoIndex.get_html();
		response.setStatus("200", "OK");
//		response.getHeaders()["Content-Type"] = "text/html";
		response.getHeaders()["Content-Length"] = std::to_string(content.length());
		response.setBody(content);
		return (response.str());
	}
	std::vector<std::string> const &locationMethods = currentLocation->getMethods();
	if (!locationMethods.empty() && std::find(locationMethods.begin(), locationMethods.end(), request.getMethod()) == locationMethods.end())
		return (response.error("405", "Method Not Allowed"));
	if (uri.empty())
		return (response.error("404", "Not Found"));

//	if (request.getHeaders().find("Referer") != request.getHeaders().end())
//	{
//		std::string str = request.getHeaders().find("Referer")->second;
//		if (str.length() != 0 && str[str.length() - 1] != '/')
//		{
//			size_t n;
//			n = str.find("//");
//			if (n == std::string::npos)
//				return (response.error("400", "Bad Request"));
//			std::string::iterator it = str.begin();
//			std::advance(it, n + 2);
//			str.erase(str.begin(), it);
//			n = str.find('/');
//			if (n == std::string::npos)
//				return (response.error("400", "Bad Request"));
//			it = str.begin();
//			std::advance(it, n);
//			str.erase(str.begin(), it);
//			uri += str;
//		}
//	}

	std::string content;
	struct stat s;
	if (stat(uri.c_str(), &s) < 0)
		return (response.error("404", "Not Found"));
	else
		response.setStatus("200", "OK");
	if (!currentLocation->getCgiExtension().empty())
	{
		CGI cgi(request, serverData, currentLocation->getCgiPath(), currentLocation->getCgiExtension());
		cgi.runCGI();
		content = cgi.getBody();
	}
	else
	{
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
	}

	response.getHeaders()["Content-Length"] = std::to_string(content.length());
	if (request.getHeaders().find("Connection") == request.getHeaders().end())
		response.getHeaders()["Connection"] = "keep-alive";
	else
		response.getHeaders()["Connection"] = request.getHeaders().find("Connection")->second;
	response.setBody(content);

	return (response.str());
}
