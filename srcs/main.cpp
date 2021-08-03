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
	ServerEngine serv;
	serv.setup();
	serv.run();
	return (0);
}