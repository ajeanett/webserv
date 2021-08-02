/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeanett <ajeanett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/18 20:32:21 by ajeanett          #+#    #+#             */
/*   Updated: 2021/07/27 16:35:01 by ajeanett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTP/Request.hpp"
#include "./Engine//ServerEngine.hpp"
#include "Utils/Date.hpp"
#include "HTTP/Autoindex.h"
#include <dirent.h>


std::string get_dir_files(char* dir)
{

	std::string autoIndexBegin = "<!DOCTYPE html>\n"
								 "<html>\n"
								 "<head>\n"
								 "<title>Index of /</title></head>\n"
								 "<body bgcolor=\"white\">\n"
								 "<h1>Index of /</h1>\n"
								 "<hr>\n"
								 "<pre>\n";

	std::string autoIndexEnd = "</pre>\n"
							   "<hr>\n"
							   "</body>\n"
							   "</html>\n";

	struct dirent *dir_point;
	DIR    *dir_descript;
	std::string     str;
	std::string     file_name;

	dir_descript = opendir(dir);
	if (dir_descript == NULL)
		return ("ERROR");
	str = "";
	str+= autoIndexBegin;
	while ((dir_point = readdir(dir_descript)))
	{

		if (dir_point->d_type != DT_DIR)
		{
			file_name = dir_point->d_name;
		}
		else
		{
			file_name = dir_point->d_name;
			file_name += "/";
		}
		str+="     <a href=\"";
		str+=file_name;
		str+="\">";
		str+=file_name;
		str+="</a>";


//        if (dir_point->d_type == DT_DIR)
//        {
//            printf("dir: %s",dir_point->d_name);
//        }
//        else if (dir_point->d_type)
//        {
//            printf(dir_point->d_name);
//        }
//        printf("\n");
	}

	str+= autoIndexEnd;
	closedir(dir_descript);
	return (str);
}

int main()
{
    ServerEngine serv;
//	Date _d;
//	std::cout << _d.get_time() << std::endl;
  serv.servStart();
//	serv.getStartPage();
	serv.run();

//    Autoindex a = Autoindex("/Users/bmarilli/Desktop/team_web/srcs/HTTP");
//    std::cout << a.get_html();


	return (0);
}