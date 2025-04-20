/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:21:26 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/20 21:40:44 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
Client::Client(int cl_fd) : _client_fd(cl_fd) 
{
	this->_nick = std::string();
	this->_user = std::string();
	this->_realname = std::string();
	this->_passok = false;
	this->_registered = false;
	
}

Client::Client(Server *server, int cl_fd) : _server(server) , _client_fd(cl_fd)
{
	this->_nick = std::string();
	this->_user = std::string();
	this->_realname = std::string();
	this->_passok = false;
	this->_registered = false;
}

Client::~Client() {}

Client::Client(const Client &other){*this = other;}

Client &Client::operator=(const Client &other)	//do we need this??
{
	if (this != &other)
	{
		this->_client_fd = other._client_fd;
		this->_nick = other._nick;
	}
	return (*this);
}

bool Client::isNickCorrect(std::string theNick)
{
	if (theNick.empty() ||
		theNick.size() > NICKLEN ||
		strchr(NICK_NOT_STARTING, theNick[0]) ||
		theNick.find(' ') != std::string::npos ||
		theNick.find('\0') != std::string::npos ||
		theNick.find('\r') != std::string::npos ||
		theNick.find('\n') != std::string::npos)
		return false;
	return true;
}

int					Client::get_clientFD(){return (this->_client_fd);}

void				Client::set_nick(const std::string &str){this->_nick = str;}
const std::string	Client::get_nick()const {return (this->_nick);}

void				Client::set_user(const std::string &str){this->_user = str;}
const std::string	Client::get_user()const {return (this->_user);}

void				Client::set_host(const std::string &str){this->_user = str;}
const std::string	Client::get_host()const {return (this->_user);}

// To dev properly
void				Client::setRealName(std::string &str) {this->_realname = str;}

void	Client::setPassOK() //set to true. In the constructor would be false
{
	_passok = true;
}

void	Client::setRegistered() //set to true. In the constructor would be false
{
	_registered = true;
	
}		


/*
std::string	Client::getLowerNick()
{
	
}
*/

/*
std::string	Client::getSource() // : <nickname> [ "!" <user> ] [ "@" <host> ]
{
	
}
*/


bool	Client::isRegistered() {return this->_registered;}

void	Client::partChannel(std::string &channelName) 
{
	std::cout << channelName << std::endl;
}

// Leave all channels after receiving a JOIN 0
void	Client::partAllChannels()
{
	
}

// channelName should be shorter or equal to CHANNELLEN
void	Client::joinChannel(std::string &channelName)
{
	std::cout << "Join Channel without password" << std::endl;
	if (channelName == "0") // it should be checked in the command handler to avoid JOIN 0,#chan or #chan,0
		this->partAllChannels();
	Channel * pChannel;
	// check if channel is not empty
	if (channelName.empty())
		throw std::runtime_error(ERR_NOSUCHCHANNEL);
	// check if name is correct
	if (!Channel::isNameCorrect(channelName))
		throw std::runtime_error(ERR_BADCHANMASK);
	// check if channel exists
	if (! this->_server->channelExists(channelName))
	{
		this->_server->addChannelMap(channelName); // create new channel
	}
	// Get pointer to channel object
	pChannel = this->_server->getChannel(channelName);
	if (! pChannel)
		throw std::runtime_error(ERR_NOSUCHCHANNEL); // it shouldn't happen
	// already in?
	if (pChannel->isMember(this->_nick))
		return;		// this will send again the JOIN replies
		// option to throw td::runtime_error(ERR_USERONCHANNEL); // not RFC compliant
		// also possible a custom error just to say it's ignored and no replies
	// is Invite only (+i) ?
	if (pChannel->isInviteOnly())
		throw std::runtime_error(ERR_INVITEONLYCHAN);
	// is pass required? In this function pass is not provided
	if (pChannel->isPassRequired())
		throw std::runtime_error(ERR_BADCHANNELKEY);
	// is channel full?
	if (pChannel->isChannelFull())
		throw std::runtime_error(ERR_CHANNELISFULL);
	// add channel to the map of channels
	this->_channels[name_tolower(channelName)] = pChannel;
	// add client to the map of clients in channel
	pChannel->addClient(this);}

void	Client::joinChannel(std::string &channelName, std::string &channelPwd)
{
	std::cout << "Join Channel with password" << std::endl;
	if (channelName == "0")
		this->partAllChannels();
	Channel * pChannel;
	// check if channel is not empty
	if (channelName.empty())
		throw std::runtime_error(ERR_NOSUCHCHANNEL);
	// check if name is correct
	if (!Channel::isNameCorrect(channelName))
		throw std::runtime_error(ERR_BADCHANMASK);
	// check if channel exists
	if (! this->_server->channelExists(channelName))
	{
		this->_server->addChannelMap(channelName); // create new channel
	}
	// Get pointer to channel object
	pChannel = this->_server->getChannel(channelName);
	if (! pChannel)
		throw std::runtime_error(ERR_NOSUCHCHANNEL); // it shouldn't happen
	// already in?
	if (pChannel->isMember(this->_nick))
		return;		// this will send again the JOIN replies
		// option to throw std::runtime_error(ERR_USERONCHANNEL); // not RFC compliant
		// also possible a custom error just to say it's ignored and no replies
	// is Invite only (+i) ?
	if (pChannel->isInviteOnly())
		throw std::runtime_error(ERR_INVITEONLYCHAN);
	// if no pwd required
	if (pChannel->isPassRequired() && pChannel->check_pass(channelPwd))
		throw std::runtime_error(ERR_BADCHANNELKEY);
	// is channel full?
	if (pChannel->isChannelFull())
		throw std::runtime_error(ERR_CHANNELISFULL);
	// add channel to the map of channels
	this->_channels[name_tolower(channelName)] = pChannel;
	// add client to the map of clients in channel
	pChannel->addClient(this);
}

void	Client::sendMessage(std::string &theMessage)
{
	this->_out.addMessage(theMessage);
}
