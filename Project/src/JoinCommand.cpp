/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 21:42:22 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/04 20:25:27 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "JoinCommand.hpp"
# include "CommandHandler.hpp"
# include <iostream>

JoinCommand::JoinCommand() : _server(NULL) {}
JoinCommand::JoinCommand(const JoinCommand& src) {
	this->_server = src._server;
}
JoinCommand& JoinCommand::operator=(const JoinCommand& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

JoinCommand::~JoinCommand () {}
JoinCommand::JoinCommand (Server* server) : _server(server) {}

// channel    =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring
// [ ":" chanstring ]
// chanstring = any octet except NUL, BELL, CR, LF, " ", "," and ":"
bool JoinCommand::isValidChannelName(const std::string& name) {
	if (name.empty() || 
			(name[0] != '#' && name[0] != '&') ||
			name.length() > CHANNELLEN ||
			!strchr(CHANTYPES, name[0]))
		return false;
	for (size_t i = 1; i < name.length(); ++i) {
		char c = name[i];
		if (c == ' ' || c == ',' || c == ':' ||
			c == 0x00 || c == 0x07 || c == 0x0D || c == 0x0A) {
			return false;
		}
	}
	return true;
}

void joinChannel(const Command& cmd, Client& sender)
{
	const std::vector<std::string>& args = cmd.getArgs();
	const std::string& channelName = args[0];
	if (!JoinCommand::isValidChannelName(channelName)) {
		sender.sendMessage(ircErrorText(ERR_BADCHANMASK, cmd, sender));
		return ;
	}
	//std::cout << "Executing JOIN command. Joining channel: "
	//	<< channelName << std::endl;
	if (!sender.getServer()->channelExists(channelName))
	{
		sender.getServer()->addChannelMap(channelName);  // add new channel into the server channel map
	} else { //exisitng channel. See if it's possible to join
		if (sender.getServer()->getChannel(channelName)->isPassRequired())
		{
			if (args.size() < 2 || args[1].empty()) {  // no password provided
				sender.sendMessage(ircErrorText(ERR_BADCHANNELKEY, cmd, sender));
				return ;
			}
			const std::string& pass = args[1];
			if (!sender.getServer()->getChannel(channelName)->check_pass(pass)) 
			{ //incorrect password
				sender.sendMessage(ircErrorText(ERR_BADCHANNELKEY, cmd, sender));
				return ;
			}
		}
		// No password required or is correct here
		if (sender.getServer()->getChannel(channelName)->isMember(sender.get_nick()))
		{ // already in, no need to send anything acording to RFC
			// sender.sendMessage(ircErrorText(ERR_USERONCHANNEL, cmd, sender));
			return ;
		}
		else if (sender.getServer()->getChannel(channelName)->isInviteOnly())
		{ // can't join without the INVITE
			sender.sendMessage(ircErrorText(ERR_INVITEONLYCHAN, cmd, sender));
			return ;
		}
		else if (sender.getServer()->getChannel(channelName)->isChannelFull())
		{ // channel full
			sender.sendMessage(ircErrorText(ERR_CHANNELISFULL, cmd, sender));
			return ;
		}

	}
	sender.getServer()->getChannel(channelName)->addClient(&sender);
	sender.addChannel(sender.getServer()->getChannel(channelName));
	//sender.addOutMessage(":" + sender.get_nick() + "!" + sender.get_user() + "@" + "host JOIN " + channelName + "\r\n");
	sender.getServer()->getChannel(channelName)->broadcast(":" + sender.get_nick() + "!" + sender.get_user() + "@" + "host JOIN " + channelName + "\r\n");
	// TODO RPL_TOPIC,  RPL_NAMREPLY,  RPL_ENDOFNAMES
	if (sender.getServer()->getChannel(args[0])->get_topic().empty())
		sender.addOutMessage(ircReplyText(RPL_NOTOPIC, cmd, sender));
	else
		sender.addOutMessage(ircReplyText(RPL_TOPIC, cmd, sender));
	sender.addOutMessage(ircReplyText(RPL_NAMREPLY, cmd, sender));
	sender.sendMessage(ircReplyText(RPL_ENDOFNAMES, cmd, sender));
	return ;
}

void JoinCommand::execute(const Command& cmd, Client& sender) {
	(void) sender;
	const std::vector<std::string>& args = cmd.getArgs();
	// TODO: implement check for gettin cmd and/or sender as NULL
	if (args.empty()) {
		std::cout << "No arguments in the JOIN command. Aborting." << std::endl;
		sender.sendMessage(ircErrorText(ERR_NEEDMOREPARAMS, cmd, sender));
		return ;
	}
	if (!sender.getOkLogin())
	{
		sender.sendMessage(ircErrorText(ERR_NOTREGISTERED, cmd, sender));
		return ;
	}
	//need a checker for args 1 2 etc to loop the args because it can contain a password;
	if (args[0] == "0") {
		std::string allChannels = sender.getListOfChannels();
		if (!allChannels.empty()) {

			Command partAll;
			partAll.setName("PART");
			partAll.addArg(allChannels);
			sender.getServer()->_dispatcher.dispatch(partAll, sender);
		}
		return ;
	}
	Command splitcmd(cmd);
	std::vector<std::string> channels = split_arg(args[0]);
	std::vector<std::string> keys; 
	if (args.size() > 1)
		keys = split_arg(args[1]);
	std::vector<std::string>::const_iterator itc = channels.begin();
	std::vector<std::string>::const_iterator itk = keys.begin();
	while (itc != channels.end()) {
		splitcmd.clearArgs();
		splitcmd.addArg(*itc);
		++itc;
		if (itk != keys.end())
		{
			if (!(*itk).empty()) 
				splitcmd.addArg(*itk);
			++itk;
		}
		joinChannel(splitcmd, sender);
	}
	return ;
}
