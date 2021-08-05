#include "ServerEngine.hpp"

ServerEngine::ServerEngine() : _ready(false)
{
	// this->_ports.insert(80);
	// this->_ports.insert(81);
	// в конфиге пройтись по всем серверам и загнать все прты из серверов в _ports/ не здесь а в start_server
	_listen_fds.clear();
	displayTimeStamp();
	std::cout << "Default WebServer created.\n";
}

ServerEngine::~ServerEngine()
{
	std::cout << "WebServer destroyed!\n";
}

static int hostToInt(std::string& str) {
	int ip = 0;
	size_t pos;

	if ((pos = str.find('.')) != std::string::npos) {
		ip = std::stoi(str.substr(0, pos)) << 8;
		str.erase(0, pos + 1);
		if ((pos = str.find('.')) != std::string::npos) {
			ip = (ip + std::stoi(str.substr(0, pos))) << 8;
			str.erase(0, pos + 1);
			if ((pos = str.find('.')) != std::string::npos) {
				ip = (ip + std::stoi(str.substr(0, pos))) << 8;
				str.erase(0, pos + 1);
				if (!str.empty()) {
					ip = (ip + std::stoi(str.substr(0)));
					return ip;
				}
			}
		}
	}
	std::cerr << "Error! Invalid host in configfile" << str << std::endl;
	return (1);
}

void ServerEngine::setAddr(int port, std::string &host)
{
	memset((char *)&_addr, 0, sizeof(_addr));

	uint32_t i;
	i = hostToInt(host);
	/* Cемейство адресов (говорим что рабоатет с интернетом) */
	_addr.sin_family = AF_INET;
	/* IP adress */
	/* Номер хоста (host to network)*/
//	inet_aton(host.c_str(), &_addr.sin_addr);
	_addr.sin_addr.s_addr = htonl(static_cast<uint32_t>(i));//  inet_addr(host.c_str()); - нерабочий альтернативный вариант  INADDR_ANY; - рабочий альтернативный вариант
	/* Номер порта (host to network)*/
	_addr.sin_port = htons(port);
}

int ServerEngine::setup()
{
	/*
	* запуск парсера, добавить в структуру класса экземпляр класса конфига
	* * в цикле заполнить порты из конфига
	*/
	std::string configfile = "./default.conf";
	_config.getServers().clear();
	_config.parse(configfile);
	_ports_host.clear();

	/* Добавляеям порты */
	for (std::map<int, ServerData>::iterator it = _config.getServers().begin(); it != _config.getServers().end(); ++it)
	{
		_ports_host[it->second.getPort()] = it->second.getHost();
	}

	int i = 0;
	/* Каждому порту инициалируем сокет(fd) */
	for (std::map<int, std::string>::iterator it = _ports_host.begin(); it != _ports_host.end(); ++it) //связываем все порты и хосты с fd, которые потом будем слушать
	{
		/* Атрибуты сокета домен тип и протокол */
		_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (_fd < 0)
		{
			std::cerr << "ERROR! Could not create server." << std::endl;
			return (1);
		}
		if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
		{
			std::cerr << "ERROR! fcnlt." << std::endl;
			return (1);
		}
		int a = 1;
		/* устанавливаем сокету повторное использование локальных адресов  */
		if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&a, sizeof(int)) < 0)
		{
			perror("setsockopt(SO_REUSEADDR) failed");
			return (1);
		}
//	#ifdef SO_REUSEPORT
//		if (setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &a, sizeof(a)) < 0)
//			perror("setsockopt(SO_REUSEPORT) failed");
//	#endif
		setAddr(it->first, it->second);
		_fdPort[_fd] = it->first;

		/* Для явного связывания сокета с некоторым адресом используется функция bind */
		if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
		{
			std::cerr << "ERROR! Could not bind port " << (it)->first << "." << std::endl;
			return (1);
		}
		if (listen(_fd, _queue) < 0)
		{
			std::cerr << "Could not listen." << std::endl;
			return (1);
		}
		_listen_fds.insert(_fd);
		displayTimeStamp();
		std::cout << "Running server '" << _config.getServers().find(i)->second.getServerName() << "' on http://" << _config.getServers().find(i)->second.getHost() << ':' << it->first  << std::endl;
		++i;
	}
	_ready = true;
	return (0);
}

int ServerEngine::ft_select(int mx, timeval *timeout)
{
	int ret;

	FD_ZERO(&_readset); // всегда обновлять в начале while
	FD_ZERO(&_writeset); // всегда обновлять в начале while
#if DEBUG
	displayTimeStamp();
	std::cout << YELLOW << "ft_select" << END << ", mx = " << mx << std::endl;
#endif
//	std::cout << "_listen_fds size: " << _listen_fds.size() << std::endl;
//	std::cout << "_clients_recv size: " << _clients_recv.size() << std::endl;
//	std::cout << "_clients_send size: " << _clients_send.size() << std::endl;
	memcpy(&_readset, &_readset_master, sizeof(_readset_master));
	memcpy(&_writeset, &_writeset_master, sizeof(_writeset_master));
	errno = 0;
	ret = select(mx + 1, &_readset, &_writeset, NULL, timeout);
	if (ret < 0)
	{
		perror("select");
		throw HTTPInternalServerError();
	}
	return (ret);
}

