/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:10:53 by romanzdanov       #+#    #+#             */
/*   Updated: 2025/05/01 22:28:54 by rzhdanov         ###   ########.fr       */
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
	// std::cout << "INSIDE EXECUTE" << std::endl;
	// std::cout << sender.get_nick() << " is operator: "
	// 	<< channel->isOperator(sender.get_nick()) << std::endl;
	// channel->printInfo();
	if (!channel->isOperator(sender.get_nick())) {
		//TODO: probably better use not nicks, which can change, but fds, which are permanent
		//discuss internally. maybe I am overcomplicating things again.
		//TODO: use appropriate irc error code  here
		std::cout << "You are not an operator of this channel." << std::endl;
		sender.appendToOutBuffer("You are not an operator of this channel\r\n");
		return ;
		//NB: I am using the channel by reference, so I am dereferencing the pointer here
	}
	handleChannelMode(sender, *channel, args);
	channel->printInfo();
}

void ModeCommand::handleChannelMode(Client& sender, Channel& channel,
	const std::vector<std::string>& args)
{
	bool	addMode = true;
	//TODO: maybe add check if args[1] exists to avoid segfaults.
	// even though we can only get here if args[1] does exist.
	std::string					modeString;
	//TODO: make validModes argument a macro
	std::string					validModes = "itkol";
	size_t						argsSize = args.size();
	std::vector<std::string>	parameters;
	for (size_t i = 1; i < argsSize; ++i) {
		const std::string& part = args[i];
		if (!part.empty() && (part[0] == '+' || part[0] == '-')) {
			bool argIsValidMode = true;
			for (size_t j = 1; j < part.length(); ++j) {
				if(validModes.find(part[j]) == std::string::npos) {
					argIsValidMode = false;
					break;
				}
			}
			if (argIsValidMode) {
				modeString += part;
				continue;
			}	
		}
		parameters.push_back(part);
	}
	size_t modeStringLength = modeString.length();
	//TODO: add check for whether modeString is not empty
	// below is a stub, but better use error codes
	// if (!modeStringLength) {
	// 	std::cout << "no valid modes in command arguments" << std::endl;
	// 	return ;
	// }
	size_t paramIndex = 0;
	for (size_t i = 0; i < modeStringLength; ++i) {
		char ch = modeString[i];
		if (ch == '+') {
			addMode = true;
			continue;
		}
		else if (ch == '-')
		{
			addMode = false;
			continue;
		}
		if (ch == 'i') {
			channel.setInviteOnly(addMode);
			//TODO put info to the sender's buffer about changing channel's invite only status
		}
		else if (ch == 't') {
			channel.setProtectTopic(addMode);
			//TODO put info to the sender's buffer about changing channel's protected topic status
		}
		else if (ch == 'k') {
			if (addMode) {
				if (paramIndex >= parameters.size())
				{
					//TODO: use proper error code
					//TODO: send info to sender's buffer about needing more parameters
					std::cout << "NOT ENOUGH ARGS" << std::endl;
					return ;
				}
				channel.set_pass(parameters[paramIndex ++]);
				std::cout << "Channel password set. +k" << std::endl;
				//TODO: implement proper reply and add it to sender buffer 
			}
		}
		else if (ch == 'l') {
			if (addMode) {
				if (paramIndex >= parameters.size())
				{
					//TODO: use proper error code
					//TODO: send info to sender's buffer about needing more parameters
					std::cout << "NOT ENOUGH ARGS" << std::endl;
					return ;
				}
				int user_limit = std::atoi(parameters[paramIndex ++].c_str());
				//TODO: add behaviour if user_limit is not a valid int
				channel.set_userLimit(user_limit);
				std::cout << "user limit is set to " << user_limit
					<< std::endl;
				//TODO: add to outbuffer of sender
				//sender.appendToOutBuffer("+l");
			}
			else {
				channel.clear_userLimit();
				std::cout << "user limit removed " << std::endl;
				//TODO: add to outbuffer of sender
				//sender.appendToOutBuffer("-l");
			}
		}
		else if (ch == 'o') {
			if (paramIndex >= parameters.size())
			{
				//TODO: use proper error code
				//TODO: send info to sender's buffer about needing more parameters
				std::cout << "NOT ENOUGH ARGS" << std::endl;
				return ;
			}
			Server* server = channel.getServer();
			if (!server) {
				std::cout << "ERROR: Channel's server pointer is null." << std::endl;
				return;
			}
			Client* target = server->getClientByNick(parameters[paramIndex++]);
			if (!target || !channel.isMember(target->get_nick())) {
				std::cout << "User is not in channel" << std::endl;
				return ;
				//TODO: use proper error code
			}
			if (addMode) {
				//TODO: add message to the sender's outbuffer
				// sender.appendToOutBuffer("+o");
				channel.addOperator(target);
			}
			else {
				channel.remOperator(target->get_nick());
				//TODO: add message to the sender's outbuffer
				// sender.appendToOutBuffer("-o");	
			}
		}
		else {
			//TODO: use proper error message
			std::cout << "ERROR: Unknown mode: " << ch <<std::endl;
		}
	}
}