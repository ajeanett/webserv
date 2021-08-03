#include "AResponder.hpp"

AResponder::AResponder()
{
}

AResponder::AResponder(AResponder const &src)
{
	*this = src;
}

AResponder::~AResponder()
{
}

AResponder &AResponder::operator=(AResponder const &src)
{
	if (this != &src)
	{
	}
	return (*this);
}

static bool	match(std::string const &s1, std::string const &s2)
{
	if (s1[0] == '\0' && s2[0] == '\0')
		return (true);
	else if (s1[0] == s2[0] && s2[0] != '*')
		return (match(s1.substr(1, s1.length() - 1), s2.substr(1, s2.length() - 1)));
	else if (s1[0] == '\0' && s2[0] == '*')
		return (match(s1, s2.substr(1, s2.length() - 1)));
	else if (s1[0] != '\0' && s2[0] == '*')
		return (match(s1.substr(1, s1.length() - 1), s2) || match(s1, s2.substr(1, s2.length() - 1)));
	else
		return (false);
}

const LocationData *AResponder::getCurrentLocation(std::vector<LocationData> const &locations, std::string const &requestLocation, std::string const &method)
{
	std::list<LocationData const *> candidates;
	std::vector<LocationData const *> cpy;
	cpy.reserve(locations.size());
	for (std::vector<LocationData>::const_iterator it = locations.begin(); it != locations.end(); ++it)
		cpy.push_back(&(*it));
	for (size_t i = 0; i < cpy.size(); ++i)
	{
		for (size_t j = i + 1; j < cpy.size(); ++j)
		{
			if (cpy[i]->getLocationPath().length() < cpy[j]->getLocationPath().length())
				std::swap(cpy[i], cpy[j]);
		}
	}
	for (std::vector<LocationData const *>::const_iterator it = cpy.begin(); it != cpy.end(); ++it)
	{
		std::string location = (*it)->getLocationPath();
		if (location.find('*') != std::string::npos && match(requestLocation, location))
		{
			if ((*it)->getMethods().empty() || std::find((*it)->getMethods().begin(), (*it)->getMethods().end(), method) != (*it)->getMethods().end())
				return (*it);
			candidates.push_front(*it);
		}
		if (location == requestLocation)
			return (*it);
		size_t l = std::min(location.length(), requestLocation.length());
		if (location.compare(0, l, requestLocation, 0, l) == 0 && location.length() <= requestLocation.length())
		{
			if ((*it)->getMethods().empty() || std::find((*it)->getMethods().begin(), (*it)->getMethods().end(), method) != (*it)->getMethods().end())
				return (*it);
			candidates.push_front(*it);
		}
	}
	if (!candidates.empty())
		return (candidates.front());
	return (nullptr);
}
