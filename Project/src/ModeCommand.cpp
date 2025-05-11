/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:10:53 by romanzdanov       #+#    #+#             */
/*   Updated: 2025/05/11 14:00:32 by josegar2         ###   ########.fr       */
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

void ModeCommand::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.empty()) {
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return ;
	}

	const std::string& target = args[0];
	
	if (!sender.getServer()->channelExists(target)) {
		sender.sendMessage(ircErrorText(ERR_NOSUCHCHANNEL, cmd, sender));
		return ;
	}

	Channel* channel = _server->getChannel(target);
	if (!channel) {
		sender.sendMessage(ircErrorText(ERR_NOSUCHCHANNEL, cmd, sender));
		return ;
	}
	if (!channel->isMember(sender.get_nick())) {
		sender.sendMessage(ircErrorText(ERR_USERNOTINCHANNEL, cmd, sender));
		return ;
	}
	if (args.size() == 1 ) {
		sender.sendMessage(ircReplyText(RPL_CHANNELMODEIS, cmd, sender));
			return ;
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
	std::string					modeString;
	std::string					validModes = "itkol";
	size_t						argsSize = args.size();
	modeString += args[1];

	size_t modeStringLength = modeString.length();
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
				if (user_limit < 1) {
					sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS,
													cmd, sender));
					return ;
				}
				else {
					channel.set_userLimit(user_limit);
					modeParameters += int_to_string(user_limit) + " ";
				}
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
			Client* target = server->getClientByNick(args[paramIndex++]);
			if (target == NULL || !target || !channel.isMember(target->get_nick())) {
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
			sender.sendMessage(ircErrorText(ERR_UNKNOWNMODE, cmd, sender));
		}
	}
	std::string message = formatModeChangeMessage(sender, channel, modeChanges,
		modeParameters);
	channel.broadcast(message);
}

std::string ModeCommand::formatModeChangeMessage(const Client& sender,
	const Channel& channel,
	const std::string& modeChanges,
	const std::string& modeParameters) {
	std::string msg = ":" + sender.get_nick() + "!" + sender.get_user() +
	"@" + sender.get_host() + " MODE " + channel.get_name() + " " +
	modeChanges;

	if (!modeParameters.empty())
	msg += " " + modeParameters;

	return msg + "\r\n";
}
