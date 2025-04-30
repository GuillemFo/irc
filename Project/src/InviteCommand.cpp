/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:23:44 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/30 10:08:12 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InviteCommand.hpp"
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
	if (!sender.getServer()->channelExists(channel))
	{
		sender.sendMessage(ircErrorText(ERR_NOSUCHCHANNEL, cmd, sender));
		return ;
	}
	Channel *theChannel = sender.getServer()->getChannel(channel);
	if (!theChannel->isMember(sender.get_nick()))
	{
		sender.sendMessage(ircErrorText(ERR_NOTONCHANNEL, cmd, sender));
		return ;
	}
	if (theChannel->isInviteOnly() && 
	! theChannel->isOperator(sender.get_nick()))
	{
		sender.sendMessage(ircErrorText(ERR_CHANOPRIVSNEEDED, cmd, sender));
		return ;
	}
/*	theChannel->set_topic(args[1]);
	sender.sendMessage(ircReplyText(RPL_TOPIC, cmd, sender));
	if (theChannel->get_topic().empty())
		//sender.sendMessage(ircReplyText(RPL_NOTOPIC, cmd, sender));
		theChannel->broadcast(ircReplyText(RPL_NOTOPIC, cmd, sender), sender);
	else
		//sender.sendMessage(ircReplyText(RPL_TOPIC, cmd, sender));	
		theChannel->broadcast(ircReplyText(RPL_TOPIC, cmd, sender), sender);
*/
		}
