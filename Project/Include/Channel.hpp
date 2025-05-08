/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 11:34:54 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/08 20:55:41 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#include "Client.hpp"

#define CHANTYPES	"&#" //Channel types prefix supported
#define CHANNELLEN	50	 // MUST be specified
#define CHANNEL_NOT_ALLOWED_CHARS " \a,"	//space, bell and comma
#define CHANNELMODES "itkol"



class Client ;
class Server ;

//Channel *channels[channelName] = new Channel(channelName); !!!!!!!
class Channel
{
	private:
		Server		*_server;
		std::string		_Name;
		std::string		_Topic;
		std::string		_key;
		std::map<std::string, Client *> _clients; // list of all clients 
		std::map<std::string, Client *> _opclients; // list of operator clients 
		std::map<std::string, Client *> _invited; // list of operator clients 
		bool			_protectTopic;
		bool			_inviteOnly;
		size_t			_clientLimit;
		
	public:
		Channel(std::string name);
		Channel(std::string channelName, Server *myserver);
		~Channel();
		Channel(const Channel &other);
		Channel &operator=(const Channel &other);

		Server*	getServer() const;

		const std::string	get_name() const;

		void				set_topic(const std::string &str);
		const std::string	get_topic() const;
		
		void				set_pass(std::string &str);
		bool				isPassRequired();
		bool				check_pass(const std::string &str);
		const std::string&	get_pass() const;
		
		void				set_userLimit(int nb);
		void				clear_userLimit();
		int					get_userLimit();
//NB: I changed setters here to take a bool as an argument and set the respective bool in channel equal to that argument
		void				setProtectTopic(bool modelFlag);
		void				resetProtectTopic();
		void				setInviteOnly(bool modeFlag);
		void				resetInviteOnly();
		std::string			getModesSet();
		
		void				addClient(Client *theClient);
		void				addOperator(Client *theClient);
		void				addInvited(Client *theClient);
		void				remClient(const std::string &clientNick);
		void				remOperator(const std::string &clientNick);
		void				remInvited(const std::string &clientNick);
		bool				isMember(const std::string &clientNick);
		bool				isOperator(const std::string &clientNick);
		bool				isInvited(const std::string &clientNick);
		bool				isInviteOnly();
		bool				isTopicProtected();
		bool				isChannelFull();
		bool				isChannelEmpty();
		static bool			isNameCorrect(const std::string &theName);
		void				broadcast(const std::string &msg);
		void				broadcast(const std::string &msg, Client &sender); //all except sender
		std::vector<std::string> getNicks();
		std::map<std::string, Client *> getMembers();
		void				printInfo();

};
