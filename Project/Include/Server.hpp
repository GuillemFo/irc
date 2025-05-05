/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:09 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/05 14:23:43 by gforns-s         ###   ########.fr       */
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
#include "QuitCommand.hpp"
#include "TopicCommand.hpp"
#include "InviteCommand.hpp"
#include "ModeCommand.hpp"
#include "KickCommand.hpp"
#include "Tools.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "PrivmsgCommand.hpp"
#include "Tools.hpp"
#include "Colors.hpp"
#include "JoinCommand.hpp"
#include "Parser.hpp"
#include "Command.hpp"
#include "CommandDispatcher.hpp"
#include <iostream>
#include <csignal>



#define HOLD_NON_ACCEPTED 100 //this will set the ammount of connections in hold before start rejecting them if they are not accepted.

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
		std::map<int, Client*>				_cl_map;
		std::map<std::string , Channel*>	_ch_map;
		//**faltaria timestamp d'inici, pel RPL_STATSUPTIME
		//**server version and MOTD
		//**LOC1, LOC2, ADMINEMAIL per les seve replies
	public:
		CommandDispatcher					_dispatcher;
		
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


		const std::map<int, Client*>	&getClientMap() const;
		const std::map<std::string, Channel*>	&getChannelMap() const;
		
		int					addClientMap(int fd);
		int					addChannelMap(const std::string &str);
		int					addChannelMap(const std::string &str, const std::string &pw);
		
		int					rmClientMap(int fd);
		int					rmChannelMap(std::string &str);
		int					rmChannelMap(std::string &str, std::string &pw);

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
		void				printAllClientsInfo() const;
};


