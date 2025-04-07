/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:09 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/07 13:51:14 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "gforns-s.hpp"
#include "rzhdanov.hpp"

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
#include <fcntl.h>

#include <cstdio> //perror

#include "Colors.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Tools.hpp"


// Need to create a channel once we start the server.
// Need to give operator permissions to the first client it joins the server or how we stablish an op??
class Client;
class Channel;

class Server
{
	private:
		int									_sv_fd;
		std::string							_sv_pass;
		int									_port;
		std::string							_sv_name;
		std::map<int, Client*>				_cl_map;	// int = client_fd
		std::map<std::string , Channel*>	_ch_map;	//string = name of channel
		
		/////////////////////CLIENT//////////////////////////
		std::string _nick;
		std::string _user;
		bool auth;		//check if pass is ok
		bool	_reg;	//check if nick and user are stored
		/////////////////////////////////////////////////////


	public:
//Testing
		
		int					send_out(std::string message);

		Server(int sv_fd, std::string sv_pass, int port);
		~Server();
		Server(const Server &other);
		Server				&operator=(const Server &other);
		
		int					get_serverFD();
		
		int					get_port() const;
		
		bool 				check_pass(std::string &str);
		
		//Add client Remove client		// will do new[] and delete
		//Add channel Remove channel	// will do new[] and delete

		int					addClientMap(int fd);
		int					addChannelMap(std::string &str);	//thinking if i should start the channels with a default password like "" or something and then just use one constructor and destructor etc  07/04/25 12.30
		int					addChannelMap(std::string &str, std::string &pw);
		
		int					rmClientMap(int fd);
		int					rmChannelMap(std::string &str);	//thinking if i should start the channels with a default password like "" or something and then just use one constructor and destructor etc  07/04/25 12.30
		int					rmChannelMap(std::string &str, std::string &pw);

		void				set_server_name(const std::string &s);
		void				buff_to_string(char *str);
		void				command_list(std::string &str);
		void				welcome_msg(const std::string &nickname);

		/////////////////////CLIENT//////////////////////////
		int client_fd;
		struct sockaddr_in client_addr;
		socklen_t client_addr_len;// = sizeof(client_addr);
		void				set_reg(bool i);
		bool				get_reg() const;
		const std::string	get_nick() const;
		const std::string	get_name() const;
		void				set_nick(const std::string &str);
		void				set_user(const std::string &str);
		void				set_auth(bool i);
		bool				get_auth() const;
		/////////////////////////////////////////////////////


};


