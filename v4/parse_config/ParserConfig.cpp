#include "ParserConfig.hpp"

ParserConfig::ParserConfig(std::string &configfile)
{
    Parser(configfile);
}

ParserConfig::~ParserConfig()
{
}

ParserConfig::ParserConfig(ParserConfig &cpy){
    *this = cpy;
}

ParserConfig	&ParserConfig::operator=(const ParserConfig &cpy){
    this->_servers = cpy._servers;
    return (*this);
}

std::map<int, ServerData> &ParserConfig::getServers(){
    return (this->_servers);
}

void    ParserConfig::check_brackets(std::string buffer)
{
    if ((buffer.find("{", 0) != std::string::npos) && server_open == false)
        server_open = true;
    else if (((buffer.find("{", 0) != std::string::npos) && server_open == true))
        loc_open = true;
    if (((buffer.find("}", 0) != std::string::npos) && loc_open == true))
    {
        loc_open = false;
    }
    else if (((buffer.find("}", 0) != std::string::npos) && loc_open == false))
    {
        server_open = false;
    }
}

void        ParserConfig::fillServerData(ServerData &s, std::string &buffer){
    size_t next = 0;
    if ((next = buffer.find("port ", 0)) != std::string::npos)
    {
            next += 5;
            s.setPort(atoi((buffer.substr(next, (buffer.find(";") - next)).c_str()))); // ищем точку с запятой, вырезаем от пробела до точи с запятой, коертируем в строку си, переводим в инт и записываем в значение порта.
    }
    else if ((next = buffer.find("host ", 0)) != std::string::npos)
    {
        next += 5;
        s.setHost(buffer.substr(next, (buffer.find(";") - next)));
    }
    else if ((next = buffer.find("server_name ", 0)) != std::string::npos)
    {
        next += 12;
        s.setServerName(buffer.substr(next, (buffer.find(";") - next)));
    }
}

LocationData        ParseLocations(std::ifstream &configfile, std::string &buffer, size_t next)
{
    LocationData l;

    l.setLocationPath(buffer.substr(next, buffer.rfind(" ") - next)); //
    while(getline(configfile, buffer))
    {
        if (buffer.find("}") != std::string::npos)
        {
                if (!l.getLocationPath().empty() && !l.getRoot().empty() && l.getFullPath().empty())
                {
                        l.setFullPath(l.getRoot(), l.getLocationPath());
                }
            return (l);
        }
        if ((next = buffer.find("root ") != std::string::npos))
        {
            next += 5;
            l.setRoot(buffer.substr(next, buffer.rfind(";") - next));
        }
        else if ((next = buffer.find("\tindex ")) != std::string::npos)
        {
            next += 6;
            l.setIndex(buffer.substr(next, buffer.rfind(";") - next));
        }
        else if ((next = buffer.find("method ")) != std::string::npos)
        {
            next += 7;
            l.setMethods(buffer.substr(next, buffer.rfind(";") - next));
        }
        else if ((next = buffer.find("cgi_path ")) != std::string::npos)
        {
            next += 9;
            l.setCgiPath(buffer.substr(next, buffer.rfind(";") - next));
        }
        else if ((next = buffer.find("redirect ")) != std::string::npos)
        {
            next += 9;
            l.setRedirect(buffer.substr(next, buffer.rfind(";") - next));
        }
        else if ((next = buffer.find("cgi_extension ")) != std::string::npos)
        {
            next += 14;
            l.setCgiExtension(buffer.substr(next, buffer.rfind(";") - next));
        }
        else if ((next = buffer.find("client_body_buffer_size ")) != std::string::npos)
        {
            next += 24;
            l.setClientBufferBodySize(atoi((buffer.substr(next, buffer.rfind(";") - next)).c_str()));
        }
        else if ((next = buffer.find("autoindex ")) != std::string::npos)
        {
            next += 10;
            l.setAutoindex((buffer.substr(next, buffer.rfind(";") - next) == "on"));
        }

    }
    // std::cout << "I`m here!!!1" << std::endl;
    // if (!l.getLocationPath().empty() && !l.getRoot().empty() && l.getFullPath().empty())
    // {
    //         std::cout << "I`m here!!!2" << std::endl;
    //         l.setFullPath(l.getRoot(), l.getLocationPath());
    // }
    return (l);
}

ServerData ParserConfig::ParseServer(std::ifstream &configfile, std::string &buffer){
    ServerData s;
    size_t next;

    while(getline(configfile, buffer))
    {
        if (buffer.find("}") != std::string::npos)
        {
            server_open = false;
            return (s);
        }
        fillServerData(s, buffer);
        if ((next = buffer.find("location ")) != std::string::npos)
        {
            s.getLocationData().push_back(ParseLocations(configfile, buffer, next + 9));
        }
    }
    return (s);
}

