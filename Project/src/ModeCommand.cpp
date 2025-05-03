/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:10:53 by romanzdanov       #+#    #+#             */
/*   Updated: 2025/05/02 22:38:36 by rzhdanov         ###   ########.fr       */
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

std::string ModeCommand::int_to_string(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void ModeCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.empty()) {
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return ;
	}

	const std::string& target = args[0];
	
	if (target.empty()) {
		sender.sendMessage(ircErrorText(ERR_NOSUCHCHANNEL, cmd, sender));
		return ;
	}
//NB! we only check for # since the subject requires functionality for channels only
	if (target[0] != '#') {
		sender.sendMessage(ircErrorText(ERR_NOSUCHCHANNEL, cmd, sender));
		return ;
	}

	Channel* channel = _server->getChannel(target);
	if (!channel) {
		sender.sendMessage(ircErrorText(ERR_NOSUCHCHANNEL, cmd, sender));
		return ;
	}

	if (args.size() == 1) {
		std::string modes = "itkol";
		sender.addOutMessage("The channel has the following modes " \
			+ modes +"\r\n");
			return ;
		//TODO: make changes to the ircReplyText to  
		// sender.sendMessage(ircReplyText(RPL_CHANNELMODEIS, cmd, sender));
	}
	if (!channel->isOperator(sender.get_nick())) {
		sender.sendMessage(ircErrorText(ERR_CHANOPRIVSNEEDED, cmd, sender));
		return ;
	}
	handleChannelMode(sender, *channel, cmd);
}

void ModeCommand::handleChannelMode(Client& sender, Channel& channel,
	const Command& cmd)
{
	std::vector<std::string> args = cmd.getArgs();

	bool	addMode = true;
	//TODO: maybe add check if args[1] exists to avoid segfaults.
	// even though we can only get here if args[1] does exist.
	std::string					modeString;
	//TODO: make validModes argument a macro
	std::string					validModes = "itkol";
	size_t						argsSize = args.size();
	// std::vector<std::string>	parameters;
	modeString += args[1];
	// for (size_t i = 1; i < argsSize; ++i) {
	// 	const std::string& part = args[i];
	// 	if (!part.empty() && (part[0] == '+' || part[0] == '-')) {
	// 		bool argIsValidMode = true;
	// 		for (size_t j = 1; j < part.length(); ++j) {
	// 			if(validModes.find(part[j]) == std::string::npos) {
	// 				//TODO: implement 472 ERR_UNKNOWNMODE here
	// 				sender.sendMessage(ircErrorText(ERR_UNKNOWNCOMMAND, ))
	// 				argIsValidMode = false;
	// 				break;
	// 			}
	// 		}
	// 		if (argIsValidMode) {
	// 			modeString += part;
	// 			continue;
	// 		}	
	// 	}
	// 	parameters.push_back(part);
	// }
	size_t modeStringLength = modeString.length();
	//TODO: add check for whether modeString is not empty
	// below is a stub, but better use error codes
	// if (!modeStringLength) {
	// 	std::cout << "no valid modes in command arguments" << std::endl;
	// 	return ;
	// }
	size_t paramIndex = 2;
	std::string modeChanges;
	std::string modeParameters;
	for (size_t i = 0; i < modeStringLength; ++i) {
		char ch = modeString[i];
		if (ch == '+') {
			addMode = true;
			modeChanges += ch;
			continue;
		}
		else if (ch == '-')
		{
			addMode = false;
			modeChanges += ch;
			continue;
		}
		modeChanges += ch;
		if (ch == 'i') {
			channel.setInviteOnly(addMode);
		}
		else if (ch == 't') {
			channel.setProtectTopic(addMode);
		}
		else if (ch == 'k') {
			if (addMode) {
				if (paramIndex >= argsSize)
				{
					sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS,
													cmd, sender));
					return ;
				}
				channel.set_pass(args[paramIndex]);
				modeParameters += args[paramIndex ++] + " ";
			}
			else {
				std::string nullPassword = "";
				channel.set_pass(nullPassword);
			}
		}
		else if (ch == 'l') {
			if (addMode) {
				if (paramIndex >= argsSize)
				{
					sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS,
													cmd, sender));
					return ;
				}
				int user_limit = std::atoi(args[paramIndex ++].c_str());
				//TODO: add behaviour if user_limit is not a valid int
				channel.set_userLimit(user_limit);
				modeParameters += int_to_string(user_limit) + " ";
			}
			else {
				channel.clear_userLimit();
			}
		}
		else if (ch == 'o') {
			if (paramIndex >= argsSize)
			{
				sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd,
												sender));
				return ;
			}
			Server* server = channel.getServer();
			if (!server) {
				// std::cout << "AAAAAAAAAAAAA SERVER IS NULL!!!" << std::endl;
				//TODO: this situation should never happen under standard operation
				// of a server. so behaviour must be something exceptional
				// though terminating connection because of that would be 
				// too extreme. IMPLEMENT APPROPRIATE BEHAVIOUR
				// sending err_msg is probably just a placeholder. TBD
				// also returning here means we do not get to send to the user the 
				// modeChanges string. So at least this has to be changed.
				std::string err_msg = "INTERNAL ERROR: could not process. \
o argument. Please report this bug to server admins in #bug_reports channel";
				sender.sendMessage(err_msg);
				std::cout << "ERROR: Channel's server pointer is null." << std::endl;
				return;
			}
			// std::cout << "ZZZZZZZZZZZ\n" << std::endl;
			// std::cout << args[paramIndex] << std::endl;
			// std::cout << server->getClientByNick(args[paramIndex]) << std::endl;
			//NB: finally found the issue (I think):
			//client is added to a channel, but not added to a server
			Client* target = server->getClientByNick(args[paramIndex++]);
			if (!target || !channel.isMember(target->get_nick())) {
				sender.sendMessage(ircErrorText(ERR_NOTONCHANNEL, cmd,
												sender));
				return ;
			}
			if (addMode) {
				channel.addOperator(target);
			}
			else {
				channel.remOperator(target->get_nick());
			}
			modeParameters += target->get_nick() + " ";
		}
		else {
			sender.sendMessage(ircErrorText(ERR_UNKNOWNCOMMAND, cmd, sender));
			//TODO: replace with ERR_UNKNOWN MODE after I implement it
		}
	}
	//TODO: refactor the adhoc broadcast below:
	std::string message = ":" + sender.get_nick() + "!" + sender.get_user()
		+ "@" + sender.get_host() + " " + modeChanges
		+ (modeParameters.empty() ? "" : " " + modeParameters) + "\r\n";
	channel.broadcast(message);
}