/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 11:35:59 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/01 12:40:00 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Channel.hpp"

// should clients be init with new[]??
class Client
{
	protected:
		std::string 		_nick;
		std::string 		_user;
		int 				_client_fd;
		socklen_t 			_client_addr_len;
		struct sockaddr_in	_client_addr;
		bool 				_auth;		//check if pass is ok
		bool				_reg;	//check if nick and user are stored

	public:
		std::string			_string_buff;	//store buffer until \r \n and permission to send. keep storing and remove the part sent. maybe need an int to check the lenght of the string so its easier to cut and to confirm from the server??

		void				set_nick(const std::string &str);
		const std::string	get_nick() const;
		void				set_user(const std::string &str);
		const std::string	get_user() const;
		void				set_auth(bool i);
		bool				get_auth() const;
		void				set_reg(bool i);
		bool				get_reg() const;

};
