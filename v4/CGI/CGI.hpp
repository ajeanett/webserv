#ifndef _CGI_HPP_
# define _CGI_HPP_

#include <iostream>
#include <map>
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
    int                                 _saveStdIn = 0;
    int                                 _saveStdOut = 1;
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