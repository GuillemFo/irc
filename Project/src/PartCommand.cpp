/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:06:40 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/25 14:45:21 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "PartCommand.hpp"
# include <iostream>

PartCommand::PartCommand() : _server(NULL) {}
PartCommand::PartCommand(const PartCommand& src) {
	this->_server = src._server;
}
PartCommand& PartCommand::operator=(const PartCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

PartCommand::~PartCommand () {}
PartCommand::PartCommand (Server* server) : _server(server) {}


void PartCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		sender._out.addMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		std::cout << sender._out.getMessage() << std::endl;
		sender.cl_Epoll_In_Out();
		return ;
	}
	const std::string& channel = args[0];
	if (!sender.getServer()->channelExists(channel))
	{
		sender._out.addMessage(ircErrorText(ERR_NOSUCHCHANNEL, cmd, sender));
		std::cout << sender._out.getMessage() << std::endl;
		sender.cl_Epoll_In_Out();
		return ;
	}
	else if (!sender.getServer()->getChannel(channel)->isMember(sender.get_nick()))
	{
		sender._out.addMessage(ircErrorText(ERR_NOTONCHANNEL, cmd, sender));
		std::cout << sender._out.getMessage() << std::endl;
		sender.cl_Epoll_In_Out();
		return ;
	}
	else
	{
		sender.getServer()->getChannel(channel)->remClient(sender.get_nick()); // might segfault
		//loop all clients of that channel to notify that client left.
		std::string prefix = ":*!" + sender.get_user() + "@" + "localhost"; // we send ":*!" so tell its server who says something
		std::string partLine = prefix + " PRIVMSG " + channel + " :" + sender.get_nick() + " has left"+ "\r\n";
		sender._out.addMessage(partLine);
		sender.cl_Epoll_In_Out();
		return ;
	}
	return ;
	
}