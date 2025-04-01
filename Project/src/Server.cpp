/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:40:34 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/01 08:40:12 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){}

Server::~Server(){}

Server::Server(const Server &other){*this = other;}

Server &Server::operator=(const Server &other)
{
	if (this != &other)
	{
		this->_pass = other._pass;
		this->_port = other._port;
	}
	return (*this);
}

///////////////////////////// OUR FUNCTIONS ////////////////////////////////////

void	Server::set_port(const int &nb)
{
	this->_port = nb;
	//Debug
	std::cout << "Port in:" << nb << std::endl;
}


void	Server::set_pass(const std::string &str)
{
	this->_pass = str;
	//Debug
	std::cout << "Pass in:" << str << std::endl;
}

void	Server::set_pass(const char *str)
{
	std::string s(str);
	this->_pass = s;
	//Debug
	std::cout << "Pass in:" << str << std::endl;
}

void	Server::set_server_name(const std::string &s){this->_sv_name = s;}

void	Server::set_nick(const std::string &str){this->_nick = str;}
void	Server::set_user(const std::string &str){this->_user = str;}

bool		Server::get_reg() const{return this->_reg;}
int			Server::get_port()const {return (this->_port);}
const 		std::string	Server::get_nick()const {return (this->_nick);}
const 		std::string	Server::get_name()const {return (this->_nick);}
void		Server::set_auth(bool i){this->auth = i;}
void		Server::set_reg(bool i){this->_reg = i;}
bool		Server::get_auth()const {return (this->auth);}

bool 		Server::check_pass(std::string &str)
{
	if (str == this->_pass)
	{
		return (true);
	}
	else
		return (false);
	return (false);
}


void	Server::check_port(const std::string &str) //change to a better name
{
	if (!str.empty())
	{
		for (std::string::size_type i = 0; i < str.length(); ++i)
		{
			if (!std::isdigit(str[i]))
				throw std::string("Non digit for port");
		}
		if (str.length() <= 5) // min port 0 max port 65535 //maybe ther is a function to protect this properly for common used ports that are protected
		{
			int res = std::atoi(str.c_str());
			if (res <= 65535 && res >= 0)
				Server::set_port(res);
			else
				throw std::string("Invalid port");
		}
		else
				throw std::string("Invalid port");
	}
	else
		throw std::string("Empty string");
}

void Server::welcome_msg(const std::string &nickname)
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
		close(this->server_fd);
		return (1);
	}
	return (0);
}


std::string Server::to_lower(std::string &str)
{
	std::stringstream ss;
	for (int i = 0; str[i] != '\0'; i++)
	{
		ss << (char)std::tolower(str[i]);
	}
		std::string ret = ss.str();
	return (ret);
}

void Server::buff_to_string(char *str)
{
	//Prepare strings to be split when \n is found ???
	std::string content;
	std::cout << "IN ->" << str << "<- ##" << std::endl;
	std::string line(str);
	size_t pos = line.find('\r');
	if (line[pos +1] == '\n')
	{
		while (pos != std::string::npos)
		{
			if (line[pos +1] == '\n')
			{
				content = line.substr(0, pos);
				this->command_list(content);
				//std::cout << "-->" << content << "<--" << std::endl << std::endl;  // Debug line
				line.erase(0, pos+2);
				pos = line.find('\r');
				if (!line.empty() && line[pos +1] == '\n')
				{
					//std::cout << line << " WEWO" << std::endl;
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

//Issues with CAP sometimes sends end and sometimes wont. 31/03/25 15.21

// Next step try listen multiple clients with epoll