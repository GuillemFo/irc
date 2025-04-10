/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 11:34:54 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/10 13:09:12 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#include "Client.hpp"

#define CHANTYPES	"&#" //Channel types prefix supported
#define CHANNELLEN	64	 // MUST be specified
#define CHANNEL_NOT_ALLOWED_CHARS " \a,"	//space, bell and comma
#define CHANNELMODES "itkol"

#define TOPICLEN 307	// MUST be defined. 307 is the typical value


class Client ;

//Channel *channels[channelName] = new Channel(channelName); !!!!!!!
class Channel
{
	private:
		Server		*_myserver;
		std::string		_Name;
		std::string		_Topic;
		std::string		_key;	// if empty no pass need?
		std::map<std::string, Client *> _clients; // list of regular clients 
		std::map<std::string, Client *> _opclients; // list of operator clients 
		bool		_protectTopic;
		bool			_inviteOnly;
		int			_clientLimit;
		
	public:
		Channel(std::string name);	//thinking if i should start the channels with a default password like "" or something and then just use one constructor and destructor etc  07/04/25 12.30
		Channel(std::string channelName, Server *myserver);
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
		
		void	addClient(Client *theClient);
		void	addOperator(Client *theClient);
		void	remClient(std::string clientNick);
		void	remOperator(std::string clientNick);
		bool		isMember(std::string clientNick);
		bool		isOperator(std::string clientNick);
		bool		isInviteOnly();
		bool		isTopicProtected();
		bool		isChannelFull();
		
	



};
