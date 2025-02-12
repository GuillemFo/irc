/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/02/12 12:10:11 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


/*
Your executable will be run as follows:
./ircserv <port> <password>
*/
int main(int ac, char **av)
{
	Server Server;
	try
	{
		if (ac != 3)
			throw ("Wrong arguments");
		Server.check_port(av[1]);
		Server.set_pass(av[2]);
	}
	catch(const std::string e)
	{
		std::cout << e << '\n';
	}
	
}