/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CapCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:23:44 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/23 18:12:05 by rzhdanov         ###   ########.fr       */
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
		std::cout << "No arguments in the Cap command. Aborting."
			<< std::endl;
		return ;
	}
	const std::string& subcmd = args[0];
	if (subcmd == "LS") {
		sender.appendToOutBuffer("CAP * LS :multi-prefix sasl\r\n");
	}
	else if (subcmd == "REQ") {
		if (args.size() < 2) {
			sender.appendToOutBuffer("CAP * NAK :Missing capability list\r\n");
			return ;
		}
		sender.appendToOutBuffer("CAP * ACK :" + args[1] + "\r\n");
	}
	else if (subcmd == "END") {
		sender.setCapNegotiationDone(true);
	}
	else {
		sender.appendToOutBuffer("CAP * NAK :Unknown subcommand\r\n");
	}
	if (!sender.getOutBuffer().isEmpty()) {
		struct epoll_event ev;
		ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
		ev.data.fd = sender.get_clientFD();
		if (epoll_ctl(_server->get_epollFD(), EPOLL_CTL_MOD,
			sender.get_clientFD(), &ev) == -1) {
				std::cout << "epoll_ctl failed for CAP response" << std::endl;
			}
	}
}

void Client::setCapNegotiationDone(bool done) {
	_capNegotiationDone = done;
}

bool Client::isCapNegotiationDone() const {
	return _capNegotiationDone;
}