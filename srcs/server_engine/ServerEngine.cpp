#include "ServerEngine.hpp"

ServerEngine::ServerEngine(std::set<int> const &ports)
{
//	for (std::set<int>::iterator it = ports.begin(); it != ports.end(); ++it)
//		this->_ports.insert(*it);
	_listen_fds.clear();
	std::cout << "WebServer created.\n";
}

ServerEngine::ServerEngine()
{
	// this->_ports.insert(80);
	// this->_ports.insert(81);
	// в конфиге пройтись по всем серверам и загнать все прты из серверов в _ports/ не здесь а в start_server
	_listen_fds.clear();
	std::cout << "Default WebServer created.\n";
}

ServerEngine::~ServerEngine()
{
	std::cout << "WebServer destroyed!\n";
}

// static void        print_servers(ParserConfig & i){
//     std::cout << "PARSER CONFIG" << std::endl;
//     // std::cout << i.getServers()[0].getPort();
//     size_t index = 0;
//     size_t i_loc;
//     size_t i_vect;
//     while (index < i.getServers().size())
//     {
//         i_loc = 0;

//         std::cout << "Server: " << index <<  std::endl;
//         std::cout << "Port: " << i.getServers()[index].getPort() <<  std::endl;
//         std::cout << "Host: " << i.getServers()[index].getHost() <<  std::endl;
//         std::cout << "Server_name: " << i.getServers()[index].getServerName() <<  std::endl;
//         while (i_loc < i.getServers()[index].getLocationData().size())
//         {
//             i_vect = 0;
//             if (!i.getServers()[index].getLocationData()[i_loc].getLocationPath().empty())
//                 std::cout << "LocationPath: " << i.getServers()[index].getLocationData()[i_loc].getLocationPath() <<  std::endl;
//             if (!i.getServers()[index].getLocationData()[i_loc].getRoot().empty())
//                 std::cout << "Root: " << i.getServers()[index].getLocationData()[i_loc].getRoot() <<  std::endl;
//             if (!i.getServers()[index].getLocationData()[i_loc].getFullPath().empty())
//                 std::cout << "FullPath: " << i.getServers()[index].getLocationData()[i_loc].getFullPath() <<  std::endl;
//             while (i_vect < i.getServers()[index].getLocationData()[i_loc].getMethods().size())
//             {
//                 std::cout << "Method: " << i_vect << " "<< i.getServers()[index].getLocationData()[i_loc].getMethods()[i_vect] <<  std::endl;
//                 ++i_vect;
//             }
//             if (!i.getServers()[index].getLocationData()[i_loc].getIndex().empty())
//                 std::cout << "Index " << i.getServers()[index].getLocationData()[i_loc].getIndex() <<  std::endl;
//             i_loc++;
//         }
//         // std::cout << "LocationPath: " << i.getServers()[index].getLocationData()[0].getLocationPath() <<  std::endl;

//         ++index;
//         std::cout << "SERV END" << std::endl <<std::endl;
//     }
// }

//int _HostFromStrToInt(std::string& str) {
//	int ip;
//	size_t _find;
//	ip = 0;
//
//	if ((_find = str.find(".")) != std::string::npos)
//	{
//		ip = atoi(str.substr(0, _find).c_str()) << 8;
//		str.erase(0, _find + 1);
//		if ((_find = str.find(".")) != std::string::npos)
//		{
//			ip = (ip + atoi(str.substr(0, _find).c_str())) << 8;
//			str.erase(0, _find + 1);
//			if ((_find = str.find(".")) != std::string::npos)
//			{
//				ip = (ip + atoi(str.substr(0, _find).c_str())) << 8;
//				str.erase(0, _find + 1);
//				if (!str.empty())
//				{
//					ip = (ip + atoi(str.substr(0).c_str()));
//					return ip;
//				}
//			}
//		}
//	}
//	std::cerr << "Error! Invalid host in configfile" << str << std::endl;
//}


