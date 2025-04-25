/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 21:42:22 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/25 15:45:23 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "JoinCommand.hpp"
# include "CommandHandler.hpp"
# include <iostream>

JoinCommand::JoinCommand() : _server(NULL) {}
JoinCommand::JoinCommand(const JoinCommand& src) {
	this->_server = src._server;
}
JoinCommand& JoinCommand::operator=(const JoinCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

JoinCommand::~JoinCommand () {}
JoinCommand::JoinCommand (Server* server) : _server(server) {}

bool JoinCommand::isValidChannelName(const std::string& name) {
	if (name.empty()) {
		std::cout << "Aborting JOIN: channel name is empty."
			<< std::endl;
		return false;
	}
	if (name[0] != '#' && name[0] != '&') {
		std::cout << "Aborting JOIN: channel must start"
			<< " with # or &." << std::endl;
		return false;
	}
	if (name.length() > 50) {
		std::cout << "Aborting JOIN: channel name contains over 50"
			<< " symbols." << std::endl;
		return false;
	}
	for (size_t i = 0; i < name.length(); ++i) {
		char c = name[i];
		if (c == ' ' || c == ',' || c < 32) {
			std::cout << "Aborting JOIN: channel name contains invalid"
				<< " symbols." << std::endl;
			return false;
		}
	}
	return true;
}

void JoinCommand::execute(const Command& cmd, Client& sender) {
	(void) sender;
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		std::cout << "No arguments in the JOIN command. Aborting." << std::endl;
		sender._out.addMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		sender.cl_Epoll_In_Out();
		return ;
	}
	if (sender.getOkLogin())
	{
		const std::string& channelName = args[0];
		//need a checker for args 1 2 etc to loop the args because it can contain a password;
		if (JoinCommand::isValidChannelName(channelName)) {
			//std::cout << "Executing JOIN command. Joining channel: "
			//	<< channelName << std::endl;
			if (sender.getServer()->channelExists(channelName))
			{
				if (sender.getServer()->getChannel(channelName)->isPassRequired())
				{
					const std::string& pass = args[1];
					std::cout << "is valid pass:" << sender.getServer()->getChannel(channelName)->check_pass(pass) << ":" << std::endl;
					if (!sender.getServer()->getChannel(channelName)->check_pass(pass))
					{
						sender._out.addMessage(ircErrorText(ERR_PASSWDMISMATCH, cmd, sender));
						sender.cl_Epoll_In_Out();
						return ;
					}
				}
				else if (sender.getServer()->getChannel(channelName)->isMember(sender.get_nick()))
				{
					sender._out.addMessage(ircErrorText(ERR_USERONCHANNEL, cmd, sender));
					sender.cl_Epoll_In_Out();
					return ;
				}
				else if (sender.getServer()->getChannel(channelName)->isInviteOnly())
				{
					sender._out.addMessage(ircErrorText(ERR_INVITEONLYCHAN, cmd, sender));
					sender.cl_Epoll_In_Out();
					return ;
				}
				else if (sender.getServer()->getChannel(channelName)->isChannelFull())
				{
					sender._out.addMessage(ircErrorText(ERR_CHANNELISFULL, cmd, sender));
					std::cout << sender._out.getMessage() << std::endl;
					sender.cl_Epoll_In_Out();
					return ;
				}
				sender.getServer()->getChannel(channelName)->addClient(&sender);
				sender._out.addMessage(":" + sender.get_nick() + "!" + sender.get_user() + "@" + "host JOIN :" + channelName + "\r\n");
				sender.cl_Epoll_In_Out();
				return ;
			}
			else
			{
				sender.getServer()->addChannelMap(channelName);
				sender.getServer()->getChannel(channelName)->addClient(&sender);
				sender._out.addMessage(":" + sender.get_nick() + "!" + sender.get_user() + "@" + "host JOIN :" + channelName + "\r\n");
				sender.cl_Epoll_In_Out();
				return ;
			}
		}
		else {
			std::cout << "ERROR::Executing JOIN command. "
				<< channelName << " not valid" << std::endl;
			sender._out.addMessage(ircErrorText(ERR_NOSUCHNICK, cmd, sender));
			sender.cl_Epoll_In_Out();
			return ;
		}
	}
	else 
	{
		sender._out.addMessage(ircErrorText(ERR_PASSWDMISMATCH, cmd, sender)); //temp error, need something to tell it has not introduced the pass or has not registered.
		std::cout << sender._out.getMessage() << std::endl;
		sender.cl_Epoll_In_Out();
		return ;
	}
	return ;
}
