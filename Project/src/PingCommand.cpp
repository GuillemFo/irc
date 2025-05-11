/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:29:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/11 14:02:23 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "PingCommand.hpp"
# include <iostream>

PingCommand::PingCommand() : _server(NULL) {}
PingCommand::PingCommand(const PingCommand& src) {
	this->_server = src._server;
}
PingCommand& PingCommand::operator=(const PingCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

PingCommand::~PingCommand () {}
PingCommand::PingCommand (Server* server) : _server(server) {}


void PingCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return ;
	}
	const std::string& Ping = args[0];
	std::string pong_out =  ":" + sender.get_nick() + " PONG " + " " + Ping + "\r\n";
	sender.sendMessage(pong_out);
	return ;
}
