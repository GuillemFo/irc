/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:41:59 by gforns-s          #+#    #+#             */
/*   Updated: 2025/04/29 01:28:04 by codespace        ###   ########.fr       */
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




	if (args.empty()) {
		//quit without msg
		return ;
	}
	const std::string& Quit = args[0];
	if (!Quit.empty()) {
	//	others will see: * bobitosan1 has quit (Client Quit) the command received by sv will be "QUIT :Client Quit"
	//	only need to send 1 time per clien not all channels/
			std::string quitLine = ":" + sender.get_nick() + " QUIT :";
			if (args.size() >= 2)
				quitLine += args[1];
			else
				quitLine += "Client Quit1";
			quitLine += "\r\n";
			sender.sendMessage(quitLine);
			std::map<std::string, Channel*>::iterator it;
			for (it = sender.getChannels().begin(); it != sender.getChannels().end(); ++it)// get channels
			{
				std::string channel = it->first;
				
				sender.getServer()->getChannel(channel)->broadcast(quitLine);	//not working...
			}
			return ;
			//quit with specific message
			std::cout << "Executing Quit command." << std::endl;
	}
	else {
			//should never happen? 
		sender.sendMessage(ircErrorText(ERR_PASSWDMISMATCH, cmd, sender));	//temp error.
		return ;
	}
}