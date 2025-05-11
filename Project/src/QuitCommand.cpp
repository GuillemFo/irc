/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:41:59 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/11 14:02:52 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "QuitCommand.hpp"
#include <iostream>
#include <set>

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
	if (sender.getOkLogin())
	{
		std::string out;
		if (args.size() > 0)
			out = ":" + sender.get_nick() + "!" + sender.get_user() + "@" + "host QUIT " + args[0] + "\r\n";
		else
			out = ":" + sender.get_nick() + "!" + sender.get_user() + "@" + "host QUIT :quitting\r\n";
		std::map<std::string, Channel*>::iterator it;
		std::set<std::string> commonNicks;
		for (it = sender.getChannels().begin(); it != sender.getChannels().end(); ++it)
		{
			std::map<std::string, Client*>::iterator itc;
			std::pair<std::set<std::string>::iterator, bool> inserted;
			std::map<std::string, Client *> members = it->second->getMembers();
			for (itc = members.begin(); itc != members.end(); ++itc) {
				inserted = commonNicks.insert(itc->first);
				if (inserted.second && sender.get_nick() != itc->second->get_nick()) {
					itc->second->sendMessage(out);
				}
			}
		
		}
		sender.sendMessage("ERROR Quitting IRC server\r\n");
	}
	close(sender.get_clientFD());
	return ;
}
