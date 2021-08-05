#pragma once
#ifndef SERVERENGINE_HPP
#define SERVERENGINE_HPP

#include "colors.hpp"
#include "../HTTP/Request.hpp"
#include "../HTTP/Request_data.hpp"
#include "../HTTP/Errors/HTTPError.hpp"
#include "../HTTP/Errors/HTTPNotFound.hpp"
#include "../HTTP/Errors/HTTPInternalServerError.hpp"
#include "../Parse/ParserConfig.hpp"
#include "../CGI/CGI.hpp"
#include "logging.hpp"
/* Для inet_addr */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <algorithm>

#define DEBUG 0



class ServerEngine
{

private:
	bool _ready;
	static const int _tcp_max = 65535;
	static const int _timeout = 10;
	static const int _queue = 4000;

	/* Временные переменные */
	int _fd;
	struct sockaddr_in _addr;

	/* map port - ключ, host - значение*/
	std::map<int, std::string> _ports_host;

	/*listen fds, сет fd \сокетов, которые будем слушать, каждый из них связан с портом */
	std::set<int> _listen_fds;

	/*_config Класс содержаший конфигурацию */
	ParserConfig _config;
	std::map<int, Request> request;

	std::set<int> _clients_recv; // для итератора чтения
	std::set<int> _clients_send; // для итератора отправки
	fd_set _readset_master; // нужны четыре сета, два читающих, два слушающих, для select
	fd_set _writeset_master; // нужны четыре сета, два читающих, два слушающих, для select
	fd_set _readset; // нужны четыре сета, два читающих, два слушающих, для select
	fd_set _writeset; // нужны четыре сета, два читающих, два слушающих, для select
	char _buf[_tcp_max + 1]; // максимальный размер пакета http
	//std::map<int, std::string> _readBuffer;// мапа для чтения запросов\request`ов, int -  это фд клиента, string - буффер отдельно для каждого клиента.
	std::map<int, t_req_data>	_readBuffer;
	std::map<int, std::string> _writeBuffer;// мапа для чтения запросов\request`ов, int -  это фд клиента, string - буффер отдельно для каждого клиента.
	/* Ключ-значение fd-port */
	std::map<int, int>	_fdPort; // map связывающий присвоенный fd сервера (ключ) c его портом (значение)
	int 				_current_port; //текущий порт
	int 				_serverFd; //текущий номер используемого сервера из конфига
	std::map<int, size_t>	_fd_size_to_send;


public:
	ServerEngine();
	virtual ~ServerEngine();

	int setup();
	void run();
	int ft_select(int mx, timeval *timeout);
	bool ft_send();
	bool ft_receive();
	bool ft_accept(int *mx);
	bool check_request(t_req_data &buffer);
	void serv_clear();

	void setAddr(int port, std::string &host);
	std::map<int, std::string> getPorts() const
	{ return (this->_ports_host); }
	int const &getFd() const
	{ return (this->_fd); }
	void setFd(int const &src)
	{ this->_fd = src; }

};

#endif
