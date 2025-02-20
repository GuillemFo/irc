/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:40:34 by gforns-s          #+#    #+#             */
/*   Updated: 2025/02/20 12:53:52 by gforns-s         ###   ########.fr       */
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
{///transform all tmp to to_uper or to_lower so we can handle and protect properly dup info (except pass text !!!)
	//CHANGE ALL THIS TO A SWITCH
	//Check extra if extra info on string!!
	// Move get auth and get_reg to an external function
	// Next step try listen multiple clients with epoll
	std::string tmp;
	std::stringstream ss(str);
	ss >> tmp;
	tmp = this->to_lower(tmp);
	std::cout << "Main command:" << tmp << ":" <<std::endl;
	if (tmp == "pass")
	{
		ss >> tmp;
		std::cout << "pass:" << tmp << ":"<<std::endl;
		if (this->check_pass(tmp))
			this->set_auth(1);
		else
			this->set_auth(0);
	}
	else if (tmp == "nick")
	{
		if (this->get_auth() == false)
		{
			this->send_out("User not registered\n");
			std::cout << "User not registered" << std::endl;
			return ;
		}
		else if (this->get_reg() == false)
		{
			this->send_out("User not established\n");
			std::cout << "User not established" << std::endl;
			return ;
		}
		ss >> tmp;
		tmp = this->to_lower(tmp);
		std::cout << "nick:" << tmp<< ":" <<std::endl;
		this->set_nick(tmp);
	}
	else if (tmp == "user")
	{
		if (this->get_auth() == false)
		{
			this->send_out("User not registered\n");
			std::cout << "User not registered" << std::endl;
			return ;
		}
		else if (this->get_reg() == false)
		{
			this->send_out("User not established\n");
			std::cout << "User not established" << std::endl;
			return ;
		}
		ss >> tmp;
		tmp = this->to_lower(tmp);
		std::cout << "user:" << tmp << ":"<<std::endl;
		this->set_user(tmp);
	}
	else if (tmp == "msg")
	{
		if (this->get_auth() == false)
		{
			this->send_out("User not registered\n");
			std::cout << "User not registered" << std::endl;
			return ;
		}
		else if (this->get_reg() == false)
		{
			this->send_out("User not established\n");
			std::cout << "User not established" << std::endl;
			return ;
		}
		ss >> tmp;
		std::cout << "msg:" << tmp << ":" <<std::endl;
		//send as a client to all
	}
	else if (tmp == "join")
	{
		if (this->get_auth() == false)
		{
			this->send_out("User not registered\n");
			std::cout << "User not registered" << std::endl;
			return ;
		}
		else if (this->get_reg() == false)
		{
			this->send_out("User not established\n");
			std::cout << "User not established" << std::endl;
			return ;
		}
		ss >> tmp;
		std::cout << "join:" << tmp << ":" <<std::endl;
		//send as a client to all
	}
	else
	{
		if (this->get_auth() == false)
		{
			this->send_out("User not registered\n");
			std::cout << "User not registered" << std::endl;
		}
		else if (this->get_reg() == false)
		{
			this->send_out("User not established\n");
			std::cout << "User not established" << std::endl;
		}
		return ;
	}
}
