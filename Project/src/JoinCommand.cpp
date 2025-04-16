/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 21:42:22 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/17 00:54:02 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "JoinCommand.hpp"
# include "CommandHandler.hpp"
# include <iostream>

JoinCommand::JoinCommand() : _server(NULL) {}
JoinCommand::JoinCommand(const JoinCommand& src) {
	this->_server = src._server;
}
JoinCommand& JoinCommand::operator=(const JoinCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

JoinCommand::~JoinCommand () {}
JoinCommand::JoinCommand (Server* server) : _server(server) {}

bool JoinCommand::isValidChannelName(const std::string& name) {
	if (name.empty()) {
		std::cout << "Aborting JOIN: channel name is empty."
			<< std::endl;
		return false;
	}
	if (name[0] != '#' && name[0] != '&') {
		std::cout << "Aborting JOIN: channel must start"
			<< " with # or &." << std::endl;
		return false;
	}
	if (name.length() > 50) {
		std::cout << "Aborting JOIN: channel name contains over 50"
			<< " symbols." << std::endl;
		return false;
	}
	for (size_t i = 0; i < name.length(); ++i) {
		char c = name[i];
		if (c == ' ' || c == ',' || c < 32) {
			std::cout << "Aborting JOIN: channel name contains invalid"
				<< " symbols." << std::endl;
			return false;
		}
	}
	return true;
}

void JoinCommand::execute(const Command& cmd, Client& sender) {
	(void) sender;
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		std::cout << "No arguments in the JOIN command. Aborting."
			<< std::endl;
		return ;
	}
	const std::string& channelName = args[0];
	if (JoinCommand::isValidChannelName(channelName)) {
		std::cout << "Executing JOIN command. Joining channel: "
			<< channelName << std::endl;
		//TODO: implement channel joining functionality
	}
	else {
		return ;
	}
}
