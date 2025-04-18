/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:21:26 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/18 22:35:10 by josegar2         ###   ########.fr       */
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

void	Client::joinChannel(std::string &channelName)
{
	std::cout << channelName << std::endl;
}

void	Client::joinChannel(std::string &channelName, std::string &channelPwd)
{
	std::cout << channelName << " - pwd : " << channelPwd << std::endl;
}

void	Client::sendMessage(std::string &theMessage)
{
	this->_out.addMessage(theMessage);
}
