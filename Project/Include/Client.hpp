/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 11:35:59 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/22 14:12:23 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Channel.hpp"


#define NICK_NOT_ALLOWED_CHARS " ,*?!@" // nick can't cotain this chars
#define NICK_NOT_STARTING "$:"
#define PREFIX "@" // the only channel membership allowed would be operator
					// the char . should not be included in nickname
#define USERLEN	64 //length of username in USER message 
#define USER_NOT_ALLOWED_CHARS " \0\r\n"

#define NICKLEN	31 // MUST BE specified, 30 or 31 are typical values 

class Server ;
class Channel ;

class Client
{
	private:
		Server *_server; // to get the details and functions of the server
		int 				_client_fd;
		std::string 		_nick;
		std::string 		_user;
		std::string 		_realname;
		bool				_passok;	//password received and OK
		bool				_registered; //registration process OK
		std::map<std::string, Channel*> _channels; //list of channels joined


	public:
		Client(int fd);	//to at least know who to send the errors
		Client(Server *server, int _client_fd); 
		~Client();
		Client(const Client &other);
		Client &operator=(const Client &other);

		std::string			_string_buff;	//store buffer until \r \n and permission to send. keep storing and remove the part sent. maybe need an int to check the lenght of the string so its easier to cut and to confirm from the server??

		int					get_clientFD();

		void				set_nick(const std::string &str);
		const std::string	get_nick() const;

		void				set_user(const std::string &str);
		void				setRealName(std::string &str);
		const std::string	get_user() const;
		
		void	setPassOK(); //set to true. In the constructor would be false
		void	setRegistered(); //set to true. In the constructor would be false
		void	joinChannel(std::string channelName);
		void	partChannel(std::string channelName);
		
		//getters
		std::string	getLowerNick();
		std::string	getSource(); // : <nickname> [ "!" <user> ] [ "@" <host> ]
		bool		isRegistered();
		
	

};
