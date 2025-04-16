/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:00:29 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/16 08:01:45 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "NickCommand.hpp"
# include <iostream>

///Under dev 16/04/25 08.01AM

NickCommand::NickCommand() : _server(NULL) {}
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
NickCommand::NickCommand (Server* server) : _server(server) {}

bool NickCommand::isValidNick(const std::string& name) {
	if (name.empty()) {
		std::cout << "Aborting Nick: Nick is empty."
			<< std::endl;
		return false;
	}
	if (this->_server->check_Nick(name) == true)
		return true;
	return false;
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
		sender.setNickOK();
		std::cout << "Executing Nick command. Cient authorized" << std::endl;
	}
	else {
		return ;
	}
}
