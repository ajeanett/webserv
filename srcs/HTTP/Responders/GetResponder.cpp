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

void displayTimeStamp()
{
	char buf[1024];
	struct timeval _t;
	struct tm* _tm;

	gettimeofday(&_t, NULL);
	_tm = localtime(&_t.tv_sec);
	std::strftime(buf, 1024, "%H:%M:%S", _tm);

	std::cout << '[' << buf << ']' << std::endl;
}

std::string GetResponder::respond(Request const &request, ParserConfig const &config, ServerData const &serverData) const
{
	displayTimeStamp();
	std::cout << "request: " << request.getMethod() << " " << request.getLocation() << std::endl;
	Response response;

	LocationData const *currentLocation = nullptr;

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
	std::string uri = currentLocation->getRoot() + request.getLocation();

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

	struct stat s;
	if (stat(uri.c_str(), &s) < 0)
		return (response.error("404", "Not Found"));
	else
		response.setStatus("200", "OK");
	if (s.st_mode & S_IFDIR)
	{
		if (uri.length() != 0 && uri[uri.length() - 1] != '/')
			uri += '/';
		std::string index = currentLocation->getIndex();
		while (!index.empty() && index[0] == ' ')
			index.erase(index.begin());
		uri += index;
	}
	if (stat(uri.c_str(), &s) < 0)
		return (response.error("404", "Not Found"));

	std::ifstream ifs(uri, std::ifstream::in);
	std::stringstream html;
	html << ifs.rdbuf();
	ifs.close();
	std::string content = html.str();

//	CGI cgi(nullptr, nullptr, "", "");
//	cgi.runCGI();


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

	response.setStatus("200", "OK");
	return (response.str());
}
