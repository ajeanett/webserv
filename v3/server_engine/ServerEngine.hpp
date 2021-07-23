#ifndef SERVERENGINE_HPP
#define SERVERENGINE_HPP

#include "Webserv_main.hpp"
#include "Request.hpp"


class ServerEngine{
    private:
        // std::map<long, std::string>	_requests;
	    int                         _fd;
	    struct sockaddr_in			_addr;
	    std::set<int>               _ports; // в дальнейшем изменить на map серверов(создать класс для сервера)
        // класс для каждого fd/socket создать или добавить этот класс в класс сервера 
        std::set<int>               _listen_fds;//listen fds
        
        std::string                 _startPage;
        fd_set                      _readset; // нужны четыре сета, два читающих, два слушающих, для select
        fd_set                      _writeset; // нужны четыре сета, два читающих, два слушающих, для select
        std::set<int>               _clients_recv; // для итератора чтения
        std::set<int>               _clients_send; // для итератора отправки
        fd_set                      _readset_master; // нужны четыре сета, два читающих, два слушающих, для select
        fd_set                      _writeset_master; // нужны четыре сета, два читающих, два слушающих, для select
        char                        _buf[65536]; // максимальный размер пакета http
        // bool                        _sel; // 

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
        bool    ft_send(void);
        bool    ft_receive(void);
        bool    ft_accept(int *mx);

};

#endif