bool ServerEngine::check_request(t_req_data &buffer)
{

	// вырезать из chunked запроса "размер в int в шестнадцатеричном формате"\r\n\ в итоговом запросе этого быть не должно
	// std::transform(buffer.begin(), buffer.end(), buffer.begin(), tolower); // В ответе должен быть верблюжий синтаксис

	size_t prev = 0;
	size_t next = 0;
//	std::string tmp;

	if (!buffer._is_body)
	{
		buffer._start_body = buffer.buffer.find("\r\n\r\n", prev);
		if (buffer._start_body != std::string::npos)
		{
			buffer._is_body = true;
			buffer._start_body += 4;
		}
	}
	if (buffer._is_body)
	{
		if (!buffer._is_not_body && !buffer._is_content_length && !buffer._is_transfer_encoding)
		{
			next = buffer.buffer.find("Content-Length: ");
			if (next != std::string::npos)
			{
				buffer._is_content_length = true;
				prev = next + 16;
				next = buffer.buffer.find("\r\n", prev);
				if (next != std::string::npos)
				{
					buffer._body_size = std::stoi(buffer.buffer.substr(prev, next - prev));
				}
			}
			else
			{
				next = buffer.buffer.find("Transfer-Encoding: ");
				if (next != std::string::npos)
				{
					prev = next + 19;
					next = buffer.buffer.find("\r\n", prev);
					if (next != std::string::npos)
					{
						if (buffer.buffer.substr(prev, next - prev) == "chunked")
						{
							buffer._is_transfer_encoding = true;
							buffer._cur_pos = buffer._start_body;
						}
					}
				}
				else
				{
					buffer._is_not_body = true;
				}
			}
		}
		if (buffer._is_not_body)
		{
			return true;
		}
		else if (buffer._is_content_length)
		{
			if (buffer.buffer.size() - buffer._start_body >= buffer._body_size)
			{
				return true;
			}
		}
		else if (buffer._is_transfer_encoding)
		{
			next = buffer.buffer.find("0\r\n\r\n", buffer._cur_pos);
			if (next != std::string::npos)
			{
				return true;
			}
//			next = buffer.buffer.rfind("0", buffer.buffer.size(), buffer.buffer.size() - buffer._cur_pos);
			next = buffer.buffer.find_last_of("0", buffer.buffer.size(), buffer.buffer.size() - buffer._cur_pos);
			if (next != std::string::npos && next > buffer._cur_pos)
			{
				buffer._cur_pos = next;
			}
		}
	}
	return false;
}

bool ServerEngine::ft_send()
{
	static size_t nbr;
	bool ret = false;
	int fd;
	for (std::set<int>::iterator it = _clients_send.begin(); it != _clients_send.end();)
	{
		fd = *it;
		++it;
		if (FD_ISSET(fd, &_writeset)) // поступили данные на отправку, отправляем
		{
		#if DEBUG
			displayTimeStamp();
			std::cout << GREEN << "ft_send" << END << std::endl;
		#endif
			errno = 0;
			if (_writeBuffer.find(fd) == _writeBuffer.end())
			{
				_writeBuffer[fd] = request[fd].respond(_config.getServers().find(_serverFd)->second);
				_fd_size_to_send[fd] = _writeBuffer[fd].length();
			}
			size_t len = _fd_size_to_send[fd];
			if (len > _tcp_max)
				len = _tcp_max;
			ssize_t bytes_sent = send(fd, _writeBuffer[fd].c_str() + _writeBuffer[fd].length() - _fd_size_to_send[fd], len, 0);
			_fd_size_to_send[fd] -= bytes_sent;
		#if DEBUG
			displayTimeStamp();
			std::cout << "Bytes sent: " << bytes_sent << ", length left: " << _fd_size_to_send[*it] << std::endl;
		#endif
			ret = true;
			if (bytes_sent < 0) // || request[fd].getHeaders().find("Connection")->second == "close")
			{
				_fd_size_to_send.erase(fd);
				_writeBuffer.erase(fd);
				FD_CLR(fd, &_readset_master);
				FD_CLR(fd, &_writeset_master);
				close(fd);
				_readBuffer.erase(fd);
				_clients_send.erase(fd);
				ret = true;
				continue;
			}
			if (_fd_size_to_send[fd] == 0)
			{
				++nbr;
				if (nbr % 1000 == 0)
				{
					displayTimeStamp();
					std::cout << "Response count: " << nbr << std::endl;
				}
				std::string statusCode = _writeBuffer[fd].substr(9, _writeBuffer[fd].find(' ', 9) - 9);
				_fd_size_to_send.erase(fd);
				_writeBuffer.erase(fd);
				FD_CLR(fd, &_writeset_master);
				if (statusCode != "200") // statusCode != "200" || request[fd].getMethod() == "POST"
				{
					close(fd);
					_clients_recv.erase(fd);
					FD_CLR(fd, &_readset_master);
				}
				else
				{
					_clients_recv.insert(fd);
					FD_SET(fd, &_readset_master);
				}
				_clients_send.erase(fd);
				continue;
			}
		}
	}
	return (ret);
}

