/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:09 by gforns-s          #+#    #+#             */
/*   Updated: 2025/02/17 21:34:35 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "gforns-s.hpp"
#include "rzhdanov.hpp"
//#include "Client.hpp"
//#include "Channel.hpp"


#include <string>
#include <exception>
#include <iostream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <map>
#include <sstream>

#include <cstdio> //perror

class Server
{
	private:
		int _port;
		std::string _pass;
		std::string _nick;
		std::string _user;
		bool auth;		//check if pass is ok
		bool	_reg;	//check if nick and user are stored

	public:
//Testing
		int server_fd;	//should go private --->> getter setter
		int client_fd;	//should go to a map container or client class
		struct sockaddr_in server_addr;	//should go private --->> getter setter
		struct sockaddr_in client_addr;	//should go to the client class or container
		socklen_t client_addr_len;// = sizeof(client_addr); 	//should go to client class or container
		
		int					send_out(std::string message);
		std::string 		to_lower(std::string &str);
		void				set_reg(bool i);
		bool				get_reg() const;

		
		Server();//start protected with a value?
		Server(std::string str, int nb);//dunno if it can be started without checking, added just in case
		~Server();
		Server(const Server &other);
		Server				&operator=(const Server &other);
		void				set_port(const int &nb);//setter
		void				set_pass(const std::string &str);//setter
		void				set_pass(const char *str);//setter with char*
		void				check_port(const std::string &str); //calls setter if all ok
		int					get_port() const;
		const std::string	get_nick() const;
		const std::string	get_name() const;
		void				set_nick(const std::string &str);
		void				set_user(const std::string &str);
		void				set_auth(bool i);
		bool				get_auth() const;
		bool 				check_pass(std::string &str);
		void				buff_to_string(char *str);

};


