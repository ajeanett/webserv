#include "ServerEngine.hpp"

ServerEngine::ServerEngine(std::set<int> const & ports){
	for (std::set<int>::iterator it = ports.begin(); it != ports.end(); ++it)
        this->_ports.insert(*it);
	_listen_fds.clear();
    std::cout << "WebServer created.\n";
        }

ServerEngine::ServerEngine(void){
    this->_ports.insert(80);
	this->_ports.insert(81);
	_listen_fds.clear();
    std::cout << "Default WebServer created.\n";
        }

ServerEngine::~ServerEngine(){
            std::cout << "WebServer destroyed!\n";
        }

int		ServerEngine::servStart(void)
{
    for (std::set<int>::iterator it = _ports.begin(); it != _ports.end(); ++it) //связываем все порты с fd, которые потом будем слушать
    {
        _fd = socket(AF_INET, SOCK_STREAM, 0);
        std::cout << "socket: " << _fd << std::endl;
	    if (_fd < 0)
	    {
	    	std::cerr << "ERROR! Could not create server." << std::endl;
	    	return (-1);
	    }
        int a = 1;
        if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int)) < 0)
            perror("setsockopt(SO_REUSEADDR) failed");
        
        setAddr(*it);

	    if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
	    {
	    	std::cerr << "ERROR! Could not bind port " << *it << "."  << std::endl;
	    	return (-1);
	    }
	    if (listen(_fd, 1000) < 0)
	    {
	    	std::cerr << "Could not listen."  << std::endl;
	    	return (-1);
	    }
        _listen_fds.insert(_fd);
    }
    return (0);
}

void		ServerEngine::setAddr(int port)
{
	memset((char *)&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY; // htonl(host);
	_addr.sin_port = htons(port);
}

void		ServerEngine::getStartPage(void)
{
	std::string tmp;
    std::string body;
    std::ifstream ifs("./error.html");
    while (getline(ifs, tmp))
        body += tmp;
    this->_startPage = "HTTP/1.1 OK\r\n\r\n" + body;
}

int     ServerEngine::ft_select(int mx, timeval timeout){
    int ret;

    FD_ZERO(&_readset); // всегда обновлять в начале while и write здесь
    FD_ZERO(&_writeset); // всегда обновлять в начале while и write здесь

    memcpy(&_readset, &_readset_master, sizeof(_readset_master));
    memcpy(&_writeset, &_writeset_master, sizeof(_writeset_master));
    errno = 0;
    ret = select(mx+1, &_readset, &_writeset, NULL, &timeout);
    if (ret < 0)
    {
        perror("select");
        // exit(3);
    }
    return (ret);
}

bool ServerEngine::ft_send(){

    bool ret;

    ret = false;

    for (std::set<int>::iterator it = _clients_send.begin(); it != _clients_send.end(); ++it)
    {
        std::cout << "send " << *it << std::endl;
        if (FD_ISSET(*it, &_writeset)) // поступили данные на отправку, отправляем
        {
            std::cout << "send2" << std::endl;
            errno = 0;
            send(*it, _startPage.c_str(), _startPage.size(), 0);//в другой if перенести
            std::cout << "Send" << std::endl;
            _clients_recv.insert(*it);
            FD_CLR(*it, &_writeset_master);
            FD_CLR(*it, &_readset_master);
            close(*it);
            _clients_send.erase(*it);
            ret = true;
            break;
        }
    }
    return(ret);
}

bool ServerEngine::ft_receive(){

    bool ret;

    ret = false;

    for(std::set<int>::iterator it = _clients_recv.begin(); it != _clients_recv.end(); it++)
    {
        std::cout << "recv " << *it << std::endl;
        if(FD_ISSET(*it, &_readset))
        {
            std::cout << "RECV" << std::endl;
            // Поступили данные от клиента, читаем их
            errno = 0;
            int bytes_read;
            bzero(_buf,65536);
            bytes_read = recv(*it, _buf, 65536, 0);
            std::string buffer = _buf;
            
            std::cout << "Read:"<< std::endl << buffer << std::endl << "Read END!"<< std::endl;

            Request request(buffer);
            request.parse_request();
            request.parse_headers();
            request.parse_body();

            _clients_send.insert(*it);
            FD_SET(*it, &_writeset_master);
            _clients_recv.erase(*it);
            ret = true;
            // if(bytes_read <= 0)
            // {
            //     // Соединение разорвано, удаляем сокет из множества
            //     close(*it);
            //     clients.erase(*it);
            //     continue;
            // }
            break;
        }
    }
    return(ret);
}

bool ServerEngine::ft_accept(int *mx){

    bool ret;

    ret = false;
    
    for (std::set<int>::iterator it = _listen_fds.begin(); it != _listen_fds.end(); ++it)
    {
        std::cout << "accept " << *it << std::endl;
        // Определяем тип события и выполняем соответствующие действия
        if(FD_ISSET(*it, &_readset))
        {
           // Поступил новый запрос на соединение, используем accept
           errno = 0;
           int sock = accept(*it, NULL, NULL);
           std::cout << "accept complete " << sock << std::endl;
           if(sock < 0)
            {
              perror("accept");
              exit(3);
            }
            if (*mx < sock)
                *mx = sock;
            fcntl(sock, F_SETFL, O_NONBLOCK);
            _clients_recv.insert(sock);
            FD_SET(sock, &_readset_master);
            ret = true;
            break;
        }
    }
    
    return(ret);
}

void		ServerEngine::run(void)
{
    int mx = *max_element(_listen_fds.begin(), _listen_fds.end());
    _clients_recv.clear();
    _clients_send.clear();
    FD_ZERO(&_readset_master); 
    FD_ZERO(&_writeset_master);

    getStartPage();

    for (std::set<int>::iterator it = _listen_fds.begin(); it != _listen_fds.end(); ++it)
        FD_SET(*it, &_readset_master);
    while(1)
    {
        std::cout << "start" << std::endl;
        // Заполняем множество сокетов
        timeval timeout;
        timeout.tv_sec = 15;
        timeout.tv_usec = 0;

        bool sel = true;
        while (sel) {
            // Ждём события в одном из сокетов
            int ret = ft_select(mx, timeout);
            if (ret > 0)
                sel = false;
        }

        if (!sel)
        {
            sel = ft_send(); // отправка данных клиенту
        }

        if (!sel)
        {
            sel = ft_receive(); // получение данные от клиента
        }

        if (!sel)
        {
            sel = ft_accept(&mx); // проверяем запросы на соединение
        }

        sel = true;
        std::cout << "end" << std::endl;
    }

}
