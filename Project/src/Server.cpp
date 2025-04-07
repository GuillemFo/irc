/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:40:34 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/07 12:32:22 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int sv_fd, std::string sv_pass, int port) : _sv_fd(sv_fd), _sv_pass(sv_pass), _port(port){}

Server::~Server(){}

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

///////////////////////////// OUR FUNCTIONS ////////////////////////////////////

int	Server::get_serverFD() {return (this->_sv_fd);}

void	Server::set_server_name(const std::string &s){this->_sv_name = s;}


int	Server::get_port()const {return (this->_port);}

bool	Server::check_pass(std::string &str)
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
		this->_cl_map.insert(std::pair<int, Client*>(fd, new Client(fd)));
		return (1);
	}
	else
		std::cout << "Client with fd " << fd << " already exists!" << std::endl;
	return (-1);
}



int	Server::addChannelMap(std::string &str)
{
	if (_ch_map.find(str) == _ch_map.end())
	{
		this->_ch_map.insert(std::pair<std::string, Channel*>(str, new Channel(str)));
		return (1);
	}
	else
		std::cout << "Channel with name " << str << " already exists!" << std::endl;
	return (-1);
}



int	Server::addChannelMap(std::string &str, std::string &pw)
{
	if (_ch_map.find(str) == _ch_map.end())
	{
		this->_ch_map.insert(std::pair<std::string, Channel*>(str, new Channel(str, pw)));
		return (1);
	}
	else
		std::cout << "Channel with name " << str << " already exists!" << std::endl;
	return (-1);
}



int	Server::rmClientMap(int fd)
{
	if (_cl_map.find(fd) == _cl_map.end())
		std::cout << "Client with fd " << fd << " does not exists!" << std::endl;
	else
	{
		delete this->_cl_map[fd];
		this->_cl_map.erase(fd);
		return (1);
	}
	return (-1);
}



int	Server::rmChannelMap(std::string &str)
{
	if (_ch_map.find(str) == _ch_map.end())
		std::cout << "Channel with name " << str << " does not exists!" << std::endl;
	else
	{
		
		delete this->_ch_map[str];
		this->_ch_map.erase(str);
		return (1);
	}
	return (-1);
}



int	Server::rmChannelMap(std::string &str, std::string &pw)
{
	if (_ch_map.find(str) == _ch_map.end())
		std::cout << "Channel with name " << str << " does not exists!" << std::endl;
	else
	{
		if (check_pass(pw) == true)	//maybe we need to add a flag to overrule the check if its operator?
		{
			delete this->_ch_map[str];
			this->_ch_map.erase(str);
			return (1);
		}
		else
			std::cout << "Password missmach for Channel " << str << ". Cannot delete with proper password." << std::endl;
	}
	return (-1);
}


void	Server::welcome_msg(const std::string &nickname)
{
	std::stringstream message;
	message << ":" << _sv_name << " 001 " << nickname << " :Welcome to our IRC server " << nickname << "!" << std::endl;
	this->send_out(message.str());
	message.clear();
}

int	Server::send_out(std::string message)
{
	ssize_t bytes_sent = send(this->client_fd , message.c_str(), strlen(message.c_str()), 0);
	if (bytes_sent < 0)
	{
		std::cout << "Error sending message from server to client" << std::endl;
		close(this->_sv_fd);
		return (1);
	}
	return (0);
}


//This will need to be redesigned

void Server::buff_to_string(char *str)
{
	//Prepare strings to be split when \n is found ???
	///////// Printing tool to show non printable //////////////////////////
	std::string test(str);
	test = replace_tool(test, "\r", "'/r'");
	test = replace_tool(test, "\n", "'/n'\n");
	std::cout << "buff_in:" << C_R << test << C_RESET << ":end"<< std::endl;
	////////////////////////////////////////////////////////////////////////

	std::string content;
	std::string line(str);
	size_t pos = line.find('\r');
	if (line[pos] == '\r' && line[pos +1] == '\n') //change 01/04/25 12.32 line[pos +1] == '\n' to check poss contains \r first
	{
		while (pos != std::string::npos)
		{
			if (line[pos] == '\r' && line[pos +1] == '\n')	//change 01/04/25 12.32 line[pos +1] == '\n' to check poss contains \r first
			{
				content = line.substr(0, pos);
				this->command_list(content);
				line.erase(0, pos+2);
				pos = line.find('\r');
				if (!line.empty() && line[pos] == '\r' && line[pos +1] == '\n') //change 01/04/25 12.32 added check for line[pos] == '\r'
				{
					pos = line.find('\r');
				}
			}			
		}
		pos = line.find('\n');
		content = line.substr(0, pos);
		this->command_list(content);
	}
	else
		throw std::string("Exiting");
}

//moved all commands to a proper file. redoing buff_to_string to properly trim the incoming strings. 31/3/25 12.11PM

//Issues with CAP sometimes sends end and sometimes wont. 31/03/25 15.21 // irssi issue, moving to hexchat 01/04/25

// Next step try listen multiple clients with epoll 



///////////////////////////CLIENT/////////////////////////////////////////////
void		Server::set_nick(const std::string &str){this->_nick = str;}
void		Server::set_user(const std::string &str){this->_user = str;}
const 		std::string	Server::get_nick()const {return (this->_nick);}
const 		std::string	Server::get_name()const {return (this->_nick);}
bool		Server::get_reg() const{return this->_reg;}
void		Server::set_auth(bool i){this->auth = i;}
void		Server::set_reg(bool i){this->_reg = i;}
bool		Server::get_auth()const {return (this->auth);}
//////////////////////////////////////////////////////////////////////////////