/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 13:39:36 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/25 13:23:25 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivmsgCommand.hpp"
#include <iostream>

PrivmsgCommand::PrivmsgCommand() : _server(NULL) {}
PrivmsgCommand::PrivmsgCommand(Server* server) : _server(server) {}
PrivmsgCommand::PrivmsgCommand(const PrivmsgCommand& src) {
	this->_server = src._server;
}
PrivmsgCommand& PrivmsgCommand::operator=(const PrivmsgCommand& src) {
	if (this != &src) {
	this->_server = src._server;
	}
	return *this;
}
PrivmsgCommand::~PrivmsgCommand() {}
Server				*PrivmsgCommand::getServer() const{return (this->_server);}

void PrivmsgCommand::execute(const Command& cmd, Client& sender) {
	(void) sender;
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.size() < 1) {
		std::cout << "PRIVMSG usage: PRIVMSG <target> :<message>" << std::endl;
		std::cout << "argumets given: 0." << std::endl;
		return ; // TODO: updated functionality so that error message is sent back to the clien
	}
	else if (args.size() < 2) {
		std::cout << "PRIVMSG usage: PRIVMSG <target> :<message>";
		std::cout << "arguments give: 1." << std::endl;
		return ; // TODO: updated functionality so that error message is sent back to the clien
	}
	std::string target = args[0];
	std::string message;
	// the if block below is optional. we can just send back an error if args.size() > 2
	if (args.size() > 2) {
		for (size_t i = 1; i < args.size(); ++i) {
			message += args[i];
			if (i + 1 < args.size())
				message += " ";
		}
	}
	else {
		message = args[1];
	}
	
	if (target[0] == '#' || target[0] == '&')
	{
		if (sender.getServer()->channelExists(target))
		{
			if (sender.getServer()->getChannel(target)->isMember(sender.get_nick()))
			{
				std::string groupPrefix = ":" + sender.get_nick() + "!" + sender.get_user() + "@" + "localhost";
				std::string groupLine = groupPrefix + " PRIVMSG " + target + " :" + message + "\r\n";
				//loop all channel members, addMessage etc...
				//_out.addMessage(privmsgLine);
				//cl_Epoll_In_Out();
				sender._out.addMessage(groupLine);
				sender.cl_Epoll_In_Out();
				return ;
			}
			else
			{
				sender._out.addMessage(ircErrorText(ERR_NOTONCHANNEL, cmd, sender));
				sender.cl_Epoll_In_Out();
				return;
			}
		}
		else
		{
			sender._out.addMessage(ircErrorText(ERR_NOSUCHNICK, cmd, sender));
			sender.cl_Epoll_In_Out();
			return;
		}
	}
	
	else if(!(this->getServer()->getClientByNick(target) == NULL))
	{
		std::string prefix = ":" + sender.get_nick() + "!" + sender.get_user() + "@" + "localhost";
		std::string privmsgLine = prefix + " PRIVMSG " + target + " :" + message + "\r\n";
		this->getServer()->getClientByNick(target)->_out.addMessage(privmsgLine);
		this->getServer()->getClientByNick(target)->cl_Epoll_In_Out();

		std::string prefix2 = ":" + sender.get_nick() + "!" + sender.get_user() + "@" + "localhost";
		std::string privmsgLine2 = prefix2 + " PRIVMSG " + target + " :" + message + "\r\n";
		sender._out.addMessage(privmsgLine2); // if double tabs open is most likely that the client does not know himself due registering by hand when we have to do the /nick /user manually due no parsing \r\n
		sender.cl_Epoll_In_Out();
	}
	else 
	{
		sender._out.addMessage(ircErrorText(ERR_NOSUCHNICK, cmd, sender));
		sender.cl_Epoll_In_Out();
		return;
	}
}
