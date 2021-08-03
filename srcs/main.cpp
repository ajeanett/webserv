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

#include "Engine/ServerEngine.hpp"

int main()
{
//	std::string chunk;
//	for (size_t i = 0; i < 8000; ++i)
//		chunk += 'N';
//
//	displayTimeStamp();
//	std::cout << std::endl;
//	std::string body;
//	for (size_t i = 0; i < 100000000 / 8000; ++i)
//		body += chunk;
//	displayTimeStamp();
//	std::cout << body.length() << std::endl;
//
////	body[8000] = '\r';
////	body[8001] = '\n';
//	for (size_t i = 0; i < 100000000 / 8000; ++i)
//		body.find("\r\n");
//	displayTimeStamp();
//	std::cout << body.length() << std::endl;

	ServerEngine serv;
	serv.setup();
	serv.run();
	return (0);
}