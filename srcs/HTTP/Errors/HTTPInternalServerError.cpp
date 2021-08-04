#include "HTTPInternalServerError.hpp"

const char *HTTPInternalServerError::what() const throw()
{
	return ("500");
}
