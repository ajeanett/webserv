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

std::string PutResponder::respond(const Request &request, const ServerData &serverData) const
{
	Response response;

	response.getHeaders()["Connection"] = "keep-alive";
	LocationData const *currentLocation = AResponder::getCurrentLocation(serverData.getLocationData(), request.getLocation(), "PUT");

//	for (std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin(); it != request.getHeaders().end(); ++it)
//	{
//		std::cout << std::setw(30) << it->first << ": " << it->second << std::endl;
//	}
//	std::cout << "'" << request.getBody() << "'" << std::endl;

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

	response.getHeaders()["Content-Location"] = uri;

	struct stat s;
	if (stat(uri.c_str(), &s) < 0)
		response.setStatus("201", "Created");
	else
		response.setStatus("204", "No Content");
	std::ofstream ofs(uri, std::ios::trunc);
	if (!ofs.is_open())
		return (response.error("403", "Forbidden"));
	ofs << request.getBody();
	ofs.close();

//	response.getHeaders()["Connection"] = "keep-alive";
	return (response.str());
}
