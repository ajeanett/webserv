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

#include "./server_engine/Webserv_main.hpp"
#include "HTTP/Request.hpp"
#include "./server_engine/ServerEngine.hpp"
#include "./server_engine/Date.hpp"

int main()
{
	ServerEngine serv;
//	Date d;
//	std::cout << d.get_time() << std::endl;
    serv.servStart();
//	serv.getStartPage();
	serv.run();

	return (0);
}