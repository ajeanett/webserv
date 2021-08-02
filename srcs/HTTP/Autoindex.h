//
// Created by Blood Marillion on 8/2/21.
//

#ifndef WEBSERV_AUTOINDEX_H
#define WEBSERV_AUTOINDEX_H

#include <iostream>

class Autoindex{
public:
    std::string directory;

    Autoindex(std::string const &dir);
    std::string get_html();
private:
    Autoindex();
};

#endif //WEBSERV_AUTOINDEX_H
