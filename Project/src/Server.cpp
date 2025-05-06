/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:40:34 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/05 22:33:33 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int sv_fd, int port, std::string sv_pass) : _sv_fd(sv_fd), _port(port), _sv_pass(sv_pass){}

Server::~Server()
{
	// loop to delete all clients and channels due new[] in each map!!
	// delete ->second (as the exam :) )
	// dont forget to .clear _cl_map and _ch_map
	std::map<std::string , Channel*>::iterator it;
	for (it = _ch_map.begin(); it != _ch_map.end(); ++it){
		delete it->second;
	}
	std::map<int , Client*>::iterator itc;
	for (itc = _cl_map.begin(); itc != _cl_map.end(); ++itc){
		delete itc->second;
	}
}

Server::Server(const Server &other){*this = other;}	//no need??

Server &Server::operator=(const Server &other)	//no need??
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
	else
		std::cout << "Client with fd " << fd << " already exists!" << std::endl;
	return (-1);
}



int	Server::addChannelMap(const std::string &str)
{
	if (_ch_map.find(name_tolower(str)) == _ch_map.end())
	{
		this->_ch_map.insert(std::pair<std::string, Channel*>(name_tolower(str), new Channel(str, this)));
		return (1);
	}
	else
		std::cout << "Channel with name " << str << " already exists!" << std::endl;
	return (-1);
}


int	Server::rmClientMap(int fd)
{
	std::map<int, Client*>::iterator it = _cl_map.find(fd);
	if (it == _cl_map.end())
	{
		std::cout << "Client with fd " << fd << " does not exist!" << std::endl;
		return (-1);
	}
	delete it->second;
	_cl_map.erase(it);
	return (1);
}




int	Server::rmChannelMap(std::string &str) // not needed by subject
{
	if (_ch_map.find(name_tolower(str)) == _ch_map.end())
		std::cout << "Channel with name " << str << " does not exists!" << std::endl;
	else
	{
		
		delete this->_ch_map[name_tolower(str)];
		this->_ch_map.erase(name_tolower(str));
		return (1);
	}
	return (-1);
}

const std::map<int, Client*>	&Server::getClientMap() const {return _cl_map;}

const std::map<std::string, Channel*>	&Server::getChannelMap() const {return _ch_map;}



// void	Server::welcome_msg(const std::string &nickname)
// {
// 	std::stringstream message;
// 	message << ":" << _sv_name << " 001 " << nickname << " :Welcome to our IRC server " << nickname << "!" << std::endl;
// 	this->send_out(message.str());
// 	message.clear();
// }


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
		return 0; // need to expand for proper error!
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



// this command should be called from server.init() function
// the commented lines below should be uncommented for each new command handler
// that will be implemented
void Server::registerAllCommands() {
	_dispatcher.registerHandler("PRIVMSG", new PrivmsgCommand(this));
	_dispatcher.registerHandler("JOIN", new JoinCommand(this));
	_dispatcher.registerHandler("NICK", new NickCommand(this));
	_dispatcher.registerHandler("USER", new UserCommand(this));
	_dispatcher.registerHandler("PASS", new PassCommand(this));
	_dispatcher.registerHandler("CAP", new CapCommand(this));
	_dispatcher.registerHandler("QUIT", new QuitCommand(this));
	_dispatcher.registerHandler("PING", new PingCommand(this));
	// _dispatcher.registerHandler("PONG", new PongCommand(this)); // not needed by subject
	// _dispatcher.registerHandler("NOTICE", new NoticeCommand(this)); // not needed by subject
	_dispatcher.registerHandler("PART", new PartCommand(this));
	_dispatcher.registerHandler("KICK", new KickCommand(this));
	_dispatcher.registerHandler("MODE", new ModeCommand(this));
	_dispatcher.registerHandler("TOPIC", new TopicCommand(this));
	_dispatcher.registerHandler("INVITE", new InviteCommand(this));
	std::cout << "All command handlers have been registered." << std::endl;
	
	// TODO: add error handling, so that if empty string is given or non-existing
	// handler, the program exits cleanly (this is a whole other set of functions
	// that we will need to implement)
	// in this particular case maybe make this funciton return a bool
	// and make all other functions return a bool. then we can do
	//return _dispatcher.registerHandler("JOIN", new JoinCommand(this)) &&
	//		_dispatcher.registerHandler("PRIVMSG", new PrivmsgCommand(this)) && ...
	// and in the server.init() we put:
	// if (!registerAllCommands) {
	//	clean_up();
	//}
}

void Server::printAllClientsInfo() const {
	std::cout << "----- Client Info Dump -----" << std::endl;

	if (_cl_map.empty()) {
		std::cout << "No clients connected to the server." << std::endl;
		return;
	}

	for (std::map<int, Client*>::const_iterator it = _cl_map.begin(); it != _cl_map.end(); ++it) {
		Client* client = it->second;
		if (client) {
			std::cout << "Client FD: " << it->first << std::endl;
			std::cout << "  Nickname: " << client->get_nick() << std::endl;
			std::cout << "  Username: " << client->get_user() << std::endl;
			std::cout << "  Host: " << client->get_host() << std::endl;
			std::cout << "  Is Registered: " << (client->isRegistered() ? "Yes" : "No") << std::endl;
			std::cout << "-----------------------------" << std::endl;
		}
	}
}
