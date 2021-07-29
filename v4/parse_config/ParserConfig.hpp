#pragma once
#ifndef PARSERCONFIG_HPP
#define PARSERCONFIG_HPP

#include <iostream>
#include <map>
#include "ServerData.hpp"
#include <fstream>


class ParserConfig
{
private:

	/* Ключ - значение (номер сервера) -> Класс{name, port, host, массив путей } */
    std::map<int, ServerData>   _servers;
    
	/* server open переменная для парсера*/
	bool    server_open;

	/* открыт ли loc */
    bool    loc_open;

	/* Количсетво серверов */
    int     serv_num;
	
    void    check_brackets(std::string buffer);
    void    fillServerData(ServerData &s, std::string &buffer);

public:
    ParserConfig();
    ParserConfig(ParserConfig &cpy);
    ~ParserConfig();
    ParserConfig	&operator = (const ParserConfig &cpy);
    void    Parser(std::string name);
    ServerData ParseServer(std::ifstream &configfile, std::string &buffer);
	std::map<int, ServerData> &getServers();
};

#endif