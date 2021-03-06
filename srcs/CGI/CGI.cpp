#include "CGI.hpp"

CGI::CGI(const Request &req, const ServerData &serv,
		 const std::string &cgi_path, const std::string &cgi_type) :
		_req(req),
		_serv(serv),
		_cgi_type(cgi_type),
		_cgi_path(cgi_path)
{
	/* environment variables for CGI */
	_tmpEnvCGI["SERVER_SOFTWARE"] = "Webserv by kfriese and ajeanett";
	_tmpEnvCGI["SERVER_NAME"];
	_tmpEnvCGI["GATEWAY_INTERFACE"];
	_tmpEnvCGI["SERVER_PROTOCOL"];
	_tmpEnvCGI["SERVER_PORT"];
	_tmpEnvCGI["REQUEST_METHOD"];
	_tmpEnvCGI["PATH_INFO"];
	_tmpEnvCGI["PATH_TRANSLATED"];
	_tmpEnvCGI["SCRIPT_NAME"];
	_tmpEnvCGI["QUERY_STRING"];
	_tmpEnvCGI["REMOTE_HOST"];
	_tmpEnvCGI["REMOTE_ADDR"];
	_tmpEnvCGI["AUTH_TYPE"];
	_tmpEnvCGI["REMOTE_USER"];
	_tmpEnvCGI["REMOTE_IDENT"];
	_tmpEnvCGI["CONTENT_TYPE"];
	_tmpEnvCGI["CONTENT_LENGTH"];
	_tmpEnvCGI["HTTP_ACCEPT"];
	_tmpEnvCGI["HTTP_USER_AGENT"];
	_current_root = "";
}

CGI::~CGI()
{
}

int CGI::clear()
{
	_tmpEnvCGI["SERVER_NAME"].clear();
	_tmpEnvCGI["GATEWAY_INTERFACE"].clear();
	_tmpEnvCGI["SERVER_PROTOCOL"].clear();
	_tmpEnvCGI["SERVER_PORT"].clear();
	_tmpEnvCGI["REQUEST_METHOD"].clear();
	_tmpEnvCGI["PATH_INFO"].clear();
	_tmpEnvCGI["PATH_TRANSLATED"].clear();
	_tmpEnvCGI["SCRIPT_NAME"].clear();
	_tmpEnvCGI["QUERY_STRING"].clear();
	_tmpEnvCGI["REMOTE_HOST"].clear();
	_tmpEnvCGI["REMOTE_ADDR"].clear();
	_tmpEnvCGI["AUTH_TYPE"].clear();
	_tmpEnvCGI["REMOTE_USER"].clear();
	_tmpEnvCGI["REMOTE_IDENT"].clear();
	_tmpEnvCGI["CONTENT_TYPE"].clear();
	_tmpEnvCGI["CONTENT_LENGTH"].clear();
	_tmpEnvCGI["HTTP_ACCEPT"].clear();
	_tmpEnvCGI["HTTP_USER_AGENT"].clear();
	_current_root.clear();
	return (0);
}

static std::string transformXHeader(std::string const &str)
{
	std::string r;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == '-')
			r += '_';
		else
			r += (char)std::toupper(str[i]);
	}
	return (r);
}

