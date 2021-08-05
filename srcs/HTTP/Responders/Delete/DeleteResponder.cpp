#include "DeleteResponder.hpp"
#include "HTTP/Response.hpp"
#include "HTTP/Request.hpp"

DeleteResponder::DeleteResponder()
{
}

DeleteResponder::DeleteResponder(DeleteResponder const &src)
{
	*this = src;
}

DeleteResponder::~DeleteResponder()
{
}

DeleteResponder &DeleteResponder::operator=(DeleteResponder const &src)
{
	if (this != &src)
	{
	}
	return (*this);
}

std::string DeleteResponder::respond(const Request &request, const ServerData &serverData, Response &response) const
{
	LocationData const *currentLocation = AResponder::getCurrentLocation(serverData.getLocationData(), request.getLocation(), "DELETE");

//	for (std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin(); it != request.getHeaders().end(); ++it)
//	{
//		std::cout << std::setw(30) << it->first << ": " << it->second << std::endl;
//	}
//	std::cout << "'" << request.getBody() << "'" << std::endl;

	response.getHeaders()["Connection"] = "close";
	response.setStatus("200", "OK");
	if (currentLocation == nullptr)
		return (response.error("404", "Not Found"));
	std::vector<std::string> const &locationMethods = currentLocation->getMethods();
	if (!locationMethods.empty() && std::find(locationMethods.begin(), locationMethods.end(), request.getMethod()) == locationMethods.end())
		return (response.error("405", "Method Not Allowed"));
	std::string uri = currentLocation->getRoot() + request.getLocation().substr(currentLocation->getLocationPath().length());
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
	if (s.st_mode & S_IFDIR || stat(uri.c_str(), &s) < 0)
		content = "{success: false}";
	else
	{
		if (remove(uri.c_str()) < 0)
			content = "{success: false}";
		else
			content = "{success: true}";
	}

	response.getHeaders()["Content-Type"] = "application/json";
	response.getHeaders()["Content-Length"] = std::to_string(content.length());
	response.setBody(content);

	return (response.str());
}
