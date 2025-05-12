/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:21:26 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/12 11:10:58 by gforns-s         ###   ########.fr       */
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
	this->_in.clear();
}

Client::Client(const Client &other){*this = other;}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		this->_client_fd = other._client_fd;
		this->_nick = other._nick;
	}
	return (*this);
}


int					Client::get_clientFD(){return (this->_client_fd);}
void				Client::set_ip(const std::string &theIP) { this->_ip = theIP;}
std::string			Client::get_ip(){return this->_ip;}

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

bool	Client::isRegistered() {return this->_registered;}


void	Client::partChannel(const std::string &channelName) 
{
	Channel *pChannel;
	if ((channelName.empty()) ||
	! this->_server->channelExists(channelName))
		throw std::runtime_error(ERR_NOSUCHCHANNEL);
	pChannel = this->_server->getChannel(channelName);
	if (! pChannel)
		throw std::runtime_error(ERR_NOSUCHCHANNEL);
	if (! pChannel->isMember(this->_nick))
		throw std::runtime_error(ERR_NOTONCHANNEL);
	pChannel->remClient(this->get_nick());
	this->_channels.erase(name_tolower(channelName));
	if (pChannel->isChannelEmpty())
		pChannel->getServer()->rmChannelMap(channelName);
}

std::string Client::getListOfChannels() {
	std::map<std::string, Channel *>::iterator it = this->_channels.begin();
	std::string result;

	while(it != this->_channels.end())
	{
		result += it->second->get_name();
		++it;
		if (it != this->_channels.end())
			result += ",";
	}
	return result;

}

void Client::partAllChannels()
{
	while (!this->_channels.empty())
	{
		std::string name = this->_channels.begin()->first;
		try {
			this->partChannel(name);
		} catch (...) {}
	}
}

void	Client::addChannel(Channel *pChannel)
{
	_channels[name_tolower(pChannel->get_name())] = pChannel;
}

void	Client::remChannel(const std::string &theChannel)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name_tolower(theChannel));
	if (it != _channels.end())
		_channels.erase(it);
}

void	Client::sendMessage(const std::string &theMessage)
{
	this->addOutMessage(theMessage);
	this->cl_Epoll_In_Out();
}

//changed to check all channels instead of the known by the client. maybe better to aknowledg the client that has been invited to certain channels? 12.05.25 10.50am
void	Client::changeAllNicks(const std::string &oldNick)
{
	std::map<std::string, Channel*>::const_iterator it;
	for (it = this->getServer()->getChannelMap().begin(); it != this->getServer()->getChannelMap().end(); ++it)
	{
		if (it->second->isMember(oldNick) || it->second->isInvited(oldNick))
		{
			if (it->second->isMember(oldNick))
			{
				it->second->addClient(this);
				if (it->second->isOperator(oldNick))
				{
					it->second->addOperator(this);
					it->second->remOperator(oldNick);
					std::cout << "OPERATOR added" << std::endl;
				}
				it->second->remClient(oldNick);
			}
			if (it->second->isInvited(oldNick))
			{
				it->second->addInvited(this);
				it->second->remInvited(oldNick);
				std::cout << "INVITE added" << std::endl;
			}
		}
	}
}

void	Client::sendAllChannels(const std::string &theMessage)
{
	std::map<std::string, Channel*>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); ++it)
	{
		it->second->broadcast(theMessage);
	}
}

void Client::addOutMessage(const std::string& message) {
	std::string temp = message;
	this->_out.addMessage(temp);
}

std::string Client::getOutMessage() {
	return this->_out.getMessage();
}

void Client::popCharsSent(size_t bytesSent) {
	this->_out.addOffset(bytesSent);
}

bool Client::isOutEmpty() const {
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
	ev.events = EPOLLIN;
	ev.data.fd = this->get_clientFD();
	epoll_ctl(this->getServer()->get_epollFD(), EPOLL_CTL_MOD, this->get_clientFD(), &ev);
}

void	Client::cl_Epoll_In_Out()
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev)); // Explicit zeroing to avoid some errors
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = this->get_clientFD();
	epoll_ctl(this->getServer()->get_epollFD(), EPOLL_CTL_MOD, this->get_clientFD(), &ev);	
}

std::map<std::string, Channel*> &Client::getChannels()
{
	return this->_channels;
}
