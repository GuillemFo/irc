/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:29:12 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/25 11:16:00 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "PingCommand.hpp"
# include <iostream>

PingCommand::PingCommand() : _server(NULL) {}
PingCommand::PingCommand(const PingCommand& src) {
	this->_server = src._server;
}
PingCommand& PingCommand::operator=(const PingCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

PingCommand::~PingCommand () {}
PingCommand::PingCommand (Server* server) : _server(server) {}


void PingCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		sender._out.addMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		std::cout << sender._out.getMessage() << std::endl;
		sender.cl_Epoll_In_Out();
		return ;
	}
	const std::string& Ping = args[0];
	std::cout << "test:" << Ping << ":" << std::endl;
	//std::string pong_out =  ":" + sender.get_nick() + " PONG " + sender.getServer()->getServerName() + " " + Ping + "\r\n"; // no need to send server name
	std::string pong_out =  ":" + sender.get_nick() + " PONG " + " " + Ping + "\r\n";
	sender._out.addMessage(pong_out);
	sender.cl_Epoll_In_Out();
	return ;

/*
	}
	else {
		sender._out.addMessage(ircErrorText("ping error", cmd, sender));
		std::cout << sender._out.getMessage() << std::endl;
		sender.cl_Epoll_In_Out();

		return ;
	}
*/
}
