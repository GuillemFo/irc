/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 19:51:25 by gforns-s          #+#    #+#             */
/*   Updated: 2025/05/06 19:54:52 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WhoCommand.hpp"
#include <sys/epoll.h>
#include <iostream>

WhoCommand::WhoCommand() : _server(NULL) {}
WhoCommand::WhoCommand(const WhoCommand& src) {
	this->_server = src._server;
}
WhoCommand& WhoCommand::operator=(const WhoCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

WhoCommand::~WhoCommand () {}
WhoCommand::WhoCommand (Server* server) : _server(server) {}

void WhoCommand::execute(const Command& cmd, Client& sender) {
	(void)cmd;
	(void)sender;
}
