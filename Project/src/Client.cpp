/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:21:26 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/26 16:40:43 by josegar2         ###   ########.fr       */
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
	this->_okLogin = false;
}

Client::Client(Server *server, int cl_fd) : _server(server) , _client_fd(cl_fd)
{
	this->_nick = std::string();
	this->_user = std::string();
	this->_realname = std::string();
	this->_passok = false;
	this->_registered = false;
	this->_okLogin = false;
}

Client::~Client() {
	this->partAllChannels();
	// has to be removed from client map in Server
}

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
Server				*Client::getServer() const{return (this->_server);}

void				Client::set_nick(const std::string &str){this->_nick = str;}
const std::string	Client::get_nick()const {return (this->_nick);}

void				Client::set_user(const std::string &str){this->_user = str;}
const std::string	Client::get_user()const {return (this->_user);}

void				Client::set_host(const std::string &str){this->_user = str;}
const std::string	Client::get_host()const {return (this->_user);}

void				Client::setRealName(const std::string &str) {this->_realname = str;}

void	Client::setPassOK()
{
	_passok = true;
}

bool	Client::getPassOK()
{
	return this->_passok;
}

void	Client::setOkLogin() {this->_okLogin = true;}
bool	Client::getOkLogin() {return this->_okLogin;}

void	Client::setRegistered()
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


void	Client::partChannel(const std::string &channelName) 
{
	std::cout << "Part CHannel" << std::endl;
	Channel * pChannel;
	// check if channelName is not empty or doesn't exist
	if ((channelName.empty()) ||
	! this->_server->channelExists(channelName))
		throw std::runtime_error(ERR_NOSUCHCHANNEL);
		// Get pointer to channel object
	pChannel = this->_server->getChannel(channelName);
	if (! pChannel)
		throw std::runtime_error(ERR_NOSUCHCHANNEL); // it shouldn't happen
	// check if in channel
	if (! pChannel->isMember(this->_nick))
		throw std::runtime_error(ERR_NOTONCHANNEL);
	// remove client from client map in channel and from channel map in client
	pChannel->remClient(this->get_nick());
	this->_channels.erase(name_tolower(channelName));
}

// Leave all channels after receiving a JOIN 0
void	Client::partAllChannels()
{
	std::map<std::string, Channel *>::iterator it;
	for (it = this->_channels.begin(); 
		 it != this->_channels.end(); ++it)
	{
		try {
			this->partChannel(it->first);
		} catch (...) {}	// no errors should be thrown from partChannel
	}
	this->_channels.clear();  // no memebers should be left
}

void	Client::addChannel(Channel *pChannel)
{
	_channels[name_tolower(pChannel->get_name())] = pChannel;
}

void	Client::remChannel(std::string &theChannel)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name_tolower(theChannel));
	if (it != _channels.end())
		_channels.erase(it);
}


// channelName should be shorter or equal to CHANNELLEN
void	Client::joinChannel(const std::string &channelName)
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

void	Client::joinChannel(const std::string &channelName, const std::string &channelPwd)
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

void	Client::sendMessage(const std::string &theMessage)
{
	this->_out.addMessage(theMessage);
	this->cl_Epoll_In_Out();
}

void	Client::sendAllChannels(const std::string &theMessage)
{
	std::map<std::string, Channel*>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); ++it)
	{
		it->second->broadcast(theMessage);
	}
}

void Client::appendToOutBuffer(const std::string& message) {
	std::string temp = message;
	this->_out.addMessage(temp);
}

std::string Client::getNextOutBufferChunk() {
	return this->_out.getMessage();
}

void Client::advanceOutBufferOffset(size_t bytesSent) {
	this->_out.addOffset(bytesSent);
}

bool Client::isOutBufferEmpty() const {
	return this->_out.isEmpty();
}

void Client::clearOutBuffer() {
	this->_out.clear();
}

const OutBuffer& Client::getOutBuffer() const {
	return this->_out;}


void	Client::cl_Epoll_In()
{
	struct epoll_event ev;
	ev.events = EPOLLIN; //| EPOLLET;
	ev.data.fd = this->get_clientFD();
	epoll_ctl(this->getServer()->get_epollFD(), EPOLL_CTL_MOD, this->get_clientFD(), &ev);
}

void	Client::cl_Epoll_In_Out()
{
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLOUT; //| EPOLLET;
	ev.data.fd = this->get_clientFD();
	epoll_ctl(this->getServer()->get_epollFD(), EPOLL_CTL_MOD, this->get_clientFD(), &ev);	
}