int _hostToInt(std::string str) {
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

void ServerEngine::setAddr(int port, std:: string &host)
{
	memset((char *)&_addr, 0, sizeof(_addr));

	uint32_t i;
//	std::cout << "host is: " << host << std::endl;
	i = _hostToInt(host);
	/* Cемейство адресов (говорим что рабоатет с интернетом) */
	_addr.sin_family = AF_INET;
	/* IP adress */
	std::cout << "host is: " << host << std::endl;
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
	_config.Parser(configfile);
	_ports_host.clear();

//	print_servers(_p);

	/* Добавляеям порты */
	for (std::map<int, ServerData>::iterator it = _config.getServers().begin(); it != _config.getServers().end(); ++it)
	{
		_ports_host[(it->second.getPort())] = it->second.getHost();
	}

	/* Каждому порту инициалируем сокет(fd) */
	for (std::map<int, std::string>::iterator it = _ports_host.begin(); it != _ports_host.end(); ++it) //связываем все порты и хосты с fd, которые потом будем слушать
	{

		/* Атрибуты сокета домен тип и протокол */

		_fd = socket(AF_INET, SOCK_STREAM, 0);
		// std::cout << "socket: " << _fd << std::endl;
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
		_fdPort[_fd] = (it)->first;

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
		std::cout << "servStart port: " << (it)->first  << std::endl;
	}
	return (0);
}

int ServerEngine::ft_select(int mx, timeval *timeout)
{
	int ret;

	FD_ZERO(&_readset); // всегда обновлять в начале while
	FD_ZERO(&_writeset); // всегда обновлять в начале while
	std::cout << "ft_select" << std::endl;
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

	for (std::set<int>::iterator it = _clients_send.begin();
		 it != _clients_send.end(); ++it)
	{
		if (FD_ISSET(*it, &_writeset)) // поступили данные на отправку, отправляем
		{
			std::cout << "ft_send" << std::endl;
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
//			if (serverFd < 0)
//				throw std::exception();
			ServerData const &data = _config.getServers()[serverFd];
			std::string msg = request.respond(_config, data);
//			добавить хедеры в результат выполнения cgi
			send(*it, msg.c_str(), msg.length(), 0);
//			std::cout << "CGI returned: '" << check_cgi << "'" << std::endl;
//			{
//				CGI cgi(request, data, "./testers/cgi_tester", "py");
//				CGI cgi(request, data, "./cgi_scripts/test.py", "py");
//				cgi.runCGI(); // для тестирования CGI
//				std::string cgi_msg = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(cgi_out.length()) + "\r\n\r\n" + cgi_out;
//				send(*it, cgi_out.c_str(), cgi_out.length(), 0); // проверка отправки результата выполнения cgi
//			}
			std::cout << "Respond on " << *it << std::endl;
			std::cout << "Server name: " << data.getServerName() << std::endl;
			_clients_recv.insert(*it);
			FD_CLR(*it, &_writeset_master);
			FD_CLR(*it, &_readset_master);
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
		std::cout << "body size (check please): " << body_size << std::endl;
		next = buffer.find("Content-Length: ", prev); //проверяем наличие content-lenght
		if (next != std::string::npos)
		{
			prev = next + 16;
			next = buffer.find("\r\n", prev);
			if (next != std::string::npos)
			{
				tmp = buffer.substr(prev, next - prev);
				int content_length = std::stoi(tmp);
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

	for (std::set<int>::iterator it = _clients_recv.begin();
		 it != _clients_recv.end(); it++)
	{
		// std::cout << "recv " << *it << std::endl;
		if (FD_ISSET(*it, &_readset))
		{
			std::cout << "ft_receive" << std::endl;
			bool full_request = true;
			// std::cout << "RECV" << std::endl;
			// Поступили данные от клиента, читаем их
			errno = 0;
			ssize_t bytes_read;
			bzero(_buf, TCP_MAX + 1); // 65536 - максим размер пакета tcp
			bytes_read = recv(*it, _buf, TCP_MAX, 0);
			if (bytes_read < 0) // was <= 0
			{
				// удаляем сокет из множества
				close(*it);
				_buffer[*it].clear();
				_clients_recv.erase(*it);
				ret = true;
				break;
			}
			/*
			 * Если recv возвращает 0 или -1 то закрываем сокет удаляем из сета write и read
			 * Добавить обработку сигнала в main sig_int и в функции обработки изменить значение булевой глоб переменной на false
			 * Очистка структур, очистка классов, закрытие сокетов
			 * */
			//проверить \r\n\r\n, затем проверяем наличие content-lenght, если число равно боди. Нужно будет актуальный размер боди сравнивать с этим числом.
			//Если content-lenght нет, то проверить transfer encoding и равен ли он chunked. Если равен, то дождаться, когда придет "0\r\n\r\n" (неважно, в конце боди или нет, важно, чтобы после первого \r\n\r\n)
			// Если нету ни content length или transfer encoding и есть \r\n\r\n, то значит запрос пришел полностью
			// std::string buffer;
			_buffer[*it] += std::string(_buf);
			// buffer += std::string(_buf);
			full_request = check_request(_buffer[*it]);
			// std::cout << "Read:"<< std::endl << _buffer[*it] << std::endl << "Read END!"<< std::endl;
			if (full_request || true)
			{
				request.parse(_buffer[*it]);
				std::cout << request.getMethod() << ' ' << request.getLocation()
						  << std::endl << std::endl;
				_clients_send.insert(*it);
				FD_SET(*it, &_writeset_master);
				_clients_recv.erase(*it);
				_buffer[*it].clear();
			}
			ret = true;
//			 if(bytes_read <= 0)
//			 {
//			     // Соединение разорвано, удаляем сокет из множества
//			     close(*it);
//			     clients.erase(*it);
//			     continue;
//			 }
			break;
		}
	}
	return (ret);
}

bool ServerEngine::ft_accept(int *mx, int *current_port)
{

	bool ret;

	ret = false;

	for (std::set<int>::iterator it = _listen_fds.begin(); it != _listen_fds.end(); ++it)
	{
//		std::cout << "accept " << *it << std::endl;
//		Определяем тип события и выполняем соответствующие действия
		if (FD_ISSET(*it, &_readset))
		{
			std::cout << "ft_accept" << std::endl;
			// Поступил новый запрос на соединение, используем accept
			errno = 0;
			int sock = accept(*it, NULL, NULL);
//			std::cout << "accept complete " << sock << std::endl;
			if (sock < 0)
			{
				perror("accept");
				exit(3);
			}
			*current_port = _fdPort[*it];
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

	struct timeval tv = {3600, 0};
	struct timeval timeout;
	timeout.tv_sec = 15;
	timeout.tv_usec = 0;
	bool _run = true;
	int current_port = 0;
	Request request;
	while (_run)
	{
		// std::cout << "start" << std::endl;
		// Заполняем множество сокетов

		bool sel = true;
		while (sel)
		{
			// Ждём события в одном из сокетов
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
		// std::cout << "end" << std::endl;
	}
}
