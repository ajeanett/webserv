#ifndef _CGI_HPP_
# define _CGI_HPP_

#include <iostream>
#include <map>
#include "../server_engine/Request.hpp"

//попробовать запустить python3 igra.py из сервера
// /usr/local/bin/python3 igra.py - путь для запуска в execve

class CGI
{
private:
    /* data */
    std::map<std::string, std::string> _tmpEnvCGI;  // мапа для занесения пар ключ\значение для переменных окружения CGI, перед выполнением execve нужно будет перевести в char**

public:
    CGI(/* args */);
    CGI(Request req);
    int runCGI();
    ~CGI();

};




#endif