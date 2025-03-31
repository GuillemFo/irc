/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 12:04:57 by gforns-s          #+#    #+#             */
/*   Updated: 2025/03/31 15:28:09 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"



/// Needs to be splited properly for each command


///transform all tmp to to_uper or to_lower so we can handle and protect properly dup info (except pass text !!!)

void Server::command_list(std::string &str)
{
	std::cout << "!" << str << "!" <<std::endl;

	std::string tmp;
	std::stringstream ss(str);
	ss >> tmp;
	tmp = to_lower(tmp);
	if (tmp == "cap")
	{
		ss >> tmp;
		if (tmp == "CAP" || tmp == "cap")
		{
			this->send_out("Missing info?? Pending");
		}
		else if (tmp == "ls" || tmp == "LS")
		{
			this->send_out("CAP * LS :multi-prefix");
			std::cout << "LS?" << std::endl;
		}
		else if (tmp == "end" || tmp == "END")
		{
			std::cout << "END???" << std::endl;
		}
		else
			std::cout << "WERRORO :(" << std::endl;
	}
	else if (tmp == "pass")	// this sets auth
	{
		ss >> tmp;
		std::cout << "pass:" << tmp << ":"<<std::endl;
		if (this->check_pass(tmp))
			this->set_auth(1);
		else
			this->set_auth(0);
	}
	else if (tmp == "nick")	// this sets nick // missing protection for same nicknames!!
	{
		ss >> tmp;
		if (this->get_auth() == false)
		{
			this->send_out("Missing password\n");
			std::cout << "Missing password" << std::endl;
			return ;
		}
		tmp = this->to_lower(tmp);
		std::cout << "nick:" << tmp<< ":" <<std::endl;
		this->set_nick(tmp);
		return ;
	}
	else if (tmp == "user") // this sets user and check if nick.empty() to set the register flag true or false
	{
		ss >> tmp;
		if (this->get_auth() == false)
		{
			this->send_out("Missing password\n");
			std::cout << "Missing password" << std::endl;
			return ;
		}
		else if (this->_nick.empty())
		{
			this->send_out("User not established. Use NICK to set a nickname\n");
			std::cout << "User not established. Use NICK to set a nickname" << std::endl;
			return ;
		}
		tmp = this->to_lower(tmp);
		std::cout << "user:" << tmp << ":"<<std::endl;
		this->set_user(tmp);
		if (!this->_nick.empty() && !this->_user.empty())
		{
			this->set_reg(1);
			this->welcome_msg(this->_nick); // if i send the welcome it returns the cap end !!! 31/03/25 15.29
		}
	}
	else if (tmp == "msg")
	{
		ss >> tmp;
		if (this->get_auth() == false)
		{
			this->send_out("Missing password\n");
			std::cout << "Missing password" << std::endl;
			return ;
		}
		else if (this->get_reg() == false)
		{
			this->send_out("User not established. Missing nickname or username\n");
			std::cout << "User not established. Missing nickname or username" << std::endl;
			return ;
		}
		std::cout << "msg:" << tmp << ":" <<std::endl;
		//send as a client to all

	}
	else if (tmp == "join")
	{
		ss >> tmp;
		if (this->get_auth() == false)
		{
			this->send_out("Missing password\n");
			std::cout << "Missing password" << std::endl;
			return ;
		}
		else if (this->get_reg() == false)
		{
			this->send_out("User not established. Missing nickname or username\n");
			std::cout << "User not established. Missing nickname or username" << std::endl;
			return ;
		}
		ss >> tmp;
		std::cout << "join:" << tmp << ":" <<std::endl;
		//send as a client to all
	}
	else if (!tmp.empty())
	{
		this->send_out("Unkown command:");
		this->send_out(tmp);
		this->send_out(" Use '!HELP' for more info\n");
		std::cout << "Unkown command:" << tmp << std::endl;
		return ;
	}

}