void CGI::fillTmpEnvCgi(const Request &req, const ServerData &serv)
{

	std::string curr_loc_str;
	std::vector<LocationData> locs;

	curr_loc_str = req.getLocation();
	locs = serv.getLocationData();

	_tmpEnvCGI["SERVER_NAME"] = serv.getServerName();
//    _tmpEnvCGI["HTTP_X_SECRET_HEADER_FOR_TEST"] = "1";
	_tmpEnvCGI["GATEWAY_INTERFACE"] = "CGI/1.1";
	_tmpEnvCGI["SERVER_PROTOCOL"] = req.getVersion();
	_tmpEnvCGI["SERVER_PORT"] = std::to_string(serv.getPort());
	_tmpEnvCGI["REQUEST_METHOD"] = req.getMethod();
	_tmpEnvCGI["PATH_INFO"] = req.getLocation(); // target
	for (std::vector<LocationData>::iterator it = locs.begin();
		 it != locs.end(); ++it)
	{
		if (it->getLocationPath() == curr_loc_str)
		{
			_tmpEnvCGI["PATH_TRANSLATED"] = (*it).getRoot(); //
			_current_root = (*it).getRoot();
			_tmpEnvCGI["SCRIPT_NAME"] = !(*it).getCgiPath().empty()
										? (*it).getCgiPath() : "";
		}
	}
	_tmpEnvCGI["QUERY_STRING"] = "";
//        _tmpEnvCGI["REMOTE_HOST"] = req.getHeaders().find("Referer") == req.getHeaders().end() ? "" : req.getHeaders().find("Content-Type")->second;
	_tmpEnvCGI["REMOTE_ADDR"] = "";
	_tmpEnvCGI["AUTH_TYPE"] = "BASIC";
	_tmpEnvCGI["REMOTE_USER"] = "User";
	// _tmpEnvCGI["REMOTE_IDENT"] = "";
	_tmpEnvCGI["CONTENT_TYPE"] =
			req.getHeaders().find("Content-Type") == req.getHeaders().end() ? ""
																			: req.getHeaders().find("Content-Type")->second;
	_tmpEnvCGI["CONTENT_LENGTH"] = std::to_string(req.getBody().size());
	_tmpEnvCGI["HTTP_ACCEPT"] =
			req.getHeaders().find("Accept") == req.getHeaders().end() ? ""
																	  : req.getHeaders().find("Accept")->second;
	_tmpEnvCGI["HTTP_USER_AGENT"] =
			req.getHeaders().find("User-Agent") == req.getHeaders().end() ? ""
																		  : req.getHeaders().find("User-Agent")->second;

	/* X- Headers */
	for (std::map<std::string, std::string>::const_iterator it = _req.getHeaders().begin(); it != _req.getHeaders().end(); ++it)
	{
		if (it->first.compare(0, 2, "X-", 0, 2) == 0)
			_tmpEnvCGI["HTTP_" + transformXHeader(it->first)] = it->second;
	}
}

void CGI::fillEnvp(char ***envp)
{
	*envp = new char *[_tmpEnvCGI.size() + 1];
	int i = 0;
	//???????????????? ???????????????????? ?????????????????? ?? char** ???? ????????, ?????????????? ???? ?????????????? ??????????
	for (std::map<std::string, std::string>::iterator it = _tmpEnvCGI.begin(); it != _tmpEnvCGI.end(); ++it)
	{
		std::string var = it->first + "=" + it->second; //???????????? ???????????? ?? ???????????????????? ?????????????????? ???????????? ???????? ???????? key=value
		(*envp)[i] = new char[var.size() + 1];
		std::strcpy((*envp)[i], var.c_str());
		i++;
	}
	(*envp)[i] = nullptr; //???????????????? ???????????????????? ????????????
}

void CGI::prepareEnvCGI(const Request &req, const ServerData &serv, char ***envp)
{
	clear(); //?????????????? ???????????????????? ???????????? ?????????? ?????????????????? ????????????????????????????;
	fillTmpEnvCgi(req, serv);
	fillEnvp(envp);
}

void CGI::runCGI()
{
	char **envp = nullptr;
	std::string _ret; // ???????????????????????? ????????????;
	prepareEnvCGI(_req, _serv, &envp);

	pid_t _pid;
	int _status;

	FILE *tmp_file_out = tmpfile();
	int tmp_file_fd_out = fileno(tmp_file_out);

	FILE *tmp_file_in = tmpfile();
	int tmp_file_fd_in = fileno(tmp_file_in);
	ssize_t i = 1;
	size_t cursor = 0;
	while (i > 0 && cursor != _req.getBody().length())
	{
		size_t len = 65535;
		if (len > _req.getBody().length() - cursor)
			len = _req.getBody().length() - cursor;
		i = write(tmp_file_fd_in, _req.getBody().c_str() + cursor, len);
		cursor += i;
	}
	lseek(tmp_file_fd_in, 0, SEEK_SET);

	_pid = fork();

	if (_pid == -1)
	{
		perror("Fork error\n");
	}
	else if (_pid == 0)
	{
		dup2(tmp_file_fd_in, STDIN_FILENO);
		dup2(tmp_file_fd_out, STDOUT_FILENO);
		if (execve(_cgi_path.c_str(), nullptr, envp) == -1)
		{
			std::cerr << "ERROR CGI" << std::endl;
		}
		exit(0);
	}
	else
	{
		waitpid(_pid, &_status, 0);

		lseek(tmp_file_fd_out, 0, SEEK_SET);
		i = 1;
		char buffer[65535 + 1];
		while (i > 0)
		{
			i = read(tmp_file_fd_out, buffer, 65535);
			buffer[i] = '\0';
			_ret += buffer;
		}
		fclose(tmp_file_in);
		fclose(tmp_file_out);

		// ???????? ???????????? ?? ?????????????? ???? ?? ????????
		size_t prev = 0;
		size_t next = _ret.find("\r\n\r\n", 0);

		if (next != std::string::npos)
		{
			_headers_all = _ret.substr(0, next);
			_ret.erase(0, next + 4);
		}
		else
		{
			perror("ERROR 500. Internal server error!. No headers in CGI data.");
		}

		std::vector<std::string> headers_raw;

		while ((next = _headers_all.find("\r\n", prev)) != std::string::npos)
		{
			headers_raw.push_back(_headers_all.substr(prev, next - prev));
			prev = next + 2;
		}
		headers_raw.push_back(_headers_all.substr(prev));

		//  ???????????? ?????????????? ?????? ????????????????
		// std::cout << "Vector Headers" << std::endl;
		// for (it = headers_raw.begin(); it != headers_raw.end(); it++)
		//     std::cout << *it << std::endl;
		// std::cout << "Vector Headers END" << std::endl;

		std::string delim = ": ";
		std::string header_key;
		std::string header_value;

		for (std::vector<std::string>::iterator it = headers_raw.begin(); it != headers_raw.end(); it++)
		{
			prev = 0;
			if ((next = it->find(delim, prev)) != std::string::npos)
			{
				header_key = it->substr(prev, next - prev);
				prev = next + 2;
			}
			header_value = it->substr(prev);
			_headers[header_key] = header_value;
		}
		_body = _ret;
	}
	if (envp)
	{
		delete[] envp;
		envp = nullptr;
	}
}

