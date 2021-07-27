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
    /* data */
    std::map<int, ServerData>   _servers; // массив серверов, int - номер сервера
    bool    server_open;
    bool    loc_open;
    int     serv_num;
    void    check_brackets(std::string buffer);
    void    fillServerData(ServerData &s, std::string &buffer);

public:
    ParserConfig(std::string &configfile);
    ParserConfig(ParserConfig &cpy);
    ~ParserConfig();
    ParserConfig	&operator=(const ParserConfig &cpy);
    void    Parser(std::string name);
    ServerData ParseServer(std::ifstream &configfile, std::string &buffer);
    std::map<int, ServerData> &getServers();
    // std::map<std::string, ConfigLocation>& ConfigServer::getLocations() {return _locations;};
};







#endif