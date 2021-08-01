#ifndef _CGI_HPP_
# define _CGI_HPP_

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cstdio>
#include <sys/stat.h>
#include "../HTTP/Request.hpp"
#include "../parse_config/ParserConfig.hpp"
#include "../server_engine/Date.hpp"

//попробовать запустить python3 igra.py из сервера
// /usr/local/bin/python3 igra.py - путь для запуска в execve

class CGI
{
private:
    /* data */
    std::map<std::string, std::string> _tmpEnvCGI;  // мапа для занесения пар ключ\значение для переменных окружения CGI, перед выполнением execve нужно будет перевести в char**
    char                                **_envp; // переменные окружения для execve;
    std::string                         _current_root;
	const Request								&_req;
	const ServerData							&_serv;
    const std::string							&_cgi_type;
    const std::string 							&_cgi_path;
	std::string 								_headers_all;
	Date										d;


    void    prepareEnvCGI(const Request &req, const ServerData & serv, char *** envp);
    int     clearCGI();
    void    fillTmpEnvCgi(const Request &req, const ServerData & serv);
    void    fillEnvp(char *** envp);
    void    startCGI(char *** envp);
    std::map<std::string, std::string> _headers;
    std::string _body;

public:
    CGI(const Request &req, const ServerData & serv, const std::string &cgi_real_path, const std::string &cgi_type);
    void runCGI();
    ~CGI();

    std::map<std::string, std::string> const &getHeaders() const;
    std::string const &getBody() const;

};




#endif

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

//using namespace std;

// int main()
// {
//     ofstream ofs("test.txt");
//     ofs << "Writing to a basic_ofstream object..." << endl;
//     ofs.close();

//     int posix_handle = ::_fileno(::fopen("test.txt", "r"));

//     ifstream ifs(::_fdopen(posix_handle, "r")); // 1

//     string line;
//     getline(ifs, line);
//     ifs.close();
//     cout << "line: " << line << endl;
//     return 0;
// }