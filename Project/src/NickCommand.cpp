/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:00:29 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/09 11:29:26 by codespace        ###   ########.fr       */
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

bool isIrcSpecialChar(char c) {
    return (c >= 0x5B && c <= 0x60) || (c >= 0x7B && c <= 0x7D);
}


bool NickCommand::isValidNick(const std::string& name) {
	if (name.empty() ||
		name.size() > NICKLEN || // has to return an error or just truncate
		!(isalpha(name[0]) || isIrcSpecialChar(name[0])))
		return false;
	for (size_t i = 1; i < name.length(); ++i) {
		char c = name[i];
		if (!(isalpha(c) || isdigit(c) || isIrcSpecialChar(c) || c == '-'))
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
	std::string Nick = args[0]; //not made const just in case nick can be truncated
	if (! sender.getPassOK())
	{
		sender.sendMessage(ircErrorText(ERR_PASSWDMISMATCH, cmd, sender));
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
		std::string oldNick = sender.get_nick();
		std::string chgMsg = ":" + oldNick + " NICK " + Nick + "\r\n";
		sender.set_nick(Nick);
		sender.sendMessage(chgMsg);
		sender.changeAllNicks(oldNick);
		sender.sendAllChannels(chgMsg);
	}
}
