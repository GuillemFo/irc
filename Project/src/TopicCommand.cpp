/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:23:44 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/29 22:28:18 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCommand.hpp"
#include <sys/epoll.h>
#include <iostream>

TopicCommand::TopicCommand() : _server(NULL) {}
TopicCommand::TopicCommand(const TopicCommand& src) {
	this->_server = src._server;
}
TopicCommand& TopicCommand::operator=(const TopicCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

TopicCommand::~TopicCommand () {}
TopicCommand::TopicCommand (Server* server) : _server(server) {}

void TopicCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.empty()) {
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return;
	}
	if (!sender.getOkLogin())
	{
		sender.sendMessage(ircErrorText(ERR_NOTREGISTERED, cmd, sender));
		return ;
	}
	const std::string& channel = args[0];
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
	if (args.size() == 1) // view topic
	{
		if (theChannel->get_topic().empty())
			sender.sendMessage(ircReplyText(RPL_NOTOPIC, cmd, sender));
		else
			sender.sendMessage(ircReplyText(RPL_TOPIC, cmd, sender));
		return ;
	}
	//change topic
	else if (theChannel->isTopicProtected() && 
	! theChannel->isOperator(sender.get_nick()))
	{
		sender.sendMessage(ircErrorText(ERR_CHANOPRIVSNEEDED, cmd, sender));
		return ;
	}
	theChannel->set_topic(args[1]);
	sender.sendMessage(ircReplyText(RPL_TOPIC, cmd, sender));
	if (theChannel->get_topic().empty())
		//sender.sendMessage(ircReplyText(RPL_NOTOPIC, cmd, sender));
		theChannel->broadcast(ircReplyText(RPL_NOTOPIC, cmd, sender), sender);
	else
		//sender.sendMessage(ircReplyText(RPL_TOPIC, cmd, sender));	
		theChannel->broadcast(ircReplyText(RPL_TOPIC, cmd, sender), sender);
}
