#include "CGI.hpp"

CGI::CGI(/* args */)
{
    /* environment variables for CGI */
    _tmpEnvCGI["SERVER_SOFTWARE"] = "Webserv by kfriese and ajeanett";
    _tmpEnvCGI["SERVER_NAME"];
    _tmpEnvCGI["GATEWAY_INTERFACE"];
    _tmpEnvCGI["SERVER_PROTOCOL"];
    _tmpEnvCGI["SERVER_PORT"];
    _tmpEnvCGI["REQUEST_METHOD"];
    _tmpEnvCGI["PATH_INFO"];
    _tmpEnvCGI["PATH_TRANSLATED"];
    _tmpEnvCGI["SCRIPT_NAME"];
    _tmpEnvCGI["QUERY_STRING"];
    _tmpEnvCGI["REMOTE_HOST"];
    _tmpEnvCGI["REMOTE_ADDR"];
    _tmpEnvCGI["AUTH_TYPE"];
    _tmpEnvCGI["REMOTE_USER"];
    _tmpEnvCGI["REMOTE_IDENT"];
    _tmpEnvCGI["CONTENT_TYPE"];
    _tmpEnvCGI["CONTENT_LENGTH"];
    _tmpEnvCGI["HTTP_ACCEPT"];
    _tmpEnvCGI["HTTP_USER_AGENT"];
}

CGI::CGI(Request &req){
     /* environment variables for CGI */
    _tmpEnvCGI["SERVER_SOFTWARE"] = "Webserv by kfriese and ajeanett";
    _tmpEnvCGI["SERVER_NAME"];
    _tmpEnvCGI["GATEWAY_INTERFACE"];
    _tmpEnvCGI["SERVER_PROTOCOL"];
    _tmpEnvCGI["SERVER_PORT"];
    _tmpEnvCGI["REQUEST_METHOD"];
    _tmpEnvCGI["PATH_INFO"];
    _tmpEnvCGI["PATH_TRANSLATED"];
    _tmpEnvCGI["SCRIPT_NAME"];
    _tmpEnvCGI["QUERY_STRING"];
    _tmpEnvCGI["REMOTE_HOST"];
    _tmpEnvCGI["REMOTE_ADDR"];
    _tmpEnvCGI["AUTH_TYPE"];
    _tmpEnvCGI["REMOTE_USER"];
    _tmpEnvCGI["REMOTE_IDENT"];
    _tmpEnvCGI["CONTENT_TYPE"];
    _tmpEnvCGI["CONTENT_LENGTH"];
    _tmpEnvCGI["HTTP_ACCEPT"];
    _tmpEnvCGI["HTTP_USER_AGENT"];
    _current_root = "";
}

CGI::~CGI()
{
    
}

int     CGI::clearCGI(){

    _tmpEnvCGI["SERVER_NAME"].clear();
    _tmpEnvCGI["GATEWAY_INTERFACE"].clear();
    _tmpEnvCGI["SERVER_PROTOCOL"].clear();
    _tmpEnvCGI["SERVER_PORT"].clear();
    _tmpEnvCGI["REQUEST_METHOD"].clear();
    _tmpEnvCGI["PATH_INFO"].clear();
    _tmpEnvCGI["PATH_TRANSLATED"].clear();
    _tmpEnvCGI["SCRIPT_NAME"].clear();
    _tmpEnvCGI["QUERY_STRING"].clear();
    _tmpEnvCGI["REMOTE_HOST"].clear();
    _tmpEnvCGI["REMOTE_ADDR"].clear();
    _tmpEnvCGI["AUTH_TYPE"].clear();
    _tmpEnvCGI["REMOTE_USER"].clear();
    _tmpEnvCGI["REMOTE_IDENT"].clear();
    _tmpEnvCGI["CONTENT_TYPE"].clear();
    _tmpEnvCGI["CONTENT_LENGTH"].clear();
    _tmpEnvCGI["HTTP_ACCEPT"].clear();
    _tmpEnvCGI["HTTP_USER_AGENT"].clear();
    _current_root.clear();
	return (0);
}

