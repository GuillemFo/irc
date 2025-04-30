/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:23:44 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/30 14:47:04 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InviteCommand.hpp"
#include "JoinCommand.hpp"
#include <sys/epoll.h>
#include <iostream>

InviteCommand::InviteCommand() : _server(NULL) {}
InviteCommand::InviteCommand(const InviteCommand& src) {
	this->_server = src._server;
}
InviteCommand& InviteCommand::operator=(const InviteCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

InviteCommand::~InviteCommand () {}
InviteCommand::InviteCommand (Server* server) : _server(server) {}

void InviteCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.size() < 2) {
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return;
	}
	if (!sender.getOkLogin())
	{
		sender.sendMessage(ircErrorText(ERR_NOTREGISTERED, cmd, sender));
		return ;
	}
	const std::string& nick = args[0];
	if (!sender.getServer()->nickExists(nick))
	{
		sender.sendMessage(ircErrorText(ERR_NOSUCHNICK, cmd, sender));
		return ;
	}
	// There is no requirement that the channel the target user is being invited to 
	// must exist or be a valid channel
	const std::string& channel = args[1];
	if (!sender.getServer()->channelExists(channel)) // create if it doesn't exist
	{
	    if (!JoinCommand::isValidChannelName(channel)) {
		    sender.sendMessage(ircErrorText(ERR_BADCHANMASK, cmd, sender));
		    return ;
	    }
		sender.getServer()->addChannelMap(channel);  // add channel
		sender.getServer()->getChannel(channel)->addClient(&sender); //add sender to channel clients
		sender.addChannel(sender.getServer()->getChannel(channel)); //add channel to sender channels
    }
	Channel *theChannel = sender.getServer()->getChannel(channel);
	if (!theChannel->isMember(sender.get_nick()))
	{
		sender.sendMessage(ircErrorText(ERR_NOTONCHANNEL, cmd, sender));
		return ;
	}
	if (theChannel->isInviteOnly() && 
		!theChannel->isOperator(sender.get_nick()))
	{
		sender.sendMessage(ircErrorText(ERR_CHANOPRIVSNEEDED, cmd, sender));
		return ;
	}
	if (theChannel->isMember(nick))
	{
		sender.sendMessage(ircErrorText(ERR_USERONCHANNEL, cmd, sender));
		return ;
	}
	Client *theInvited = sender.getServer()->getClientByNick(nick);
	theChannel->addClient(theInvited);
	theInvited->addChannel(theChannel);
	sender.sendMessage(ircReplyText(RPL_INVITING, cmd, sender));
	theInvited->sendMessage(sender.get_nick() + " INVITE " + nick + " " + channel);
}
