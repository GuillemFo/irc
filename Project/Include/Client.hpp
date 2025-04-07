/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 11:35:59 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/07 10:06:51 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Channel.hpp"

//Client *clients[fd] = new Client(fd, nickname); !!!!!!!
class Client
{
	private:
		int 				_client_fd;
		std::string 		_nick;
		std::string 		_user;
		//maybe we have extra info apart from user, read irc man

		/////////////// This 2 down have shit names, pending to change for clarity
		bool 				_auth;	//check if pass is ok
		bool				_reg;	//check if nick and user are stored

	public:
		Client(int fd);	//to at least know who to send the errors
		~Client();
		Client(const Client &other);
		Client &operator=(const Client &other);

		std::string			_string_buff;	//store buffer until \r \n and permission to send. keep storing and remove the part sent. maybe need an int to check the lenght of the string so its easier to cut and to confirm from the server??

		int					get_clientFD();

		void				set_nick(const std::string &str);
		const std::string	get_nick() const;

		void				set_user(const std::string &str);
		const std::string	get_user() const;
		
		void				set_auth(bool i);
		bool				get_auth() const;
		
		void				set_reg(bool i);
		bool				get_reg() const;

};
