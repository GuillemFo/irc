/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:09 by gforns-s          #+#    #+#             */
/*   Updated: 2025/02/12 12:59:48 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <exception>
#include <iostream>
#include <sstream>
#include <cctype>
#include <cstdlib>

class Server
{
	private:
		int _port;
		std::string _pass;
	public:
		Server();//start protected with a value?
		Server(std::string str, int nb);//dunno if it can be started without checking, added just in case
		~Server();
		Server(const Server &other);
		Server	&operator=(const Server &other);
		void	set_port(const int &nb);//setter
		void	set_pass(const std::string &str);//setter
		void	check_port(const std::string &str); //calls setter if all ok
		
};


