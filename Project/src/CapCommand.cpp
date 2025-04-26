/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:23:44 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/26 10:06:48 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CapCommand.hpp"
#include <sys/epoll.h>
#include <iostream>

CapCommand::CapCommand() : _server(NULL) {}
CapCommand::CapCommand(const CapCommand& src) {
	this->_server = src._server;
}
CapCommand& CapCommand::operator=(const CapCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

CapCommand::~CapCommand () {}
CapCommand::CapCommand (Server* server) : _server(server) {}

void CapCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return;
	}
	const std::string& subcmd = args[0];
	if (subcmd == "LS") {
		if (args.size() < 2) {
			sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
			return;
		}
	}
	else
	{
		// unrecognized command
		sender.sendMessage(ircErrorText(ERR_INVALIDCAPCMD, cmd, sender));
	}
}
