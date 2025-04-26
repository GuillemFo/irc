/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:00:29 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/26 14:47:35 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "NickCommand.hpp"
# include "CommandHandler.hpp"
# include <iostream>

NickCommand::NickCommand() : _server(NULL) {}
NickCommand::NickCommand (Server* server) : _server(server) {}
NickCommand::NickCommand(const NickCommand& src) {
	this->_server = src._server;
}
NickCommand& NickCommand::operator=(const NickCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

NickCommand::~NickCommand () {}

bool NickCommand::isValidNick(const std::string& name) {
	if (name.empty()) {
		std::cout << "Aborting Nick: Nick is empty."
			<< std::endl;
		return false;
	}
	/////////////////////////////////////////////////////	Need to filter whats valid for nick! josegar2
	if (name.length() > 50) {
		std::cout << "Aborting NICK: Nick contains over 50"
			<< " symbols." << std::endl;
		return false;
	}
	for (size_t i = 0; i < name.length(); ++i) {
		char c = name[i];
		if (c == ' ' || c == ',' || c < 32) {
			std::cout << "Aborting Nick: Nicks contains invalid"
				<< " symbols." << std::endl;
			return false;
		}
	}
	///////////////////////////////////////////////////////////
	std::map<int, Client*>::const_iterator it;
	for (it = _server->getClientMap().begin(); it != _server->getClientMap().end(); ++it)
	{
		Client *client = it->second;
		if (client && client->get_nick() == name)
			return false;
	}
	return true;
}

void NickCommand::execute(const Command& cmd, Client& sender) {
	(void) sender;
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		sender.sendMessage(ircErrorText(ERR_NONICKNAMEGIVEN, cmd, sender));
		return ;
	}
	const std::string& Nick = args[0];
	if (! sender.getPassOK())  // can the order be NICK-PASS-USER
	{
		sender.sendMessage(ircErrorText(ERR_PASSWDMISMATCH, cmd, sender)); //temp error, need something to tell it has not introduced the pass.
		return ;
	}
	if (!NickCommand::isValidNick(Nick))
	{
		sender.sendMessage(ircErrorText(ERR_ERRONEUSNICKNAME, cmd, sender));
		return;
	}
	if (_server->nickExists(Nick))
	{
		sender.sendMessage(ircErrorText(ERR_NICKNAMEINUSE, cmd, sender));
		return ;
	}
	// nick is correct and available here
	if (sender.get_nick().empty()) //new nick
	{
		sender.set_nick(Nick);
		std::cout << "Executing Nick command. Nick: " << Nick << " assigned to client: " << sender.get_clientFD() << std::endl;
		sender.setRegistered();
	}
	else { // change nick
		//TODO broadcst to all channels the change of nick
		std::string oldNick = sender.get_nick();
		std::string chgMsg = ":" + oldNick + " NICK " + Nick + "\r\n";
		sender.sendAllChannels(chgMsg);
	}
}
