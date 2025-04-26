/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romanzdanov <romanzdanov@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:23:44 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/26 21:20:05 by romanzdanov      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CapCommand.hpp"
#include <sys/epoll.h>
#include <iostream>

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
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		std::string errorMsg = ":irc.server.name 410 "
			+ sender.get_nick() + " :CAP command requires arguments\r\n";
		sender.appendToOutBuffer(errorMsg);
		std::cout << "No arguments in the Cap command. Aborting."
			<< std::endl;
		return ;
	}
	const std::string& subcmd = args[0];
	if (subcmd == "LS" and args.size() >= 2) {
		//TODO: remove the cout message before submitting the project.
		std::cout << "Your CAP Command would be processed here if the subject "
			<< "required it :)" << std::endl;
	}
}
