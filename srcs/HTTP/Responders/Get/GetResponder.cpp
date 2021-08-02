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

std::string GetResponder::respond(Request const &request, ParserConfig const &config, ServerData const &serverData) const
{
	Response response;

	LocationData const *currentLocation = nullptr;

//	for (std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin(); it != request.getHeaders().end(); ++it)
//	{
//		std::cout << std::setw(30) << it->first << ": " << it->second << std::endl;
//	}
//	std::cout << "'" << request.getBody() << "'" << std::endl;

	const std::vector<LocationData> &locations = serverData.getLocationData();
	for (std::vector<LocationData>::const_reverse_iterator it = locations.rbegin(); it != locations.rend(); ++it)
	{
		std::string const &location = it->getLocationPath();
		if (location.compare(0, location.length(), request.getLocation(), 0, location.length()) == 0)
		{
			currentLocation = &(*it);
			break;
		}
	}
	if (currentLocation == nullptr)
		return (response.error("404", "Not Found"));
	std::vector<std::string> const &locationMethods = currentLocation->getMethods();
	if (!locationMethods.empty() && std::find(locationMethods.begin(), locationMethods.end(), request.getMethod()) == locationMethods.end())
		return (response.error("405", "Method Not Allowed"));
	std::string requestLocation = request.getLocation();
	requestLocation.erase(0, currentLocation->getLocationPath().length());
	std::string uri = currentLocation->getRoot() + requestLocation;

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
	response.getHeaders()["Content-length"] = std::to_string(content.length());
	response.getHeaders()["Connection"] = "keep-alive";
	response.setBody(content);

	return (response.str());
}
