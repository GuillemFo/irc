/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 13:39:36 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/24 18:12:30 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivmsgCommand.hpp"
#include <iostream>

PrivmsgCommand::PrivmsgCommand() : _server(NULL) {}
PrivmsgCommand::PrivmsgCommand(Server* server) : _server(server) {}
PrivmsgCommand::PrivmsgCommand(const PrivmsgCommand& src) {
	this->_server = src._server;
}
PrivmsgCommand& PrivmsgCommand::operator=(const PrivmsgCommand& src) {
	if (this != &src) {
	this->_server = src._server;
	}
	return *this;
}
PrivmsgCommand::~PrivmsgCommand() {}
Server				*PrivmsgCommand::getServer() const{return (this->_server);}

void PrivmsgCommand::execute(const Command& cmd, Client& sender) {
	(void) sender;
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.size() < 1) {
		std::cout << "PRIVMSG usage: PRIVMSG <target> :<message>" << std::endl;
		std::cout << "argumets given: 0." << std::endl;
		return ; // TODO: updated functionality so that error message is sent back to the clien
	}
	else if (args.size() < 2) {
		std::cout << "PRIVMSG usage: PRIVMSG <target> :<message>";
		std::cout << "arguments give: 1." << std::endl;
		return ; // TODO: updated functionality so that error message is sent back to the clien
	}
	std::string target = args[0];
	std::string message;
	// the if block below is optional. we can just send back an error if args.size() > 2
	if (args.size() > 2) {
		for (size_t i = 1; i < args.size(); ++i) {
			message += args[i];
			if (i + 1 < args.size())
				message += " ";
		}
	}
	else {
		message = args[1];
	}
	
	// if not found, send back error CLIENT NOT FOUND
	
	// if(!recipient) {
	// sender.sendError("Nick not found on the server.")
	//}
	
	std::string prefix = ":" + sender.get_nick() + "!" + sender.get_user() + "@" + "localhost";
	std::string privmsgLine = prefix + " PRIVMSG " + target + " :" + message + "\r\n";
	this->getServer()->getClientByNick(target)->_out.addMessage(privmsgLine);
	this->getServer()->getClientByNick(target)->cl_Epoll_In_Out(); //to set the epoll in and out


	//std::cout << "Command tested: PRIVMSG" << std::endl;
	//std::cout << "Target is: " << target << ", message is: " << message
	//<< std::endl;
}
