/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:41:59 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/03 13:03:52 by codespace        ###   ########.fr       */
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


void KickCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL

	//If op
	//if on channel
	//
	std::string out = ":" + sender.get_nick() + "!" + sender.get_user() + "@" + "host KICK " + args[0] + "\r\n";
	std::map<std::string, Channel*>::iterator it;

	for (it = sender.getChannels().begin(); it != sender.getChannels().end(); ++it) // not for all channels, once per client!!
	{
		std::string channel = it->first;
		sender.getServer()->getChannel(channel)->broadcast(out);
	}
	
	return ;
}