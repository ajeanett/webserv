#ifndef _CGI_HPP_
# define _CGI_HPP_

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cstdio>
#include "../HTTP/Request.hpp"
#include "../parse_config/ParserConfig.hpp"

//попробовать запустить python3 igra.py из сервера
// /usr/local/bin/python3 igra.py - путь для запуска в execve

class CGI
{
private:
    /* data */
    std::map<std::string, std::string> _tmpEnvCGI;  // мапа для занесения пар ключ\значение для переменных окружения CGI, перед выполнением execve нужно будет перевести в char**
    char                                **_envp; // переменные окружения для execve;
    std::string                         _current_root;
    void    prepareEnvCGI(const Request &req, ServerData & serv, char *** envp);
    int     startCGI(char **envp);
    int     clearCGI();
    void    fillTmpEnvCgi(const Request &req, ServerData & serv);
    void    fillEnvp(char *** envp);
    void    startCGI(char *** envp);

public:
    CGI(/* args */);
    CGI(Request &req);
    std::string runCGI(const Request &req, ServerData & serv);
    ~CGI();

};




#endif

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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