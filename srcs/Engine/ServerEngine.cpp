#include "ServerEngine.hpp"

ServerEngine::ServerEngine(std::set<int> const &ports)
{
//	for (std::set<int>::iterator it = ports.begin(); it != ports.end(); ++it)
//		this->_ports.insert(*it); // where is _ports ??
	_listen_fds.clear();
	displayTimeStamp();
	std::cout << "WebServer created.\n";
}

ServerEngine::ServerEngine()
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

int _hostToInt(std::string& str) {
	int ip = 0;
	size_t pos;

	if ((pos = str.find(".")) != std::string::npos) {
		ip = atoi(str.substr(0, pos).c_str()) << 8;
		str.erase(0, pos + 1);
		if ((pos = str.find(".")) != std::string::npos) {
			ip = (ip + atoi(str.substr(0, pos).c_str())) << 8;
			str.erase(0, pos + 1);
			if ((pos = str.find(".")) != std::string::npos) {
				ip = (ip + atoi(str.substr(0, pos).c_str())) << 8;
				str.erase(0, pos + 1);
				if (!str.empty()) {
					ip = (ip + atoi(str.substr(0).c_str()));
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
	i = _hostToInt(host);
	/* Cемейство адресов (говорим что рабоатет с интернетом) */
	_addr.sin_family = AF_INET;
	/* IP adress */
	/* Номер хоста (host to network)*/
//	inet_aton(host.c_str(), &_addr.sin_addr);
	_addr.sin_addr.s_addr = htonl(static_cast<uint32_t>(i));//  inet_addr(host.c_str()); - нерабочий альтернативный вариант  INADDR_ANY; - рабочий альтернативный вариант
	/* Номер порта (host to network)*/
	_addr.sin_port = htons(port);
}

int ServerEngine::servStart()
{
	/*
	* запуск парсера, добавить в структуру класса экземпляр класса конфига
	* * в цикле заполнить порты из конфига
	*/
	std::string configfile = "./ex.conf";
	_config.getServers().clear();
	_config.parse(configfile);
	_ports_host.clear();

	/* Добавляеям порты */
	for (std::map<int, ServerData>::iterator it = _config.getServers().begin(); it != _config.getServers().end(); ++it)
	{
		_ports_host[it->second.getPort()] = it->second.getHost();
	}

	/* Каждому порту инициалируем сокет(fd) */
	for (std::map<int, std::string>::iterator it = _ports_host.begin(); it != _ports_host.end(); ++it) //связываем все порты и хосты с fd, которые потом будем слушать
	{
		/* Атрибуты сокета домен тип и протокол */
		_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (_fd < 0)
		{
			std::cerr << "ERROR! Could not create server." << std::endl;
			return (-1);
		}
		int a = 1;
		/* устанавливаем сокету повторное использование локальных адресов  */
		if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int)) < 0)
			perror("setsockopt(SO_REUSEADDR) failed");

		setAddr(it->first, it->second);
		_fdPort[_fd] = it->first;

		/* Для явного связывания сокета с некоторым адресом используется функция bind */
		if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
		{
			std::cerr << "ERROR! Could not bind port " << (it)->first << "." << std::endl;
			return (-1);
		}
		if (listen(_fd, 1000) < 0)
		{
			std::cerr << "Could not listen." << std::endl;
			return (-1);
		}
		_listen_fds.insert(_fd);
//		displayTimeStamp();
//		std::cout << "servStart port: " << (it)->first  << std::endl;
	}
	return (0);
}

int ServerEngine::ft_select(int mx, timeval *timeout)
{
	int ret;

	FD_ZERO(&_readset); // всегда обновлять в начале while
	FD_ZERO(&_writeset); // всегда обновлять в начале while
//	displayTimeStamp();
//	std::cout << "ft_select, mx = " << mx << std::endl;
	memcpy(&_readset, &_readset_master, sizeof(_readset_master));
	memcpy(&_writeset, &_writeset_master, sizeof(_writeset_master));
	errno = 0;
	ret = select(mx + 1, &_readset, &_writeset, NULL, timeout);
	if (ret < 0)
	{
		perror("select");
		exit(1);
	}
	return (ret);
}

bool ServerEngine::ft_send(const Request &request, int current_port)
{
	bool ret = false;

	for (std::set<int>::iterator it = _clients_send.begin(); it != _clients_send.end(); ++it)
	{
		if (FD_ISSET(*it, &_writeset)) // поступили данные на отправку, отправляем
		{
//			displayTimeStamp();
//			std::cout << "ft_send" << std::endl;
			errno = 0;
			int serverFd = -1;
			for (std::map<int, ServerData>::iterator it_data = _config.getServers().begin(); it_data != _config.getServers().end(); ++it_data) // servers с пустым сервером
			{
				if (it_data->second.getPort() == current_port)
				{
					serverFd = it_data->first;
					break;
				}
			}
			if (serverFd < 0)
				throw std::exception();
			ServerData const &data = _config.getServers().find(serverFd)->second;
			std::string msg = request.respond(_config, data);
			send(*it, msg.c_str(), msg.length(), 0);
//			std::cout << "CGI returned: '" << check_cgi << "'" << std::endl;
//			{
//				CGI cgi(request, data, "./testers/cgi_tester", "py");
//				CGI cgi(request, data, "./cgi_scripts/test.py", "py");
//				cgi.runCGI(); // для тестирования CGI
//				std::string cgi_msg = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(cgi_out.length()) + "\r\n\r\n" + cgi_out;
//				send(*it, cgi_out.c_str(), cgi_out.length(), 0); // проверка отправки результата выполнения cgi
//			}
			_clients_recv.insert(*it);
			FD_CLR(*it, &_writeset_master);
			FD_CLR(*it, &_readset_master); // на чтение
			close(*it);
			_clients_send.erase(*it);
			ret = true;
			break;
		}
	}
	return (ret);
}

bool ServerEngine::check_request(std::string const &buffer)
{

	// вырезать из chunked запроса "размер в int в шестнадцатеричном формате"\r\n\ в итоговом запросе этого быть не должно
	// std::transform(buffer.begin(), buffer.end(), buffer.begin(), tolower); // В ответе должен быть верблюжий синтаксис

	size_t prev = 0;
	size_t next = 0;
	std::string tmp;

	next = buffer.find("\r\n\r\n", prev);
	//проверить \r\n\r\n, затем проверяем наличие content-lenght, если число равно боди. Нужно будет актуальный размер боди сравнивать с этим числом.
	//Если content-lenght нет, то проверить transfer encoding и равен ли он chunked. Если равен, то дождаться, когда придет "0\r\n\r\n" (неважно, в конце боди или нет, важно, чтобы после первого \r\n\r\n)
	// Если нету ни content length или transfer encoding и есть \r\n\r\n, то значит запрос пришел полностью

	if (next != std::string::npos)
	{
		if (buffer.find("0\r\n\r\n", prev) != std::string::npos)
			return (true);
		size_t body_size = buffer.length() - next - 4;
		next = buffer.find("Content-Length: ", prev); //проверяем наличие content-lenght
		if (next != std::string::npos)
		{
			prev = next + 16;
			next = buffer.find("\r\n", prev);
			if (next != std::string::npos)
			{
				tmp = buffer.substr(prev, next - prev);
				size_t content_length = std::stoi(tmp);
				return (body_size == content_length);
			}
		}
		prev = 0;
		next = buffer.find("Transfer-Encoding: ", prev); // проверяем transfer-encoding, не является ли запрос фрагментированным
		if (next != std::string::npos)
		{
			prev = next + 19;
			next = buffer.find("\r\n", prev);
			if (next != std::string::npos)
			{
				tmp = buffer.substr(prev, next - prev);
				// trim(tmp); //утонить, нужно ли обрезать пробелы и табуляцию по краям. upd. Уточнил - не нужно.
				if (tmp == "chunked")
				{
					// _chunked[fd] = true; // нет необходимости в булевой переменной
					if (buffer.find("0\r\n\r\n", prev) != std::string::npos)
						return (true);
					return (false);
				}
			}
		}
		return (true); // есть \r\n\r\n , нет transfer-encoding и content-lenght, запрос пришел полностью
	}
	return (false); // проверить ошибки 404, 500, 505
}

/*
 * старт лайн
 * хедеры
 * chunked
 * rnrn
 * в боди ищу rn substr - strtol - записываем в переменную, чтобы сравнить со следующим отрезком
 * След отрезок до rn я сохраняю в боди и сверяем со значением strtol - если не совпадает - badrequest
 * по циклу идем, пока strtol не вернет 0 и след отрезок не будет пустым
 *
 * Для response обязательно Content-Length - размер боди
 * Content-Type - тип , если не определяешь, то text/plain
 * Date
 * Connection
 *
 * Если среди хедеров запроса есть хедер, в котором присутствует слово secret, то этот хедер нужно преобразовать и записать в переменные среды для cgi (все тире заменяем на нижнее подчеркивание, все символы переводишь в верхний регистр
 * и в начало добавляешь HTTP_)
 *
 */

bool ServerEngine::ft_receive(Request &request)
{

	bool ret;

	ret = false;

	for (std::set<int>::iterator it = _clients_recv.begin(); it != _clients_recv.end(); ++it)
	{
		if (FD_ISSET(*it, &_readset))
		{
			bool full_request = true;
			// Поступили данные от клиента, читаем их
			errno = 0;
			ssize_t bytes_read;
			bytes_read = recv(*it, _buf, TCP_MAX, 0);
			if (bytes_read >= 0)
				_buf[bytes_read] = '\0';
//			displayTimeStamp();
//			std::cout << "ft_receive, bytes_read = " << bytes_read << ", *it = " << *it << std::endl;
			if (bytes_read <= 0) // was <= 0
			{
				// удаляем сокет из множества
				FD_CLR(*it, &_readset_master);
				FD_CLR(*it, &_writeset_master);
				close(*it);
				_buffer.erase(*it);
				_clients_send.erase(*it);
				ret = true;
				break;
			}
			/*
			 * Если recv возвращает 0 или -1 то закрываем сокет удаляем из сета write и read
			 * Добавить обработку сигнала в main sig_int и в функции обработки изменить значение булевой глоб переменной на false
			 * Очистка структур, очистка классов, закрытие сокетов
			 *
			 * проверить \r\n\r\n, затем проверяем наличие content-lenght, если число равно боди. Нужно будет актуальный размер боди сравнивать с этим числом.
			 * Если content-lenght нет, то проверить transfer encoding и равен ли он chunked. Если равен, то дождаться, когда придет "0\r\n\r\n" (неважно, в конце боди или нет, важно, чтобы после первого \r\n\r\n)
			 * Если нету ни content length или transfer encoding и есть \r\n\r\n, то значит запрос пришел полностью
			 */
			_buffer[*it] += _buf;
			full_request = check_request(_buffer[*it]);
			if (full_request)
			{
				request.clear();
				request.parse(_buffer[*it]);
				displayTimeStamp();
				std::cout << request.getMethod() << ' ' << request.getLocation() << std::endl;
				_clients_send.insert(*it);
				FD_SET(*it, &_writeset_master);
				_clients_recv.erase(*it);
				FD_CLR(*it, &_readset_master);
				_buffer.erase(*it);
			}
			ret = true;
			break;
		}
	}
	return (ret);
}

bool ServerEngine::ft_accept(int *mx, int *current_port)
{
	bool ret = false;

	for (std::set<int>::iterator it = _listen_fds.begin(); it != _listen_fds.end(); ++it)
	{
//		Определяем тип события и выполняем соответствующие действия
		if (FD_ISSET(*it, &_readset))
		{
//			displayTimeStamp();
//			std::cout << "ft_accept" << std::endl;
			// Поступил новый запрос на соединение, используем accept
			errno = 0;
			int sock = accept(*it, NULL, NULL);
			if (sock < 0)
			{
				perror("accept");
				exit(3);
			}
			*current_port = _fdPort.find(*it)->second;
			if (*mx < sock)
				*mx = sock;
			fcntl(sock, F_SETFL, O_NONBLOCK);
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
	int mx = *max_element(_listen_fds.begin(), _listen_fds.end());
	_clients_recv.clear();
	_clients_send.clear();
	FD_ZERO(&_readset_master);
	FD_ZERO(&_writeset_master);

	for (std::set<int>::iterator it = _listen_fds.begin(); it != _listen_fds.end(); ++it)
		FD_SET(*it, &_readset_master);

	struct timeval tv = {10, 0};
	struct timeval timeout;
	timeout.tv_sec = 15;
	timeout.tv_usec = 0;
	bool _run = true;
	int current_port = 0;
	Request request;
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
			sel = ft_send(request, current_port); // отправка данных клиенту
		}
		if (!sel)
		{
			try
			{
				sel = ft_receive(request); // получение данные от клиента
			}
			catch (HTTPError &e)
			{
				request.setError(e.what());
				sel = true;
			}
		}
		if (!sel)
		{
			sel = ft_accept(&mx, &current_port); // проверяем запросы на соединение
		}
		sel = true;
	}
}
