#include "HTTPBadRequest.hpp"

const char *HTTPBadRequest::what() const throw()
{
	return ("400");
}
