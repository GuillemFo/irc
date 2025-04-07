/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 11:34:54 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/07 12:31:43 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#include "Client.hpp"
#include <set>	//to store client fd or nick and operators

class Client ;

//Channel *channels[channelName] = new Channel(channelName); !!!!!!!
class Channel
{
	private:
		std::string		_chName;
		std::string		_chTopic;
		std::string		_chPass;	// if empty no pass need?
		std::set<int>	_clients;
		std::set<int>	_operators;	//might change to strigns of nicks.
		bool			_inviteOnly;
		int				_userLimit;
		
	public:
		Channel(std::string name);	//thinking if i should start the channels with a default password like "" or something and then just use one constructor and destructor etc  07/04/25 12.30
		Channel(std::string name, std::string password);	//need to explore if we need more types to be irc compliant.
		~Channel();
		Channel(const Channel &other);
		Channel &operator=(const Channel &other);

		const std::string	get_name() const;

		void				set_topic(const std::string &str);
		const std::string	get_topic() const;
		
		void				set_pass(std::string &str);
		bool				check_pass(std::string &str);
		
		void				set_userLimit(int nb);
		int					get_userLimit();
		
		void				add_client(int fd);
		std::set<int>		get_clients() const;

		void				add_op(int fd);
		std::set<int>		get_ops() const;
		
		void				set_invOnly(bool);
		bool				get_invOnly() const;



};
