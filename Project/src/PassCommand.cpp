/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 07:49:13 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/24 12:18:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "PassCommand.hpp"
# include <iostream>

PassCommand::PassCommand() : _server(NULL) {}
PassCommand::PassCommand(const PassCommand& src) {
	this->_server = src._server;
}
PassCommand& PassCommand::operator=(const PassCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

PassCommand::~PassCommand () {}
PassCommand::PassCommand (Server* server) : _server(server) {}

bool PassCommand::isValidPass(const std::string& name) {
	if (name.empty()) {
		std::cout << "Aborting Pass: Pass is empty."
			<< std::endl;
		return false;
	}
	if (this->_server->check_pass(name) == true)
		return true;
	return false;
}

void PassCommand::execute(const Command& cmd, Client& sender) {
	(void) sender;
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		std::cout << "No arguments in the Pass command. Aborting."
			<< std::endl;
		return ;
	}
	const std::string& Pass = args[0];
	if (PassCommand::isValidPass(Pass)) {
		sender.setPassOK();
		std::cout << "Executing Pass command. Cient authorized" << std::endl;
	}
	else {
		return ;
	}
}
