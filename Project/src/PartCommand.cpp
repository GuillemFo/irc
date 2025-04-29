/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:06:40 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/29 01:33:14 by codespace        ###   ########.fr       */
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
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return ;
	}
	if (sender.getOkLogin())
	{
		const std::string& channel = args[0];
		if (!sender.getServer()->channelExists(channel))
		{
			sender.sendMessage(ircErrorText(ERR_NOSUCHCHANNEL, cmd, sender));
			return ;
		}
		else if (!sender.getServer()->getChannel(channel)->isMember(sender.get_nick()))
		{
			sender.sendMessage(ircErrorText(ERR_NOTONCHANNEL, cmd, sender)); //dont know if this is needed.
			return ;
		}
		else
		{
			sender.getServer()->getChannel(channel)->remClient(sender.get_nick());
			sender.remChannel(channel);
			std::string partLine = ":" + sender.get_nick() + " PART " + channel + " :";
			if (args.size() >= 2)
				partLine += args[1];
			else
				partLine += "User is leaving the channel";
			partLine += "\r\n";
			sender.sendMessage(partLine);
			sender.getServer()->getChannel(channel)->broadcast(partLine);
			return ;
		}
	}
	else
	{
		sender.sendMessage(ircErrorText(ERR_NOTREGISTERED, cmd, sender)); //temp error, need something to tell it has not introduced the pass or has not registered.
		return ;
	}
	return ;
	
}
