#include "ServerData.hpp"

ServerData::ServerData(/* args */)
{
}

ServerData::ServerData(const ServerData &cpy)
{
    *this = cpy;
}

ServerData::~ServerData()
{
}

ServerData  &ServerData::operator=(const ServerData &cpy){
    this->_server_name = cpy._server_name;
    this->_port = cpy._port;
    this->_host = cpy._host;
    this->_loc = cpy._loc;
    // this->_loc.clear();
    // ////////////////начало кода с ошибкой
    // std::vector<LocationData>::iterator it = cpy._loc.begin();
    // for (it; it != cpy._loc.end(); ++it)
    // {
    //     this->_loc.push_back((*it));
    // }
    return *this;
}

std::string                 &ServerData::getServerName(){
    return (this->_server_name);
}

int                         ServerData::getPort(){
    return (this->_port);
}

std::string                 ServerData::getHost(){
    return (this->_host);
}

std::vector<LocationData>   &ServerData::getLocationData(){
    return (this->_loc);
}

void                        ServerData::setServerName(std::string name){
    this->_server_name = name;
}

void                        ServerData::setPort(int port){
    this->_port = port;

}

void                        ServerData::setHost(std::string host){
    this->_host = host;
}
