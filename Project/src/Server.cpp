/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:40:34 by gforns-s          #+#    #+#             */
/*   Updated: 2025/02/13 22:27:59 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){}

Server::~Server(){}

Server::Server(const Server &other)
{
	*this = other;
}

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

const int	&Server::get_port()
{
	return (this->_port);
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