void    CGI::fillTmpEnvCgi(const Request &req, ServerData & serv){

    std::string curr_loc_str;
    std::vector<LocationData> locs;
    std::vector<LocationData>::iterator it;

    curr_loc_str = req.getLocation();
    locs = serv.getLocationData();

    _tmpEnvCGI["SERVER_NAME"] = serv.getServerName();
    _tmpEnvCGI["GATEWAY_INTERFACE"] = "CGI/1.1";
    _tmpEnvCGI["SERVER_PROTOCOL"] = req.getVersion();
    _tmpEnvCGI["SERVER_PORT"] = std::to_string(serv.getPort());
    _tmpEnvCGI["REQUEST_METHOD"] = req.getMethod();
    _tmpEnvCGI["PATH_INFO"] = req.getLocation();
    for (it = locs.begin(); it != locs.end(); ++it)
    {
        if ((*it).getLocationPath() == curr_loc_str)
        {
            _tmpEnvCGI["PATH_TRANSLATED"] = (*it).getRoot();
            _current_root = (*it).getRoot();
            _tmpEnvCGI["SCRIPT_NAME"] = !(*it).getCgiPath().empty() ? (*it).getCgiPath() : "" ;
            _cgi_type = (*it).getCgiExtension();
        }
    }
        _tmpEnvCGI["QUERY_STRING"] = "";
//        _tmpEnvCGI["REMOTE_HOST"] = req.getHeaders().find("Referer") == req.getHeaders().end() ? "" : req.getHeaders().find("Content-Type")->second;
        _tmpEnvCGI["REMOTE_ADDR"] = "";
        _tmpEnvCGI["AUTH_TYPE"] = "BASIC";
        _tmpEnvCGI["REMOTE_USER"] = "User";
        // _tmpEnvCGI["REMOTE_IDENT"] = "";
        _tmpEnvCGI["CONTENT_TYPE"] = req.getHeaders().find("Content-Type") == req.getHeaders().end() ? "" : req.getHeaders().find("Content-Type")->second;
        _tmpEnvCGI["CONTENT_LENGTH"] = std::to_string(req.getBody().size());
        _tmpEnvCGI["HTTP_ACCEPT"] =  req.getHeaders().find("Accept") == req.getHeaders().end() ? "" : req.getHeaders().find("Accept")->second;
        _tmpEnvCGI["HTTP_USER_AGENT"] = req.getHeaders().find("User-Agent") == req.getHeaders().end() ? "" : req.getHeaders().find("User-Agent")->second;
    
}

void    CGI::fillEnvp(char *** envp){

    *envp = new char* [_tmpEnvCGI.size() + 1];
	int	i = 0;
    //Заплняем переменные окружения в char** из мапы, которую мы создали ранее
	for (std::map<std::string, std::string>::iterator it = _tmpEnvCGI.begin(); it != _tmpEnvCGI.end(); ++it) {
		std::string	var = it->first + "=" + it->second; //каждая строка в переменной окружения должна быть вида key=value
		(*envp)[i] = new char[var.size() + 1];
		std::strcpy((*envp)[i], var.c_str());
		i++;
	}
	(*envp)[i] = NULL; //зануляем двухмерный массив

    return ;
}

void    CGI::prepareEnvCGI(const Request &req, ServerData & serv, char *** envp){

    clearCGI(); //очищаем предыдущие данные перед повторным использованием;
    fillTmpEnvCgi(req, serv);
    fillEnvp(envp);
}

// void    CGI::startCGI(char *** envp){

// }


std::string CGI::runCGI(const Request &req, ServerData & serv){

    char **envp = nullptr;
    std::string _ret; // возвращаемая строка;
    prepareEnvCGI(req, serv, &envp);
    // startCGI(&envp);


    pid_t _pid;
    int _status;
//    int _fd[2];

	std::string cgi_tmp_path_in = "./cgi_tmp_in";
	std::string cgi_tmp_path_out = "./cgi_tmp_out";
	/* tmp_file_fd_in - временнй файл для сохранения body запроса, для обработки этих данныхв CGI*/
	int tmp_file_fd_in = open(cgi_tmp_path_in.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0777);
	/* tmp_file_fd_out - временнй файл для сохранения результата выполнения CGI*/
	int tmp_file_fd_out = open(cgi_tmp_path_out.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0777);
	write(tmp_file_fd_in, req.getBody().c_str(), req.getBody().length());
	lseek(tmp_file_fd_in, 0, SEEK_SET);
//    pipe(_fd);

    _pid = fork();

    if (_pid == -1)
    {
        perror("Fork error\n");
    } 
    else if (_pid == 0) {
//		std::cout << "START CHILD PROCESS" << std::endl;
		dup2(tmp_file_fd_in, STDIN_FILENO);
		dup2(tmp_file_fd_out, STDOUT_FILENO);
		// сохранить текущий fd
		// dup 2 подменить fd на другие, возможно создать врем файл
		// записть в файл результат скрипта
		// считать результат в
		char **arg = new char*[3];
		std::string path = _current_root + _tmpEnvCGI["SCRIPT_NAME"]; // испрвить под конкретную реализацию, здесь длолжен быть путь к исполняемому файлу
		_cgi_type = "php"; // для тестирования
		if (_cgi_type == "py")
		{
			arg[0] = const_cast<char *>("/usr/local/bin/python3"); //strdup(path.c_str());
			arg[1] = const_cast<char *>("./python/test.py"); //strdup(_tmpEnvCGI["PATH_TRANSLATED"].c_str());
			arg[2] = NULL;
		}
		else if (_cgi_type == "php")
		{
			/* Заносим в arg значения для скрипта  php*/
			arg[0] = const_cast<char *>("/usr/bin/php"); //strdup(path.c_str());
			arg[1] = const_cast<char *>("./python/test.php"); //strdup(_tmpEnvCGI["PATH_TRANSLATED"].c_str());
			arg[2] = NULL;
		}
		else
		{
			/* выполняем cgi_tester */
		}
//		std::cout << "path to execve program = " << arg[0] << std::endl; // какую программу выполняем
//		std::cout << "execve first argument path = " << arg[1] << std::endl; // какой файл\скрипт выполняем с помощью программы
		if (execve(arg[0], arg, envp) == -1)
		{
			std::cerr << "ERROR CGI" << std::endl;
		}
		exit(0);
    }

    waitpid(_pid, &_status, 0);

	struct stat file;
	fstat(tmp_file_fd_out, &file);
	lseek(tmp_file_fd_out, 0, SEEK_SET);
	_ret.resize(file.st_size);
	read(tmp_file_fd_out, const_cast<char *>(_ret.c_str()), _ret.capacity());
	close(tmp_file_fd_out);
	close(tmp_file_fd_in);
	remove(cgi_tmp_path_in.c_str());
	remove(cgi_tmp_path_out.c_str());

//    char buf[1000];
//    bzero(buf, 1000);
//
//    size_t _read;
//    close(_fd[1]);
//    while ((_read = read(_fd[0], buf, 1000)) > 0)
//    {
//        _ret += buf;
//    }
//    close(_fd[0]);

    if (envp) 
    {
        for (int i = 0; envp[i] != NULL; ++i) 
        {
          delete[] envp[i];
        }
        delete[] envp;
        envp = NULL;
    }

    return _ret;
}

