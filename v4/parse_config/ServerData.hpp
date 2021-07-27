#ifndef SERVERDATA_HPP
#define SERVERDATA_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "LocationData.hpp"

// данные содержащиеся в сервере из конфиг файла (серверов может и должно быть несколько)

class ServerData
{
private:
    std::string     _server_name;
    int             _port;
    std::string     _host; //нужно будет перевести в int возможно, если неиспользовать IFADDR_ANY
    std::vector<LocationData>    _loc; // массив локейшенов данного сервера со всеми данными
    // std::map<int, LocationData>    _loc; // массив локейшенов данного сервера со всеми данными

    /* data */
public:
    ServerData(/* args */);
    ServerData(const ServerData &cpy);
    ServerData  &operator=(const ServerData &cpy);
    ~ServerData();

    std::string                 &getServerName();
    int                         getPort();
    std::string                 getHost();
    std::vector<LocationData>   &getLocationData();
    void                        setServerName(std::string name);
    void                        setPort(int port);
    void                        setHost(std::string host);
};

#endif