bool ServerEngine::ft_receive()
{

	bool ret;

	ret = false;
	int	fd;
	try
	{
		for (std::set<int>::iterator it = _clients_recv.begin(); it != _clients_recv.end();)
		{
			fd = *it;
			++it;
			if (FD_ISSET(fd, &_readset))
			{
				// Поступили данные от клиента, читаем их
				errno = 0;
				ssize_t bytes_read;
				bytes_read = recv(fd, _buf, _tcp_max, 0);
				if (bytes_read > 0)
					_buf[bytes_read] = '\0';
#if DEBUG
				displayTimeStamp();
				std::cout << MAGENTA << "ft_receive" << END << ", bytes_read = " << bytes_read << ", fd = " << fd << std::endl;
#endif
				if (bytes_read <= 0)
				{
					FD_CLR(fd, &_readset_master);
					FD_CLR(fd, &_writeset_master);
					close(fd);
					_readBuffer.erase(fd);
					_clients_send.erase(fd);
					ret = true;
					continue;
				}
				/*
				 * Если recv возвращает 0 или -1 то закрываем сокет удаляем из сета write и read
				 * Добавить обработку сигнала в main sig_int и в функции обработки изменить значение булевой глоб переменной на false
				 * Очистка структур, очистка классов, закрытие сокетов
				 */
				_readBuffer[fd].buffer += _buf;
				if (check_request(_readBuffer[fd]))
				{
					request[fd].clear();
					request[fd].parse(_readBuffer[fd], _config.getServers().find(_serverFd)->second);
#if DEBUG
					displayTimeStamp();
					std::cout << request[fd].getMethod() << ' ' << request[fd].getLocation() << std::endl;
#endif
					_clients_send.insert(fd);
					FD_SET(fd, &_writeset_master);
					_clients_recv.erase(fd);
					FD_CLR(fd, &_readset_master);
					_readBuffer.erase(fd);
				}
				ret = true;
				continue;
			}
		}
	}
	catch (HTTPError &e)
	{
		request[fd].setError(e.what());
		ret = true;
	}
	return (ret);
}

bool ServerEngine::ft_accept(int *mx)
{
	bool ret = false;

	for (std::set<int>::iterator it = _listen_fds.begin(); it != _listen_fds.end(); ++it)
	{
//		Определяем тип события и выполняем соответствующие действия
		if (FD_ISSET(*it, &_readset))
		{
		#if DEBUG
			displayTimeStamp();
			std::cout << BLUE << "ft_accept" << END << std::endl;
		#endif
			// Поступил новый запрос на соединение, используем accept
			errno = 0;
			int sock = accept(*it, nullptr, nullptr);
			if (sock < 0)
			{
				perror("ERROR accept");
				throw HTTPInternalServerError();
			}
			int i = 1;
			if (setsockopt(sock , SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int)) < 0)
			{
				perror("ERROR setsockopt");
				throw HTTPInternalServerError();
			}
			fcntl(sock, F_SETFL, O_NONBLOCK);
			_current_port = _fdPort.find(*it)->second;
			_serverFd = -1;
			for (std::map<int, ServerData>::iterator it_data = _config.getServers().begin(); it_data != _config.getServers().end(); ++it_data)
			{
				if (it_data->second.getPort() == _current_port)
				{
					_serverFd = it_data->first;
					break;
				}
			}
			if (_serverFd < 0)
				throw HTTPInternalServerError();
			if (*mx < sock)
				*mx = sock;
			_clients_recv.insert(sock);
			FD_SET(sock, &_readset_master);
			ret = true;
			break;
		}
	}
	return (ret);
}

void ServerEngine::run()
{
	if (!_ready)
	{
		std::cerr << "Server is not ready (error in setup)" << std::endl;
		return ;
	}
	int mx = *std::max_element(_listen_fds.begin(), _listen_fds.end());
	_clients_recv.clear();
	_clients_send.clear();
	FD_ZERO(&_readset_master);
	FD_ZERO(&_writeset_master);

	for (std::set<int>::iterator it = _listen_fds.begin(); it != _listen_fds.end(); ++it)
		FD_SET(*it, &_readset_master);

	struct timeval tv = {_timeout, 0};
	bool _run = true;
	_current_port = 0;
	while (_run)
	{
		bool sel = true;
		while (sel)
		{
			int ret = ft_select(mx, &tv);
			if (ret > 0)
				sel = false;
		}
		if (!sel)
		{
			sel = ft_accept(&mx); // проверяем запросы на соединение
		}
		if (!sel)
		{
			sel = ft_receive(); // получение данные от клиента
		}
		if (!sel)
		{
			sel = ft_send(); // отправка данных клиенту
		}

		sel = true;
	}
}
