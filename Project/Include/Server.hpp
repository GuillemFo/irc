/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:09 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/29 21:09:58 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <exception>
#include <iostream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <map>
#include <vector>
#include <sstream>
#include <fcntl.h>
#include <errno.h>

#include <cstdio> //perror

#include "Colors.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Codes.hpp"
#include "Tools.hpp"
#include "Command.hpp"
#include "CommandHandler.hpp"
#include "CommandDispatcher.hpp"
#include "Parser.hpp"
#include "PrivmsgCommand.hpp"
#include "JoinCommand.hpp"
#include "NickCommand.hpp"
#include "PassCommand.hpp"
#include "UserCommand.hpp"
#include "CapCommand.hpp"
#include "PingCommand.hpp"
#include "PartCommand.hpp"
#include "TopicCommand.hpp"



#define HOLD_NON_ACCEPTED 10 //this will set the ammount of connections in hold before start rejecting them if they are not accepted.

// Need to create a channel once we start the server.
// Need to give operator permissions to the first client it joins the server or how we stablish an initial op??
class Client;
class Channel;

class Server
{
	private:
		int									_sv_fd;
		int									_epoll_fd;
		int									_port;
		std::string							_sv_pass;
		std::string							_sv_name;
		std::map<int, Client*>				_cl_map;	// int = client_fd
		std::map<std::string , Channel*>	_ch_map;	//string = name of channel
		//**faltaria timestamp d'inici, pel RPL_STATSUPTIME
		//**server version and MOTD
		//**LOC1, LOC2, ADMINEMAIL per les seve replies
	public:
		CommandDispatcher					_dispatcher;
//Testing
		
		int					send_out(std::string message);

		Server(int sv_fd, int port, std::string sv_pass);
		~Server();
		Server(const Server &other);
		Server				&operator=(const Server &other);
		
		int					get_serverFD();

		void				set_epollFD(int nb);
		int					get_epollFD();
		
		int					get_port() const;
		
		bool 				check_pass(const std::string &str);
		
		//Add client Remove client		// will do new[] and delete
		//Add channel Remove channel	// will do new[] and delete


		const std::map<int, Client*>	&getClientMap() const;
		const std::map<std::string, Channel*>	&getChannelMap() const;
		
		int					addClientMap(int fd);
		int					addChannelMap(const std::string &str);	//thinking if i should start the channels with a default password like "" or something and then just use one constructor and destructor etc  07/04/25 12.30
		int					addChannelMap(const std::string &str, const std::string &pw); // the pw will be in the channel object
		
		int					rmClientMap(int fd);
		int					rmChannelMap(std::string &str);	//thinking if i should start the channels with a default password like "" or something and then just use one constructor and destructor etc  07/04/25 12.30
		int					rmChannelMap(std::string &str, std::string &pw); // josegar2: the channels aren't removed

		bool				nickExists(const std::string &theNick);
		bool				channelExists(const std::string &theChannel);
		Channel				*getChannel(const std::string &theChannel);
		Client			*getClient(const int &fd);
		Client			*getClientByNick(const std::string &s);
		
		void				set_server_name(const std::string &s);
		std::string			getServerName() const;
		void				buff_to_string(char *str);
		void				command_list(std::string &str);
		void				welcome_msg(const std::string &nickname);
		void				registerAllCommands();

};


