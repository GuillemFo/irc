/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:23:44 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/22 19:30:46 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CapCommand.hpp"
# include <iostream>

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
	(void) sender;
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		std::cout << "No arguments in the Cap command. Aborting."
			<< std::endl;
		return ;
	}
	const std::string& Cap = args[0];
	if (true) {
		(void)Cap;
		std::cout << "Executing Cap command. Cient authorized" << std::endl;
	}
	else {
		return ;
	}
}
