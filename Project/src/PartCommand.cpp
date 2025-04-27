/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:06:40 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/27 21:50:25 by josegar2         ###   ########.fr       */
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

void partChannel(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();

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
		sender.getServer()->getChannel(channel)->remClient(sender.get_nick()); // might segfault
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

void PartCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return ;
	}
	if (!sender.getOkLogin())
	{
		sender.sendMessage(ircErrorText(ERR_NOTREGISTERED, cmd, sender)); //temp error, need something to tell it has not introduced the pass or has not registered.
		return ;
	}
	Command splitcmd(cmd);
	std::vector<std::string> targets = split_arg(args[0]);
	std::vector<std::string>::const_iterator it;
	for (it = targets.begin(); it != targets.end(); ++it) {
		splitcmd.clearArgs();
		splitcmd.addArg(*it);
		if (args.size() >= 2) // add reason if exists
			splitcmd.addArg(args[1]);
		partChannel(splitcmd, sender);
	}
	return ;
	
}
