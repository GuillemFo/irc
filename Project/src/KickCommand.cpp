/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:41:59 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/12 18:22:56 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "KickCommand.hpp"
# include <iostream>

KickCommand::KickCommand() : _server(NULL) {}
KickCommand::KickCommand(const KickCommand& src) {
	this->_server = src._server;
}
KickCommand& KickCommand::operator=(const KickCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

KickCommand::~KickCommand () {}
KickCommand::KickCommand (Server* server) : _server(server) {}
Server				*KickCommand::getServer() const{return (this->_server);}

bool isValidChannelName(const std::string& name) {
	if (name.empty() || 
			(name[0] != '#' && name[0] != '&') ||
			name.length() > CHANNELLEN ||
			!strchr(CHANTYPES, name[0]))
		return false;
	for (size_t i = 1; i < name.length(); ++i) {
		char c = name[i];
		if (c == ' ' || c == ',' || c == ':' ||
			c == 0x00 || c == 0x07 || c == 0x0D || c == 0x0A) {
			return false;
		}
	}
	return true;
}


void KickChannel(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();
	if (args[0].empty() || args[1].empty())
	{
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return ;
	}
	const std::string& channelName = args[0];
	const std::string& target = args[1];
	if (sender.getServer()->getChannel(channelName) == NULL) {
		sender.sendMessage(ircErrorText(ERR_BADCHANMASK, cmd, sender));
		return ;
	}
	if (!sender.getServer()->getChannel(channelName)->isMember(sender.get_nick()))
	{
		sender.sendMessage(ircErrorText(ERR_NOTONCHANNEL, cmd, sender));
		return ;
	}
	if (!sender.getServer()->getChannel(channelName)->isOperator(sender.get_nick()))
	{
		sender.sendMessage(ircErrorText(ERR_CHANOPRIVSNEEDED, cmd, sender));
		return ;
	}
	if (!sender.getServer()->getChannel(channelName)->isMember(target))
	{
		sender.sendMessage(ircErrorText(ERR_USERNOTINCHANNEL, cmd, sender));
		return ;
	}
	std::string commentPrefix = ":" + sender.get_nick();
	std::string commentLine = commentPrefix + " KICK " + channelName + " " + target + " :";
	if (args.size() > 2 && !args[2].empty())
		commentLine += args[2];
	else
		commentLine += "You've been kicked";
	commentLine += "\r\n";
	sender.getServer()->getChannel(channelName)->broadcast(commentLine);
	
	sender.getServer()->getChannel(channelName)->remClient(target);
	sender.getServer()->getClientByNick(target)->remChannel(channelName);
	
	return ;
}

void KickCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL

	if (args.empty() || args.size() < 2) {
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return ;
	}
	if (!sender.getOkLogin())
	{
		sender.sendMessage(ircErrorText(ERR_NOTREGISTERED, cmd, sender));
		return ;
	}
	
	
	Command splitcmd(cmd);
	std::vector<std::string> channels = split_arg(args[0]);
	std::vector<std::string> users = split_arg(args[1]);
	if (channels.size() > 1 && channels.size() != users.size())
	{
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return ;
	}
	std::vector<std::string>::const_iterator itc = channels.begin();
	std::vector<std::string>::const_iterator itu = users.begin();
	while (itu != users.end()) {
		splitcmd.clearArgs();
		splitcmd.addArg(*itc);
		splitcmd.addArg(*itu);
		++itu;
		if (channels.size() > 1)
			++itc;
		if (args.size() > 2)	
			splitcmd.addArg(args[2]);
		KickChannel(splitcmd, sender);
	}
	
	return ;
}
