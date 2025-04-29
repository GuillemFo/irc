/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:41:59 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/29 07:14:00 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "QuitCommand.hpp"
# include <iostream>

QuitCommand::QuitCommand() : _server(NULL) {}
QuitCommand::QuitCommand(const QuitCommand& src) {
	this->_server = src._server;
}
QuitCommand& QuitCommand::operator=(const QuitCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

QuitCommand::~QuitCommand () {}
QuitCommand::QuitCommand (Server* server) : _server(server) {}
Server				*QuitCommand::getServer() const{return (this->_server);}


void QuitCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL

	// Redoing 29.04.25 09.13am
	if (args.empty()) {
		sender._out.addMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		std::cout << sender._out.getMessage() << std::endl;
		sender.cl_Epoll_In_Out();
		return ;
	}
	const std::string& Quit = args[0];	
	std::string Quit_out =  ":" + sender.get_nick() + " QUIT " + " " + Quit + "\r\n";
	sender._out.addMessage(Quit_out);
	sender.cl_Epoll_In_Out();
	return ;

}