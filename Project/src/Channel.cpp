/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:27:19 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/22 20:45:24 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name) : _Name(name) {}

Channel::Channel(std::string channelName, Server *server)
{
	_Name = channelName;
	_server = server;
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

bool				Channel::isPassRequired() {return (!this->_key.empty());}

bool 				Channel::check_pass(const std::string &str) {return (str == this->_key);}

void				Channel::setProtectTopic() {this->_protectTopic = true;}
void				Channel::resetProtectTopic() {this->_protectTopic = false;}
void				Channel::setInviteOnly() {this->_inviteOnly = true;}
void				Channel::resetInviteOnly() {this->_inviteOnly = false;}

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
	if (this->_clients.size() == 0) // if no clients in channel add as operator
		this->addOperator(theClient);
	this->_clients[name_tolower(theClient->get_nick())] = theClient;
}
void	Channel::addOperator(Client *theClient)
{
	// the logic of it has to be normal or operator should be outside
	this->_opclients[name_tolower(theClient->get_nick())] = theClient;
}


void	Channel::remClient(const std::string &clientNick)
{
	std::map<std::string, Client *>::iterator it;

	// in the partchannel will be checked that client isMember
	it = this->_clients.find(name_tolower(clientNick));
	if (it == this->_clients.end())  // it must not happen
		return;
	this->_clients.erase(it);
	// remove from operators
	it = this->_opclients.find(name_tolower(clientNick));
	if (it == this->_opclients.end())
		return;
	this->_opclients.erase(it);

}
void	Channel::remOperator(const std::string &clientNick)
{
	// It will be called just if isOperator after a MOD -o
	// another way to remove from map
	int d = this->_opclients.erase(name_tolower(clientNick));
	(void) d;
}

bool		Channel::isMember(const std::string &clientNick)
{
	std::map<std::string, Client *>::iterator it;
	
	it = this->_clients.find(name_tolower(clientNick));
	if (it != this->_clients.end())
		return true;
	return false;
}
bool		Channel::isOperator(const std::string &clientNick)
{
	std::map<std::string, Client *>::iterator it;
	
	it = this->_opclients.find(name_tolower(clientNick));
	if (it != this->_opclients.end())
	return true;
	return false;
}

bool		Channel::isInviteOnly()
{
	return	this->_inviteOnly;
}

bool		Channel::isTopicProtected()
{
	return	this->_protectTopic;
}

bool		Channel::isChannelFull()
{
	if (this->_clientLimit > 0 &&
		(this->_clients.size() >= this->_clientLimit))
		return true;
	return false;
}

bool		Channel::isChannelEmpty()
{
	return (this->_clients.size() == 0);
}

bool Channel::isNameCorrect(const std::string theName)
{
	if (theName.empty() ||
		theName.size() > CHANNELLEN ||
		!strchr(CHANTYPES, theName[0]) ||
		theName.find(' ') != std::string::npos ||
		theName.find('\a') != std::string::npos ||
		theName.find(',') != std::string::npos)
		return false;
	return true;
}
