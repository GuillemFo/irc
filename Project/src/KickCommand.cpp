/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:41:59 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/05 12:27:31 by gforns-s         ###   ########.fr       */
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
		sender.sendMessage(ircErrorText(ERR_USERNOTINCHANNEL, cmd, sender)); //need to fix 04.05 08.09 pm
		return ;
	}
	std::string commentPrefix = ":" + sender.get_nick();
	std::string commentLine = commentPrefix + " KICK " + channelName + " " + target + " :";
	if (!args[2].empty())
	{
		const std::string& comment = args[2];
		commentLine += comment;
	}
	commentLine += "\r\n";
	sender.getServer()->getChannel(channelName)->broadcast(commentLine, sender);
	
	sender.getServer()->getChannel(channelName)->remClient(target);
	sender.getServer()->getClientByNick(target)->remChannel(channelName);
	
	return ;
}

void KickCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL

	if (args.empty()) {
		std::cout << "No arguments in the KICK command. Aborting." << std::endl;
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return ;
	}
	if (!sender.getOkLogin())
	{
		sender.sendMessage(ircErrorText(ERR_NOTREGISTERED, cmd, sender));
		return ;
	}
	

	// The loop does not work properly 04.05.25 09.03pm
	Command splitcmd(cmd);
	std::vector<std::string> channels = split_arg(args[0]);
	std::vector<std::string> targets; 
	if (args.size() > 1)
		targets = split_arg(args[1]);
	std::vector<std::string>::const_iterator itc = channels.begin();
	std::vector<std::string>::const_iterator itk = targets.begin();
	while (itc != channels.end()) {
		splitcmd.clearArgs();
		splitcmd.addArg(*itc);
		++itc;
		if (itk != targets.end() && !(*itk).empty())
		{
			splitcmd.addArg(*itk);
			++itk;
		}
		//if no target send error for that command?
		KickChannel(splitcmd, sender);
	}
	
	return ;
}
