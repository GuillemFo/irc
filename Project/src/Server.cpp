/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:40:34 by gforns-s          #+#    #+#             */
/*   Updated: 2025/02/17 17:25:19 by gforns-s         ###   ########.fr       */
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

int			Server::get_port(){return (this->_port);}
const 		std::string	Server::get_nick(){return (this->_nick);}
const 		std::string	Server::get_name(){return (this->_nick);}
void		Server::set_auth(bool i){this->auth = i;}
bool		Server::get_auth(){return (this->auth);}

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


void Server::buff_to_string(char *str)
{
	std::string tmp;
	std::stringstream ss(str);
	ss >> tmp;
	std::cout << "1:" << tmp << ":" <<std::endl;
	if (tmp == "pass" || tmp == "PASS")
	{
		ss >> tmp;
		std::cout << "1 tmp:" << tmp << ":"<<std::endl;
		if (this->check_pass(tmp))
			this->set_auth(1);
		else
			this->set_auth(0);
	}
	else if (tmp == "nick" || tmp == "NICK")
	{
		if (this->get_auth() == false)
		{
			std::cout << "User not registered" << std::endl;
			return ;
		}
		std::cout << "2 tmp:" << tmp<< ":" <<std::endl;
		ss >> tmp;
		this->set_nick(tmp);
	}
	else if (tmp == "user" || tmp == "USER")
	{
		if (this->get_auth() == false)
		{
			std::cout << "User not registered" << std::endl;
			return ;
		}
		std::cout << "3 tmp:" << tmp << ":"<<std::endl;
		ss >> tmp;
		this->set_user(tmp);
	}
	else
	{
		if (this->get_auth() == false)
			std::cout << "User not registered" << std::endl;
		return ;
	}
		
}
