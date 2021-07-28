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

	LocationData const &locationData = serverData.getLocationData()[0];

	std::string uri = locationData.getRoot() + request.getStartLine().find("location")->second;
	while (!uri.empty() && uri[0] == ' ')
		uri.erase(uri.begin());

//	const std::vector<LocationData> &locationData = serverData.getLocationData();
//	std::string requestLocation = request.getStartLine().find("location")->second;
//	for (std::vector<LocationData>::const_reverse_iterator it = locationData.rbegin(); it != locationData.rend(); ++it)
//	{
//		std::string locationPath = it->getLocationPath();
//		if (locationPath.compare(0, locationPath.length() - 1, requestLocation) == 0)
//		{
//			uri = it->getRoot() + it->getFullPath();
//			break;
//		}
//	}

	if (uri.empty())
	{
		response.setStatus("404", "Not Found");
		response.setBody("404, oops");
		return (response.str());
	}

	if (request.getHeaders().find("Referer") != request.getHeaders().end())
	{
		std::string str = request.getHeaders().find("Referer")->second;
		if (str.length() != 0 && str[str.length() - 1] != '/')
		{
			size_t n;
			n = str.find("//");
			if (n == std::string::npos)
			{
				// bad request
			}
			std::string::iterator it = str.begin();
			std::advance(it, n + 2);
			str.erase(str.begin(), it);
			n = str.find('/');
			if (n == std::string::npos)
			{
				// bad request
			}
			it = str.begin();
			std::advance(it, n);
			str.erase(str.begin(), it);
			uri += str;
		}
	}

	struct stat s;
	if (stat(uri.c_str(), &s) < 0)
	{
		response.setStatus("404", "Not Found");
		response.setBody("404, oops");
		return (response.str());
	}
	else
		response.setStatus("200", "OK");
	if (s.st_mode & S_IFDIR)
	{
		if (uri.length() != 0 && uri[uri.length() - 1] != '/')
			uri += '/';
		std::string index = locationData.getIndex();
		while (!index.empty() && index[0] == ' ')
			index.erase(index.begin());
		uri += index;
	}
	if (stat(uri.c_str(), &s) < 0)
	{
		std::cout << "404" << std::endl;
		response.setStatus("404", "Not Found");
		response.setBody("404, oops");
		return (response.str());
	}

	std::ifstream ifs(uri, std::ifstream::in);
	std::stringstream html;
	html << ifs.rdbuf();
	ifs.close();
	std::string content = html.str();

	response.getHeaders()["Content-Type"] = "text/html";
	response.getHeaders()["Content-length"] = std::to_string(content.length());
	response.getHeaders()["Connection"] = "close";
	response.setBody(content);

	response.setStatus("200", "OK");
	response.getHeaders()["Content-Type"] = "text/html";
	return (response.str());
}
