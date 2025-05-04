/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:41:59 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/04 21:06:46 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "QuitCommand.hpp"
# include <iostream>

QuitCommand::QuitCommand() : _server(NULL) {}
QuitCommand::QuitCommand(const QuitCommand& src) {
	this->_server = src._server;
}
QuitCommand& QuitCommand::operator=(const QuitCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

QuitCommand::~QuitCommand () {}
QuitCommand::QuitCommand (Server* server) : _server(server) {}
Server				*QuitCommand::getServer() const{return (this->_server);}


void QuitCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL

	if (sender.getOkLogin() && args.size() > 0)
	{
		std::string out = ":" + sender.get_nick() + "!" + sender.get_user() + "@" + "host QUIT " + args[0] + "\r\n";
		std::map<std::string, Channel*>::iterator it;

		for (it = sender.getChannels().begin(); it != sender.getChannels().end(); ++it) // not for all channels, once per client!!
		{
			std::string channel = it->first;
			sender.getServer()->getChannel(channel)->broadcast(out, sender);
		}
		sender.sendMessage("ERROR: Quitting IRC server\r\n");
	}
	close(sender.get_clientFD());
	return ;
}