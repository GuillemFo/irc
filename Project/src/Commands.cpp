/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 12:04:57 by gforns-s          #+#    #+#             */
/*   Updated: 2025/03/31 12:14:20 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"



/// Needs to be splited properly for each command


///transform all tmp to to_uper or to_lower so we can handle and protect properly dup info (except pass text !!!)

	std::stringstream ss(str);
	ss >> tmp;
	std::cout << "Command:" << tmp << ":" << std::endl << std::endl;	//ak the first word of the string incoming
	


if (tmp == "cap")
	{
		ss >> tmp;
		if (tmp == "CAP" || tmp == "cap")
		{
			this->send_out("Missing info?? Pending");
			return ;
		}
		if (tmp == "ls" || tmp == "LS")
		{
			this->send_out("CAP * LS :");
			return ;
		}
	}


	if (tmp == "pass")	// this sets auth
	{
		ss >> tmp;
		std::cout << "pass:" << tmp << ":"<<std::endl;
		if (this->check_pass(tmp))
			this->set_auth(1);
		else
			this->set_auth(0);
	}
	if (tmp == "nick")	// this sets nick
	{
		if (this->get_auth() == false)
		{
			this->send_out("Missing password\n");
			std::cout << "Missing password" << std::endl;
			return ;
		}
		ss >> tmp;
		if (tmp == "nick")
		{
			this->send_out("Nick cannot be empty\n");
			std::cout << "Nick cannot be empty" << std::endl;
			return ;
		}
		tmp = this->to_lower(tmp);
		std::cout << "nick:" << tmp<< ":" <<std::endl;
		this->set_nick(tmp);
	}
	if (tmp == "user") // this sets user and check if nick.empty() to set the register flag true or false
	{
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
		if (tmp == "user")
		{
			this->send_out("User cannot be empty\n");
			std::cout << "User cannot be empty" << std::endl;
			return ;
		}
		tmp = this->to_lower(tmp);
		std::cout << "user:" << tmp << ":"<<std::endl;
		this->set_user(tmp);
		if (!this->_nick.empty() && !this->_user.empty())
		{
			this->set_reg(1);
			this->welcome_msg(this->_nick);
		}

	}
	if (tmp == "msg")
	{
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
		if (tmp == "msg")
		{
			this->send_out("Message cannot be empty\n");
			std::cout << "Message cannot be empty" << std::endl;
			return ;
		}
		std::cout << "msg:" << tmp << ":" <<std::endl;
		//send as a client to all

	}
	if (tmp == "join")
	{
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
	else if (tmp == "help")
	{
		std::cout << "####   REMEMBER TO EXPAND PROPERLY   ####" << std::endl;
		this->send_out("####   REMEMBER TO EXPAND PROPERLY   ####\n");

		std::cout << "Available commands:" << std::endl;
		this->send_out("Available commands:\n");

		std::cout << "- PASS: This command will allow the user to authenticate on the server. User will need to set NICK and USER to register properly" << std::endl;
		this->send_out("- PASS: This command will allow the user to authenticate on the server. User will need to set NICK and USER to register properly\n");
		
		std::cout << "- NICK: This command will set the nickname of the user for this server. This nickname cannot be equal to other users." << std::endl;
		this->send_out("- NICK: This command will set the nickname of the user for this server. This nickname cannot be equal to other users.\n");
		
		std::cout << "- USER: This command will set the username of the user for this server." << std::endl;
		this->send_out("- USER: This command will set the username of the user for this server.\n");
		
		std::cout << "- MSG: This command will send a message to the channel connected. need to expand command info once we add all" << std::endl;
		this->send_out("- MSG: This command will send a message to the channel connected. need to expand command info once we add all\n");
		
		std::cout << "- JOIN: This command will connect the user to a desired channel if exists. need to expand command info once we add all" << std::endl;
		this->send_out("- JOIN: This command will connect the user to a desired channel if exists. need to expand command info once we add all\n");
		
		std::cout << "- HELP: This command will show this list." << std::endl;
		this->send_out("- HELP: This command will show this list.\n");
	}
	else
	{
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
		}
		else
		{
			this->send_out("Unkown command:");
			this->send_out(tmp);
			this->send_out(" Use 'HELP' for more info\n");
			std::cout << "Unkown command:" << tmp << " Use 'HELP' for more info" << std::endl;
		}
		return ;
	}