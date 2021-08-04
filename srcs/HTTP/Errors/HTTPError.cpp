#include "HTTPError.hpp"

const char *HTTPError::what() const throw()
{
	return ("Some HTTP error");
}

std::string HTTPError::getMessage(const std::string &code)
{
	if (code == "400")
		return ("Bad Request");
	if (code == "404")
		return ("Not Found");
	if (code == "405")
		return ("Method Not Allowed");
	if (code == "413")
		return ("Payload Too Large");
	if (code == "500")
		return ("Internal Server Error");
	return ("Unknown");
}
