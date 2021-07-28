#pragma once
#ifndef SERVERENGINE_HPP
#define SERVERENGINE_HPP

#include "Webserv_main.hpp"
#include "Request.hpp"
#include "../parse_config/ParserConfig.hpp"

#define TCP_MAX 65535


class ServerEngine{
    private:
        // std::map<long, std::string>	_requests;
	    int                         _fd;
	    struct sockaddr_in			_addr;
	    std::set<int>               _ports; //  сет портов, которые будем слушать
        std::set<int>               _listen_fds;//listen fds, сет fd\сокетов, которые будем слушать, каждый из них связан с портом
        
        std::string                 _startPage; //стартовая страница, index.html
        
        std::set<int>               _clients_recv; // для итератора чтения
        std::set<int>               _clients_send; // для итератора отправки
        fd_set                      _readset_master; // нужны четыре сета, два читающих, два слушающих, для select
        fd_set                      _writeset_master; // нужны четыре сета, два читающих, два слушающих, для select
        fd_set                      _readset; // нужны четыре сета, два читающих, два слушающих, для select
        fd_set                      _writeset; // нужны четыре сета, два читающих, два слушающих, для select
        char                        _buf[TCP_MAX + 1]; // максимальный размер пакета http
        std::map<int, std::string>  _buffer;// мапа для чтения запросов\request`ов, int -  это фд клиента, string - буффер отдельно для каждого клиента. 
        // bool                        _sel; //
        std::map<int, bool>         _chunked; // проверка на фрагментированность каждого запроса. int -  это фд клиента, bool - фрагментирован запрос или нет.

    public:
        ServerEngine(ServerEngine const & src);
        ServerEngine &operator=(ServerEngine const & src);
        ServerEngine(std::set<int> const & ports);
        ServerEngine(void);
        virtual ~ServerEngine();
        std::set<int> getPorts() const { return (this->_ports);}
        int const & getFd() const { return (this->_fd);}
        void setFd(int const & src) {this->_fd = src; }

        int		servStart(void);
        void	getStartPage(void);
        void	run(void);
        void    setAddr(int port);
        int     ft_select(int mx, timeval timeout);
        bool    ft_send(Request const &request);
        bool    ft_receive(Request &request);
        bool    ft_accept(int *mx);
        bool    check_request(std::string buffer);

};

#endif
