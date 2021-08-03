#ifndef _CGI_HPP_
# define _CGI_HPP_

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cstdio>
#include <sys/stat.h>
#include "../HTTP/Request.hpp"
#include "../Parse/ParserConfig.hpp"
#include "Utils/Date.hpp"

class CGI
{

private:
	/* data */
	std::map<std::string, std::string> _tmpEnvCGI;  // мапа для занесения пар ключ\значение для переменных окружения CGI, перед выполнением execve нужно будет перевести в char**
	std::string _current_root;
	const Request &_req;
	const ServerData &_serv;
	std::string _cgi_type;
	std::string _cgi_path;
	std::string _headers_all;
	Date _d;
	std::map<std::string, std::string> _headers;
	std::string _body;

	void prepareEnvCGI(const Request &req, const ServerData &serv, char ***envp);
	int clear();
	void fillTmpEnvCgi(const Request &req, const ServerData &serv);
	void fillEnvp(char ***envp);

public:
	CGI(const Request &req, const ServerData &serv, const std::string &cgi_path, const std::string &cgi_type);
	~CGI();

	void runCGI();

	std::map<std::string, std::string> const &getHeaders() const;
	std::string const &getBody() const;

};

#endif
