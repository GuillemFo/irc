/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:27:19 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/10 23:53:57 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name) : _Name(name) {}

Channel::Channel(std::string channelName, Server *myserver)
{
	_Name = channelName;
	_myserver = myserver;
	_Topic = std::string();
	_key = std::string();
	_inviteOnly = false;
	_protectTopic = false;
	_clientLimit = 0;
}


//Channel::Channel(const Channel &other) {*this = other;}


const std::string	Channel::get_name() const {return (this->_Name);}

void				Channel::set_topic(const std::string &str) {this->_Topic = str;}

const std::string	Channel::get_topic() const {return (this->_Topic);}

void				Channel::set_pass(std::string &str) {this->_key = str;}

bool 				Channel::check_pass(std::string &str)
{
	if (str == this->_key)
	{
		return (true);
	}
	return (false);
}


Channel::~Channel()
{
	/*
	// remove channel from client map of channels
	std::map<std::string, Client *>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it) {
        it->second->partChannel(_Name);  //it can be a loop********
    }
	for (it = _opclients.begin(); it != _opclients.end(); ++it) {
        it->second->partChannel(_Name);  //it can be a loop********
    }
	*/
}


void	Channel::addClient(Client *theClient)
{
	// once the join channel is succesful --> add client to the map _clients
	_clients[theClient->get_nick()] = theClient;
}
void	Channel::addOperator(Client *theClient)
{
	_opclients[theClient->get_nick()] = theClient;
}

/*
void	Channel::remClient(std::string clientNick)
{
	int d = _clients.erase(clientNick);  // perhaps is better a *theClient to avoid case problems
}
void	Channel::remOperator(std::string clientNick)
{
	int d = _opclients.erase(clientNick);  // perhaps is better a *theClient to avoid case problems
}
*/

bool		Channel::isMember(std::string clientNick)
{
	std::map<std::string, Client *>::iterator it;
	
	it = _clients.find(clientNick);
	if (it != _clients.end())
	return true;
	it = _opclients.find(clientNick);
	if (it != _opclients.end())
	return true;
	return false;
}
bool		Channel::isOperator(std::string clientNick)
{
	std::map<std::string, Client *>::iterator it;
	
	it = _opclients.find(clientNick);
	if (it != _opclients.end())
	return true;
	return false;
}
bool		Channel::isInviteOnly()
{
	return	_inviteOnly;
}
bool		Channel::isTopicProtected()
{
	return	_protectTopic;
}

/*
bool		Channel::isChannelFull()
{
	if (_clientLimit > 0)
	{
		if ((_clients.size() + _opclients.size()) >= _clientLimit)
		return true;
	}
	return false;
}
*/