// Specification
// The following environment variables are not request-specific and are set for all requests:

// SERVER_SOFTWARE
// The name and version of the information server software answering the request (and running the gateway). Format: name/version

// SERVER_NAME
// The server's hostname, DNS alias, or IP address as it would appear in self-referencing URLs.

// GATEWAY_INTERFACE
// The revision of the CGI specification to which this server complies. Format: CGI/revision

// The following environment variables are specific to the request being fulfilled by the gateway program:
// SERVER_PROTOCOL
// The name and revision of the information protcol this request came in with. Format: protocol/revision

// SERVER_PORT
// The port number to which the request was sent.

// REQUEST_METHOD
// The method with which the request was made. For HTTP, this is "GET", "HEAD", "POST", etc.

// PATH_INFO
// The extra path information, as given by the client. In other words, scripts can be accessed by their virtual pathname, followed by extra information at the end of this path. The extra information is sent as PATH_INFO. This information should be decoded by the server if it comes from a URL before it is passed to the CGI script.

// PATH_TRANSLATED
// The server provides a translated version of PATH_INFO, which takes the path and does any virtual-to-physical mapping to it.

// SCRIPT_NAME
// A virtual path to the script being executed, used for self-referencing URLs.

// QUERY_STRING
// The information which follows the ? in the URL which referenced this script. This is the query information. It should not be decoded in any fashion. This variable should always be set when there is query information, regardless of command line decoding.

// REMOTE_HOST
// The hostname making the request. If the server does not have this information, it should set REMOTE_ADDR and leave this unset.

// REMOTE_ADDR
// The IP address of the remote host making the request.

// AUTH_TYPE
// If the server supports user authentication, and the script is protects, this is the protocol-specific authentication method used to validate the user.

// REMOTE_USER
// If the server supports user authentication, and the script is protected, this is the username they have authenticated as.

// REMOTE_IDENT
// If the HTTP server supports RFC 931 identification, then this variable will be set to the remote user name retrieved from the server. Usage of this variable should be limited to logging only.

// CONTENT_TYPE
// For queries which have attached information, such as HTTP POST and PUT, this is the content type of the data.

// CONTENT_LENGTH
// The length of the said content as given by the client.

// In addition to these, the header lines recieved from the client, if any, are placed into the environment with the prefix HTTP_ followed by the header name. Any - characters in the header name are changed to _ characters. The server may exclude any headers which it has already processed, such as Authorization, Content-type, and Content-length. If necessary, the server may choose to exclude any or all of these headers if including them would exceed any system environment limits.
// An example of this is the HTTP_ACCEPT variable which was defined in CGI/1.0. Another example is the header User-Agent.

// HTTP_ACCEPT
// The MIME types which the client will accept, as given by HTTP headers. Other protocols may need to get this information from elsewhere. Each item in this list should be separated by commas as per the HTTP spec.

// Format: type/subtype, type/subtype

// HTTP_USER_AGENT
// The browser the client is using to send the request. General format: software/version library/version.

// /usr/bin/python