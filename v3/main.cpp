/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeanett <ajeanett@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/18 20:32:21 by ajeanett          #+#    #+#             */
/*   Updated: 2021/07/20 11:55:06 by ajeanett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server_engine/Webserv_main.hpp"
#include "./server_engine/Request.hpp"
#include "./server_engine/ServerEngine.hpp"

// class Clients;

int main(void)
{
    ServerEngine serv;
    serv.servStart();
    // serv.getStartPage();
    serv.run();

    return (0);
}