const std::map<std::string, std::string> &CGI::getHeaders() const
{
	return (_headers);
}

const std::string &CGI::getBody() const
{
	return (_body);
}

// Specification
// The following environment variables are not request-specific and are set for all requests:

// SERVER_SOFTWARE
// The name and version of the information server software answering the request (and running the gateway). Format: name/version

// SERVER_NAME
// The server's hostname, DNS alias, or IP address as it would appear in self-referencing URLs.

// GATEWAY_INTERFACE
// The revision of the CGI specification to which this server complies. Format: CGI/revision

// The following environment variables are specific to the request being fulfilled by the gateway program:
// SERVER_PROTOCOL
// The name and revision of the information protcol this request came in with. Format: protocol/revision

// SERVER_PORT
// The port number to which the request was sent.

// REQUEST_METHOD
// The method with which the request was made. For HTTP, this is "GET", "HEAD", "POST", etc.

// PATH_INFO
// The extra path information, as given by the client. In other words, scripts can be accessed by their virtual pathname, followed by extra information at the end of this path. The extra information is sent as PATH_INFO. This information should be decoded by the server if it comes from a URL before it is passed to the CGI script.

// PATH_TRANSLATED
// The server provides a translated version of PATH_INFO, which takes the path and does any virtual-to-physical mapping to it.

// SCRIPT_NAME
// A virtual path to the script being executed, used for self-referencing URLs.

// QUERY_STRING
// The information which follows the ? in the URL which referenced this script. This is the query information. It should not be decoded in any fashion. This variable should always be set when there is query information, regardless of command line decoding.

// REMOTE_HOST
// The hostname making the request. If the server does not have this information, it should set REMOTE_ADDR and leave this unset.

// REMOTE_ADDR
// The IP address of the remote host making the request.

// AUTH_TYPE
// If the server supports user authentication, and the script is protects, this is the protocol-specific authentication method used to validate the user.

// REMOTE_USER
// If the server supports user authentication, and the script is protected, this is the username they have authenticated as.

// REMOTE_IDENT
// If the HTTP server supports RFC 931 identification, then this variable will be set to the remote user name retrieved from the server. Usage of this variable should be limited to logging only.

// CONTENT_TYPE
// For queries which have attached information, such as HTTP POST and PUT, this is the content type of the data.

// CONTENT_LENGTH
// The length of the said content as given by the client.

// In addition to these, the header lines recieved from the client, if any, are placed into the environment with the prefix HTTP_ followed by the header name. Any - characters in the header name are changed to _ characters. The server may exclude any headers which it has already processed, such as Authorization, Content-type, and Content-length. If necessary, the server may choose to exclude any or all of these headers if including them would exceed any system environment limits.
// An example of this is the HTTP_ACCEPT variable which was defined in CGI/1.0. Another example is the header User-Agent.

// HTTP_ACCEPT
// The MIME types which the client will accept, as given by HTTP headers. Other protocols may need to get this information from elsewhere. Each item in this list should be separated by commas as per the HTTP spec.

// Format: type/subtype, type/subtype

// HTTP_USER_AGENT
// The browser the client is using to send the request. General format: software/version library/version.

// /usr/bin/cgi_scripts
