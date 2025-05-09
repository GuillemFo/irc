/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 13:39:36 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/09 11:31:25 by codespace        ###   ########.fr       */
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

void privMsg(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	std::string target = args[0];
	std::string message;
	message = args[1];
	if (target[0] == '#' || target[0] == '&')
	{
		if (sender.getServer()->channelExists(target))
		{
			if (sender.getServer()->getChannel(target)->isMember(sender.get_nick()))
			{
				std::string groupPrefix = ":" + sender.get_nick();
				std::string groupLine = groupPrefix + " PRIVMSG " + target + " :" + message + "\r\n";
				sender.getServer()->getChannel(target)->broadcast(groupLine, sender);
				return ;
			}
			else
			{
				sender.sendMessage(ircErrorText(ERR_NOTONCHANNEL, cmd, sender));
				return;
			}
		}
		else
		{
			sender.sendMessage(ircErrorText(ERR_NOSUCHCHANNEL, cmd, sender));
			return;
		}
	}
	// target = nick
	else if(!(sender.getServer()->getClientByNick(target) == NULL))
	{
		std::string prefix = ":" + sender.get_nick() + "!" + sender.get_user() + "@" + "localhost";
		std::string privmsgLine = prefix + " PRIVMSG " + target + " :" + message + "\r\n";
		sender.getServer()->getClientByNick(target)->sendMessage(privmsgLine);
	}
	else 
	{
		sender.sendMessage(ircErrorText(ERR_NOSUCHNICK, cmd, sender));
		return;
	}
}

void PrivmsgCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.size() < 1) {
		sender.sendMessage(ircErrorText(ERR_NORECIPIENT, cmd, sender));
		return ;
	}
	if (args.size() < 2) {
		sender.sendMessage(ircErrorText(ERR_NOTEXTTOSEND, cmd, sender));
		return ;
	}
	if (!sender.getOkLogin())
	{
		sender.sendMessage(ircErrorText(ERR_NOTREGISTERED, cmd, sender));
		std::cout << sender._out.getMessage() << std::endl;
		return ;
	}
	Command splitcmd(cmd);
	std::vector<std::string> targets = split_arg(args[0]);
	std::vector<std::string>::const_iterator it;
	for (it = targets.begin(); it != targets.end(); ++it) {
		splitcmd.clearArgs();
		splitcmd.addArg(*it);
		splitcmd.addArg(args[1]);
		privMsg(splitcmd, sender);
	}	
}
