/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 11:35:59 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/29 01:17:19 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdexcept>
#include "Server.hpp"
#include "Channel.hpp"
#include "OutBuffer.hpp"
#include "InBuffer.hpp"



#define NICK_NOT_ALLOWED_CHARS " ,*?!@" // nick can't cotain this chars
#define NICK_NOT_STARTING "$:"
#define PREFIX "@" // the only channel membership allowed would be operator
					// the char . should not be included in nickname
#define USERLEN	64 //length of username in USER message 
#define USER_NOT_ALLOWED_CHARS " \0\r\n"

#define NICKLEN	9 // MUST BE specified, 30 or 31 are typical values 

class Server ;
class Channel ;

class Client
{
	private:
		Server 				*_server; // to get the details and functions of the server
		int 				_client_fd;
		std::string 		_nick;
		std::string 		_user;
		std::string 		_realname;
		std::string			_host;		// or IP Maximum 63 chars. If longer --> IP
		bool				_passok;	//password received and OK
		bool				_registered; //nick OK
		bool				_okLogin;	// pass ok and nick ok
		std::map<std::string, Channel*> _channels; //list of channels joined
		
		
	public:
		// made public to be able to call its member functions
		OutBuffer			_out;
		InBuffer			_in;

		Client(int fd);	//to at least know who to send the errors
		Client(Server *server, int _client_fd); 
		~Client();
		Client(const Client &other);
		Client &operator=(const Client &other);

		std::string			_string_buff;

		int					get_clientFD();

		Server				*getServer() const;

		void				set_nick(const std::string &str);
		const std::string	get_nick() const;

		void				set_user(const std::string &str);
		void				setRealName(const std::string &str);
		const std::string	get_user() const;
	
		void				set_host(const std::string &str);
		const std::string	get_host() const;

		void	setPassOK();
		bool	getPassOK();
		void	setRegistered();
		void	setOkLogin();
		bool	getOkLogin();
		void	addChannel(Channel *pChannel);
		void	remChannel(const std::string &theChannel);
		void	joinChannel(const std::string &channelName);
		void	joinChannel(const std::string &channelName, const std::string &channelPwd);
		void	partChannel(const std::string &channelName);
		void	partAllChannels();
		
		std::string	getLowerNick();
		std::string	getClient(); // <nick> [ "!" <user> ] [ "@" <host> ]
		std::string	getSource(); // :<nick> [ "!" <user> ] [ "@" <host> ]
		bool		isRegistered();

		void		sendMessage(const std::string &theMessage);
		void		changeAllNicks(const std::string &oldNick);
		void		sendAllChannels(const std::string &theMessage);

		void		addOutMessage(const std::string& message);
		std::string	getOutMessage();
		void		popCharsSent(size_t bytesSent);
		bool		isOutEmpty() const;
		void		clearOutBuffer();
		const OutBuffer& getOutBuffer() const;
		void		cl_Epoll_In_Out();
		void		cl_Epoll_In();
		std::map<std::string, Channel*> &getChannels();
		
};
