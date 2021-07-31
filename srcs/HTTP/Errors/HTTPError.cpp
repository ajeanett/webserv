#include "HTTPError.hpp"

const char *HTTPError::what() const throw()
{
	return ("Some HTTP error");
}
