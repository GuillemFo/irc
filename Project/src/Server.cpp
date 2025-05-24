/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:40:34 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/11 14:09:04 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int sv_fd, int port, std::string sv_pass) : _sv_fd(sv_fd), _port(port), _sv_pass(sv_pass){}

Server::~Server()
{
	std::map<std::string , Channel*>::iterator it;
	for (it = _ch_map.begin(); it != _ch_map.end(); ++it){
		delete it->second;
	}
	std::map<int , Client*>::iterator itc;
	for (itc = _cl_map.begin(); itc != _cl_map.end(); ++itc){
		delete itc->second;
	}
}

Server::Server(const Server &other){*this = other;}

Server &Server::operator=(const Server &other)
{
	if (this != &other)
	{
		this->_sv_pass = other._sv_pass;
		this->_port = other._port;
	}
	return (*this);
}

int	Server::get_serverFD() {return (this->_sv_fd);}

void	Server::set_epollFD(int nb) {this->_epoll_fd = nb;}
int	Server::get_epollFD() {return (this->_epoll_fd);}

void	Server::set_server_name(const std::string &s){this->_sv_name = s;}
std::string			Server::getServerName() const {return this->_sv_name;}


int	Server::get_port()const {return (this->_port);}

bool	Server::check_pass(const std::string &str)
{
	if (str == this->_sv_pass)
	{
		return (true);
	}
	return (false);
}


int	Server::addClientMap(int fd)
{
	if (_cl_map.find(fd) == _cl_map.end())
	{
		this->_cl_map.insert(std::pair<int, Client*>(fd, new Client(this, fd)));
		return (1);
	}
	// it shouldn't happen than fd already exists
	return (-1);
}



int	Server::addChannelMap(const std::string &str)
{
	if (_ch_map.find(name_tolower(str)) == _ch_map.end())
	{
		this->_ch_map.insert(std::pair<std::string, Channel*>(name_tolower(str), new Channel(str, this)));
		return (1);
	}
	// it shouldn't happen that Channel already exist whe calling thhis function
	return (-1);
}


int	Server::rmClientMap(int fd)
{
	std::map<int, Client*>::iterator it = _cl_map.find(fd);
	if (it == _cl_map.end())
	{
		// it shouldn't happen that Client with fd does not exist!
		return (-1);
	}
	delete it->second;
	_cl_map.erase(it);
	return (1);
}

void 	Server::rmChannelMap(const std::string &str)
{
	if (_ch_map.find(name_tolower(str)) != _ch_map.end())
	{
		delete this->_ch_map[name_tolower(str)];
		this->_ch_map.erase(name_tolower(str));
	}
}

const std::map<int, Client*>	&Server::getClientMap() const {return _cl_map;}

const std::map<std::string, Channel*>	&Server::getChannelMap() const {return _ch_map;}


bool	Server::nickExists(const std::string &theNick)
{
	std::map<int, Client*>::const_iterator it;
	for (it = this->getClientMap().begin(); it != this->getClientMap().end(); ++it)
	{
		Client *client = it->second;
		if (client && name_tolower(client->get_nick()) == name_tolower(theNick))
		{
			return true;
		}
	}
	return false;
}

bool	Server::channelExists(const std::string &theChannel)
{
	return this->_ch_map.find(name_tolower(theChannel)) != this->_ch_map.end();
}

Channel	*Server::getChannel(const std::string &theChannel)
{
	std::map<std::string, Channel *>::iterator it;

	it = this->_ch_map.find(name_tolower(theChannel));
	if (it == this->_ch_map.end())
	{
		return NULL;
	}
	return it->second;
}

Client	*Server::getClient(const int &fd)
{
	std::map<int, Client *>::iterator it;

	it = this->_cl_map.find(fd);
	if (it == this->_cl_map.end())
	{
		return NULL;
	}
	return it->second;
}

Client			*Server::getClientByNick(const std::string &s)
{
	std::map<int, Client *>::iterator it;

	for (it = this->_cl_map.begin(); it != this->_cl_map.end(); ++it)
	{
		if (name_tolower(it->second->get_nick()) == name_tolower(s))
			return it->second;
	}
	return NULL;
}

bool Server::registerAllCommands() {
	bool	r = true;

	r = r && _dispatcher.registerHandler("PRIVMSG", new PrivmsgCommand(this));
	r = r && _dispatcher.registerHandler("JOIN", new JoinCommand(this));
	r = r && _dispatcher.registerHandler("NICK", new NickCommand(this));
	r = r && _dispatcher.registerHandler("USER", new UserCommand(this));
	r = r && _dispatcher.registerHandler("PASS", new PassCommand(this));
	r = r && _dispatcher.registerHandler("CAP", new CapCommand(this));
	r = r && _dispatcher.registerHandler("QUIT", new QuitCommand(this));
	r = r && _dispatcher.registerHandler("PING", new PingCommand(this));
	r = r && _dispatcher.registerHandler("PART", new PartCommand(this));
	r = r && _dispatcher.registerHandler("KICK", new KickCommand(this));
	r = r && _dispatcher.registerHandler("MODE", new ModeCommand(this));
	r = r && _dispatcher.registerHandler("TOPIC", new TopicCommand(this));
	r = r && _dispatcher.registerHandler("INVITE", new InviteCommand(this));
	r = r && _dispatcher.registerHandler("WHO", new WhoCommand(this));

	return r;
}
