/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:10:53 by romanzdanov       #+#    #+#             */
/*   Updated: 2025/04/27 09:11:58 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCommand.hpp"
#include <iostream>

ModeCommand::ModeCommand() : _server(NULL) {}
ModeCommand::ModeCommand(Server* server) : _server(server) {}
ModeCommand::ModeCommand(const ModeCommand& src) : _server(src._server) {}
ModeCommand& ModeCommand::operator=(const ModeCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

ModeCommand::~ModeCommand() {}

//TODO: Start using standard irc error codes
void ModeCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.empty()) {
		std::cout << "No arguments provided." << std::endl;
		std::string errorMsg = ":irc.server.name 461 " + sender.get_nick()
			+ " :MODE Not enough parameters\r\n";
		sender.appendToOutBuffer(errorMsg);
		return ;
	}

	const std::string& target = args[0];
	
	if (target.empty()) {
		//TODO: do it with regular irc error codes
		std::cout << "Channel name is empty." << std::endl;
		sender.appendToOutBuffer("Channel name is empty\r\n");
		return ;
	}
//NB! we only check for # since the subject requires functionality for channels only
	if (target[0] != '#') {
		//TODO: do it with regular irc error codes
		std::cout << "Channel name must start with a \'#\'." << std::endl;
		sender.appendToOutBuffer("Invalid channel name\r\n");
		return ;
	}

	Channel* channel = _server->getChannel(target);
	if (!channel) {
		//TODO: use irc error code for that too
		std::cout << "Channel not found." << std::endl;
		sender.appendToOutBuffer("Channel not found\r\n");
		return ;
	}

	if (args.size() == 1) {
		//TODO: use irc reply code here.
		std::cout << "Printing cahnnel modes." << std::endl;
		//TODO: remove this print before submission of the project
		//TODO: add getModes() method to Channel class
		std::string modes = "itkol"; // Placeholder for actual channel modes. to be removed
		//std::string modes = channel->getModes();
		sender.appendToOutBuffer("The channel has the following modes "
			+ modes +"\r\n");
		return ;
	}
	if (!channel->isOperator(sender.get_nick())) {
		//TODO: probably better use not nicks, which can change, but fds, which are permanent
		//discuss internally. maybe I am overcomplicating things again.
		//TODO: use appropriate irc error code  here
		std::cout << "You are not an operator of this channel." << std::endl;
		sender.appendToOutBuffer("You are not an operator of this channel\r\n");
		return ;
		//NB: I am using the channel by reference, so I am dereferencing the pointer here
	}
	handleChannelMode(sender, *channel, cmd);
}

void ModeCommand::handleChannelMode(Client& sender, Channel& channel,
	const Command& cmd)
{
	(void) sender;
	(void) channel;
	(void) cmd;
	std::cout << "this is just a placeholder of he handleChannelMode"
		<< std::endl;
}