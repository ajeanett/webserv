#include "PutResponder.hpp"

PutResponder::PutResponder()
{
}

PutResponder::PutResponder(PutResponder const &src)
{
	*this = src;
}

PutResponder::~PutResponder()
{
}

PutResponder &PutResponder::operator=(PutResponder const &src)
{
	if (this != &src)
	{
	}
	return (*this);
}

std::string PutResponder::respond(const Request &request, const ParserConfig &config, const ServerData &serverData) const
{
	Response response;

	LocationData const *currentLocation = nullptr;

	for (std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin(); it != request.getHeaders().end(); ++it)
	{
		std::cout << std::setw(30) << it->first << ": " << it->second << std::endl;
	}
	std::cout << "'" << request.getBody() << "'" << std::endl;

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

//	response.getHeaders()["Content-length"] = std::to_string(content.length());
//	response.getHeaders()["Connection"] = "keep-alive";
//	response.setBody(content);

	return (response.str());
}