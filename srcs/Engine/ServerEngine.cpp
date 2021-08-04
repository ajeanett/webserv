#include "ServerEngine.hpp"ServerEngine::ServerEngine(std::set<int> const &ports){//	for (std::set<int>::iterator it = ports.begin(); it != ports.end(); ++it)//		this->_ports.insert(*it); // where is _ports ??	_listen_fds.clear();	displayTimeStamp();	std::cout << "WebServer created.\n";}ServerEngine::ServerEngine(){	// this->_ports.insert(80);	// this->_ports.insert(81);	// в конфиге пройтись по всем серверам и загнать все прты из серверов в _ports/ не здесь а в start_server	_listen_fds.clear();	displayTimeStamp();	std::cout << "Default WebServer created.\n";}ServerEngine::~ServerEngine(){	std::cout << "WebServer destroyed!\n";}int _hostToInt(std::string& str) {	int ip = 0;	size_t pos;	if ((pos = str.find(".")) != std::string::npos) {		ip = atoi(str.substr(0, pos).c_str()) << 8;		str.erase(0, pos + 1);		if ((pos = str.find(".")) != std::string::npos) {			ip = (ip + atoi(str.substr(0, pos).c_str())) << 8;			str.erase(0, pos + 1);			if ((pos = str.find(".")) != std::string::npos) {				ip = (ip + atoi(str.substr(0, pos).c_str())) << 8;				str.erase(0, pos + 1);				if (!str.empty()) {					ip = (ip + atoi(str.substr(0).c_str()));					return ip;				}			}		}	}	std::cerr << "Error! Invalid host in configfile" << str << std::endl;	return (1);}void ServerEngine::setAddr(int port, std::string &host){	memset((char *)&_addr, 0, sizeof(_addr));	uint32_t i;	i = _hostToInt(host);	/* Cемейство адресов (говорим что рабоатет с интернетом) */	_addr.sin_family = AF_INET;	/* IP adress */	/* Номер хоста (host to network)*///	inet_aton(host.c_str(), &_addr.sin_addr);	_addr.sin_addr.s_addr = htonl(static_cast<uint32_t>(i));//  inet_addr(host.c_str()); - нерабочий альтернативный вариант  INADDR_ANY; - рабочий альтернативный вариант	/* Номер порта (host to network)*/	_addr.sin_port = htons(port);}int ServerEngine::setup(){	/*	* запуск парсера, добавить в структуру класса экземпляр класса конфига	* * в цикле заполнить порты из конфига	*/	std::string configfile = "./ex.conf";	_config.getServers().clear();	_config.parse(configfile);	_ports_host.clear();	/* Добавляеям порты */	for (std::map<int, ServerData>::iterator it = _config.getServers().begin(); it != _config.getServers().end(); ++it)	{		_ports_host[it->second.getPort()] = it->second.getHost();	}	/* Каждому порту инициалируем сокет(fd) */	for (std::map<int, std::string>::iterator it = _ports_host.begin(); it != _ports_host.end(); ++it) //связываем все порты и хосты с fd, которые потом будем слушать	{		/* Атрибуты сокета домен тип и протокол */		_fd = socket(AF_INET, SOCK_STREAM, 0);		if (_fd < 0)		{			std::cerr << "ERROR! Could not create server." << std::endl;			return (-1);		}		fcntl(_fd, F_SETFL, O_NONBLOCK);		int a = 1;//		struct timeval tv = {60, 0};		/* устанавливаем сокету повторное использование локальных адресов  */		if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&a, sizeof(int)) < 0)			perror("setsockopt(SO_REUSEADDR) failed");//	#ifdef SO_REUSEPORT//		if (setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &a, sizeof(a)) < 0)//			perror("setsockopt(SO_REUSEPORT) failed");//	#endif		setAddr(it->first, it->second);		_fdPort[_fd] = it->first;		/* Для явного связывания сокета с некоторым адресом используется функция bind */		if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)		{			std::cerr << "ERROR! Could not bind port " << (it)->first << "." << std::endl;			return (-1);		}		if (listen(_fd, _queue) < 0)		{			std::cerr << "Could not listen." << std::endl;			return (-1);		}		_listen_fds.insert(_fd);//		displayTimeStamp();//		std::cout << "setup port: " << (it)->first  << std::endl;	}	return (0);}int ServerEngine::ft_select(int mx, timeval *timeout){	int ret;	FD_ZERO(&_readset); // всегда обновлять в начале while	FD_ZERO(&_writeset); // всегда обновлять в начале while#if DEBUG	displayTimeStamp();	std::cout << YELLOW << "ft_select" << END << ", mx = " << mx << std::endl;#endif//	std::cout << "listen_fds size: " << _listen_fds.size() << std::endl;//	std::cout << "_clients_recv size: " << _clients_recv.size() << std::endl;//	std::cout << "_clients_send size: " << _clients_send.size() << std::endl;	memcpy(&_readset, &_readset_master, sizeof(_readset_master));	memcpy(&_writeset, &_writeset_master, sizeof(_writeset_master));	errno = 0;	ret = select(mx + 1, &_readset, &_writeset, NULL, timeout);	if (ret < 0)	{		perror("select");		exit(1);	}	return (ret);}bool ServerEngine::check_request(t_req_data &buffer){	// вырезать из chunked запроса "размер в int в шестнадцатеричном формате"\r\n\ в итоговом запросе этого быть не должно	// std::transform(buffer.begin(), buffer.end(), buffer.begin(), tolower); // В ответе должен быть верблюжий синтаксис	size_t prev = 0;	size_t next = 0;//	std::string tmp;	if (!buffer._is_body)	{		buffer._start_body = buffer.buffer.find("\r\n\r\n", prev);		if (buffer._start_body != std::string::npos)		{			buffer._is_body = true;			buffer._start_body += 4;		}	}	if (buffer._is_body)	{		if (!buffer._is_not_body && !buffer._is_content_length && !buffer._is_transfer_encoding)		{			next = buffer.buffer.find("Content-Length: ");			if (next != std::string::npos)			{				buffer._is_content_length = true;				prev = next + 16;				next = buffer.buffer.find("\r\n", prev);				if (next != std::string::npos)				{					buffer._body_size = std::stoi(buffer.buffer.substr(prev, next - prev));				}			}			else			{				next = buffer.buffer.find("Transfer-Encoding: ");				if (next != std::string::npos)				{					prev = next + 19;					next = buffer.buffer.find("\r\n", prev);					if (next != std::string::npos)					{						if (buffer.buffer.substr(prev, next - prev) == "chunked")						{							buffer._is_transfer_encoding = true;							buffer._cur_pos = buffer._start_body;						}					}				}				else				{					buffer._is_not_body = true;				}			}		}		if (buffer._is_not_body)		{			return true;		}		else if (buffer._is_content_length)		{			if (buffer.buffer.size() - buffer._start_body >= buffer._body_size)			{				return true;			}		}		else if (buffer._is_transfer_encoding)		{			next = buffer.buffer.find("0\r\n\r\n", buffer._cur_pos);			if (next != std::string::npos)			{				return true;			}//			next = buffer.buffer.rfind("0", buffer.buffer.size(), buffer.buffer.size() - buffer._cur_pos);			next = buffer.buffer.find_last_of("0", buffer.buffer.size(), buffer.buffer.size() - buffer._cur_pos);			if (next != std::string::npos && next > buffer._cur_pos)			{				buffer._cur_pos = next;			}		}	}	return false;//	next = buffer.find("\r\n\r\n", prev);//	//проверить \r\n\r\n, затем проверяем наличие content-lenght, если число равно боди. Нужно будет актуальный размер боди сравнивать с этим числом.//	//Если content-lenght нет, то проверить transfer encoding и равен ли он chunked. Если равен, то дождаться, когда придет "0\r\n\r\n" (неважно, в конце боди или нет, важно, чтобы после первого \r\n\r\n)//	// Если нету ни content length или transfer encoding и есть \r\n\r\n, то значит запрос пришел полностью////	if (next != std::string::npos)//	{////		if (buffer.find("0\r\n\r\n", prev) != std::string::npos)////			return (true);//		size_t body_size = buffer.length() - next - 4;//		next = buffer.find("Content-Length: ", prev); //проверяем наличие content-lenght//		if (next != std::string::npos)//		{//			prev = next + 16;//			next = buffer.find("\r\n", prev);//			if (next != std::string::npos)//			{//				tmp = buffer.substr(prev, next - prev);//				size_t content_length = std::stoi(tmp);//				return (body_size == content_length);//			}//		}//		prev = 0;//		next = buffer.find("Transfer-Encoding: ", prev); // проверяем transfer-encoding, не является ли запрос фрагментированным//		if (next != std::string::npos)//		{//			prev = next + 19;//			next = buffer.find("\r\n", prev);//			if (next != std::string::npos)//			{//				tmp = buffer.substr(prev, next - prev);//				// trim(tmp); //утонить, нужно ли обрезать пробелы и табуляцию по краям. upd. Уточнил - не нужно.//				if (tmp == "chunked")//				{//					// _chunked[fd] = true; // нет необходимости в булевой переменной//					if (buffer.find("0\r\n\r\n", prev) != std::string::npos)//						return (true);//					return (false);//				}//			}//		}//		return (true); // есть \r\n\r\n , нет transfer-encoding и content-lenght, запрос пришел полностью//	}//	return (false); // проверить ошибки 404, 500, 505}/* * старт лайн * хедеры * chunked * rnrn * в боди ищу rn substr - strtol - записываем в переменную, чтобы сравнить со следующим отрезком * След отрезок до rn я сохраняю в боди и сверяем со значением strtol - если не совпадает - badrequest * по циклу идем, пока strtol не вернет 0 и след отрезок не будет пустым * * Для response обязательно Content-Length - размер боди * Content-Type - тип , если не определяешь, то text/plain * Date * Connection * * Если среди хедеров запроса есть хедер, в котором присутствует слово secret, то этот хедер нужно преобразовать и записать в переменные среды для cgi (все тире заменяем на нижнее подчеркивание, все символы переводишь в верхний регистр * и в начало добавляешь HTTP_) * */bool ServerEngine::ft_send(const Request &request){	bool ret = false;	for (std::set<int>::iterator it = _clients_send.begin(); it != _clients_send.end(); ++it)	{		if (FD_ISSET(*it, &_writeset)) // поступили данные на отправку, отправляем		{		#if DEBUG			displayTimeStamp();			std::cout << GREEN << "ft_send" << END << std::endl;		#endif			errno = 0;			if (_writeBuffer.find(*it) == _writeBuffer.end())			{				_writeBuffer[*it] = request.respond(_config, _config.getServers().find(_serverFd)->second);				_fd_size_to_send[*it] = _writeBuffer[*it].length();			}			size_t len = _fd_size_to_send[*it];			if (len > _tcp_max)				len = _tcp_max;			ssize_t bytes_sent = send(*it, &(_writeBuffer[*it][_writeBuffer[*it].length() - _fd_size_to_send[*it]]), len, 0);			_fd_size_to_send[*it] -= bytes_sent;		#if DEBUG			displayTimeStamp();			std::cout << "Bytes sent: " << bytes_sent << ", length left: " << _writeBuffer[*it].length() << std::endl;		#endif			ret = true;			if (bytes_sent < 0)			{				_fd_size_to_send.erase(*it);				_writeBuffer.erase(*it);				FD_CLR(*it, &_readset_master);				FD_CLR(*it, &_writeset_master);				close(*it);				_readBuffer.erase(*it);				_clients_send.erase(*it);				ret = true;				break;			}			if (_fd_size_to_send[*it] == 0)			{				_fd_size_to_send.erase(*it);				_writeBuffer.erase(*it);				FD_CLR(*it, &_writeset_master);				close(*it);				FD_CLR(*it, &_readset_master);				_clients_recv.erase(*it);				_clients_send.erase(*it);				break;			}		}	}	return (ret);}bool ServerEngine::ft_receive(Request &request){	bool ret;	ret = false;	for (std::set<int>::iterator it = _clients_recv.begin(); it != _clients_recv.end(); ++it)	{		if (FD_ISSET(*it, &_readset))		{			bool full_request = true;			// Поступили данные от клиента, читаем их			errno = 0;			ssize_t bytes_read;			bytes_read = recv(*it, _buf, _tcp_max, 0);			if (bytes_read >= 0)				_buf[bytes_read] = '\0';		#if DEBUG			displayTimeStamp();			std::cout << MAGENTA << "ft_receive" << END << ", bytes_read = " << bytes_read << ", *it = " << *it << std::endl;		#endif			if (bytes_read <= 0)			{				FD_CLR(*it, &_readset_master);				FD_CLR(*it, &_writeset_master);				close(*it);				_readBuffer.erase(*it);				_clients_send.erase(*it);				ret = true;				break;			}			/*			 *			 *			 * Если recv возвращает 0 или -1 то закрываем сокет удаляем из сета write и read			 * Добавить обработку сигнала в main sig_int и в функции обработки изменить значение булевой глоб переменной на false			 * Очистка структур, очистка классов, закрытие сокетов			 *			 * проверить \r\n\r\n, затем проверяем наличие content-lenght, если число равно боди. Нужно будет актуальный размер боди сравнивать с этим числом.			 * Если content-lenght нет, то проверить transfer encoding и равен ли он chunked. Если равен, то дождаться, когда придет "0\r\n\r\n" (неважно, в конце боди или нет, важно, чтобы после первого \r\n\r\n)			 * Если нету ни content length или transfer encoding и есть \r\n\r\n, то значит запрос пришел полностью			 */			_readBuffer[*it].buffer += _buf;			full_request = check_request(_readBuffer[*it]);			if (full_request)			{				request.clear();				request.parse(_readBuffer[*it], _config.getServers().find(_serverFd)->second);			#ifdef DEBUG				displayTimeStamp();				std::cout << request.getMethod() << ' ' << request.getLocation() << std::endl;			#endif				_clients_send.insert(*it);				FD_SET(*it, &_writeset_master);				_clients_recv.erase(*it);				FD_CLR(*it, &_readset_master);				_readBuffer.erase(*it);			}			ret = true;			break;		}	}	return (ret);}bool ServerEngine::ft_accept(int *mx){	bool ret = false;	for (std::set<int>::iterator it = _listen_fds.begin(); it != _listen_fds.end(); ++it)	{//		Определяем тип события и выполняем соответствующие действия		if (FD_ISSET(*it, &_readset))		{		#if DEBUG			displayTimeStamp();			std::cout << BLUE << "ft_accept" << END << std::endl;		#endif			// Поступил новый запрос на соединение, используем accept			errno = 0;			int sock = accept(*it, nullptr, nullptr);			if (sock < 0)			{				perror("ERROR accept");			}			int i = 1;			if (setsockopt(sock , SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int)) < 0)			{				perror("ERROR setsockopt");			}			fcntl(sock, F_SETFL, O_NONBLOCK);			_current_port = _fdPort.find(*it)->second;			_serverFd = -1;			for (std::map<int, ServerData>::iterator it_data = _config.getServers().begin(); it_data != _config.getServers().end(); ++it_data) // servers с пустым сервером			{				if (it_data->second.getPort() == _current_port)				{					_serverFd = it_data->first;					break;				}			}			if (_serverFd < 0)				throw std::exception();			if (*mx < sock)				*mx = sock;			_clients_recv.insert(sock);			FD_SET(sock, &_readset_master);			ret = true;			break;		}	}	return (ret);}void ServerEngine::run(){	int mx = *max_element(_listen_fds.begin(), _listen_fds.end());	_clients_recv.clear();	_clients_send.clear();	FD_ZERO(&_readset_master);	FD_ZERO(&_writeset_master);	for (std::set<int>::iterator it = _listen_fds.begin(); it != _listen_fds.end(); ++it)		FD_SET(*it, &_readset_master);	struct timeval tv = {_timeout, 0};	bool _run = true;	_current_port = 0;	Request request;	while (_run)	{		bool sel = true;		while (sel)		{			int ret = ft_select(mx, &tv);			if (ret > 0)				sel = false;		}		if (!sel)		{			sel = ft_send(request); // отправка данных клиенту		}		if (!sel)		{			try			{				sel = ft_receive(request); // получение данные от клиента			}			catch (HTTPError &e)			{				request.setError(e.what());				sel = true;			}		}		if (!sel)		{			sel = ft_accept(&mx); // проверяем запросы на соединение		}		sel = true;	}}