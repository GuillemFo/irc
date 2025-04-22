/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:00:29 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/22 19:04:26 by gforns-s         ###   ########.fr       */
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
		std::cout << "No arguments in the Nick command. Aborting."
			<< std::endl;
		return ;
	}
	const std::string& Nick = args[0];
	if (NickCommand::isValidNick(Nick)) {
		sender.set_nick(Nick);
		std::cout << "Executing Nick command. Nick: " << Nick << " assigned to client: " << sender.get_clientFD() << std::endl;
	}
	else {
		return ;
	}
}
