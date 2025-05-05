/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:27:19 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/05 17:36:06 by gforns-s         ###   ########.fr       */
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


Channel::Channel(const Channel &other) {*this = other;}
Channel& Channel::operator=(const Channel& other) {
	if (this != &other) {
		this->_Name = other._Name;
	}
	return *this;
}

Server*				Channel::getServer() const { return this->_server; }

const std::string	Channel::get_name() const {return (this->_Name);}

void				Channel::set_topic(const std::string &str) {this->_Topic = str;}

const std::string	Channel::get_topic() const {return (this->_Topic);}

void				Channel::set_pass(std::string &str) {this->_key = str;}

bool				Channel::isPassRequired() {return (!this->_key.empty());}

bool 				Channel::check_pass(const std::string &str) {return (str == this->_key);}

const std::string&	Channel::get_pass() const {
	return this->_key;
}



void				Channel::set_userLimit(int userLimit) {
	this->_clientLimit = userLimit;
}

void				Channel::clear_userLimit() {
	this->_clientLimit = 0;
}

int					Channel::get_userLimit() {
	return this->_clientLimit;
}

//NB: I changed setters here to take a bool as an argument and set the respective bool in channel equal to that argument
void				Channel::setProtectTopic(bool modeFlag) {this->_protectTopic = modeFlag;}
void				Channel::resetProtectTopic() {this->_protectTopic = false;}
void				Channel::setInviteOnly(bool modeFlag) {this->_inviteOnly = modeFlag;}
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

bool Channel::isNameCorrect(const std::string &theName)
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

void Channel::broadcast(const std::string &msg)
{
	std::map<std::string, Client *>::iterator it;
	for(it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		it->second->sendMessage(msg);
	}
}

void Channel::broadcast(const std::string &msg, Client &sender)
{
	std::map<std::string, Client *>::iterator it;
	for(it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (name_tolower(it->second->get_nick()) != name_tolower(sender.get_nick()))
			it->second->sendMessage(msg);
	}
}

std::vector<std::string> Channel::getNicks()
{
	std::vector<std::string> result;
	std::map<std::string, Client *>::iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (isOperator(it->second->get_nick()))
			result.push_back("@" + it->second->get_nick());
		else
			result.push_back(it->second->get_nick());
	}
	return result;
}

void Channel::printInfo()
{
    std::cout << "///===PRINTING CHANNEL INFO===///" << std::endl;
    std::cout << "Channel Name: " << _Name << std::endl;
    std::cout << "Topic: " << (_Topic.empty() ? "(No topic set)" : _Topic) << std::endl;
    std::cout << "Invite Only: " << (_inviteOnly ? "Yes" : "No") << std::endl;
    std::cout << "Topic Protected: " << (_protectTopic ? "Yes" : "No") << std::endl;
    std::cout << "Password Protected: " << (isPassRequired() ? "Yes" : "No") << std::endl;
    // std::cout << "Client Limit: " << (_clientLimit > 0 ? std::to_string(_clientLimit) : "No limit") << std::endl;
    std::cout << "Current Clients: " << _clients.size() << std::endl;
    std::cout << "Operators: " << _opclients.size() << std::endl;

    // Print all client nicknames
    std::cout << "Clients in Channel: ";
    std::map<std::string, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        std::cout << it->first << " "; // `it->first` is the nickname of the client
    }
    std::cout << std::endl;

    // Print all operator nicknames
    std::cout << "Operators in Channel: ";
    for (it = _opclients.begin(); it != _opclients.end(); ++it)
    {
        std::cout << it->first << " "; // `it->first` is the nickname of the operator
    }
    std::cout << std::endl;

    std::cout << "///===END OF PRINTING CHANNEL INFO===///\n" << std::endl;
}