void    ParserConfig::Parser(std::string name){

    server_open = false;
    loc_open = false;
    std::ifstream configfile(name);
    std::string buffer;
    size_t  next = 0;
    bool    server_find = false;
    serv_num = 0;

    if (!configfile)
	{
		std::cerr << "Uh oh, " << name << " config file could not be opened for reading!" << std::endl;
        exit(1);
	}
    while (getline(configfile, buffer))
    {
        next = buffer.find("server ", 0);
        if (next  != std::string::npos)
        {
            server_find = true;
        }
        if (server_find)
        {
            check_brackets(buffer);
            if (server_open)
            {
                _servers.insert(std::pair<int, ServerData>(serv_num, ParseServer(configfile, buffer)));
                serv_num += 1;
                server_find = false;
            }
        }
        

    }
}

// std::ostream & operator<<(std::ostream & o, ParserConfig const & i)
// {
    // i.
    // o << "PARSED CONFIG" << std::endl;
    // // int m = 0;
    
    // // i.getServers()[i].getPort();
    // // m++;
    // // o << i.getServers()[0];
    // return o;
// }



// void	ConfigServers::add(std::string fileName) {
// 	std::string str;
// 	std::ifstream file(fileName);
// 	if (file.is_open())
// 	{
// 		while (!file.eof())
// 		{
// 			getline (file,str);
// 			trim(str);
// 			if (str.find("server", 0, 6) != std::string::npos && (!str[6] || isspace(str[6]))) {
// 				this->_serv.push_back(_parse_server(file, str));
// 			}
// 		}
// 		file.close();
// 	}
// }


    // while (getline(ifs, content))
	// {
	// 	find = content.find(src);
	// 	if (i)
	// 		ofs << std::endl;
	// 	while (find != std::string::npos)
	// 	{
	// 		content.replace(content.find(src), src.size(), dst);
	// 		find = content.find(src, find + src.length());
	// 	}
	// 	i++;
	// 	ofs << content ;
	// }


//     bool    ServerEngine::check_request(std::string buffer, int fd){



//     // std::string s = "IaFFSjndsUFfE";
//     // std::transform(s.begin(), s.end(), s.begin(), tolower);
//     std::transform(buffer.begin(), buffer.end(), buffer.begin(), tolower);
//     // std::cout << s << std::endl;


//     size_t prev = 0;
//     size_t next = 0;
//     std::string     tmp;

//     next = buffer.find("\r\n\r\n", prev);
//     //проверить \r\n\r\n, затем проверяем наличие content-lenght, если число равно боди. Нужно будет актуальный размер боди сравнивать с этим числом.
//     //Если content-lenght нет, то проверить transfer encoding и равен ли он chunked. Если равен, то дождаться, когда придет "0\r\n\r\n" (неважно, в конце боди или нет, важно, чтобы после первого \r\n\r\n)
//     // Если нету ни content length или transfer encoding и есть \r\n\r\n, то значит запрос пришел полностью
//     // std::string buffer;

//     if (next  != std::string::npos)
//     {
//         if (buffer.find("0\r\n\r\n", prev) != std::string::npos)
//                         return (true);
//         int body_size = atoi((buffer.substr(next + 4)).c_str());
//         next = buffer.find("content-length: ", prev); //проверяем наличие content-lenght
//         if (next  != std::string::npos){
//             prev = next;
//             next = buffer.find("\r\n", prev);
//             if (next  != std::string::npos)
//             {
//                 tmp = buffer.substr(prev, next - prev);
//                 int content_lenght = atoi(tmp.c_str());
//                 return (body_size == content_lenght);
//             }
//         }
//         prev = 0;
//         next = buffer.find("transfer-encoding: ", prev); // проверяем transfer-encoding, не является ли запрос фрагментированным
//         if (next  != std::string::npos){
//             prev = next;
//             next = buffer.find("\r\n", prev);
//             if (next  != std::string::npos)
//             {
//                 tmp = buffer.substr(prev, next - prev);
//                 // trim(tmp); //утонить, нужно ли обрезать пробелы и табуляцию по краям. upd. Уточнил - не нужно.
//                 if (tmp == "chunked")
//                 {
//                     _chunked[fd] = true;
//                     if (buffer.find("0\r\n\r\n", prev) != std::string::npos)
//                         return (true);
//                     return (false);
//                 }
//             }
//         }
//         return (true); // есть \r\n\r\n , нет transfer-encoding и content-lenght, запрос пришел полностью
//     }
//     return (false); // проверить ошибки 404, 500, 505
// }