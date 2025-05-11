/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:26:55 by josegar2          #+#    #+#             */
/*   Updated: 2025/05/11 14:05:21 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codes.hpp"

// Helper function to initialize the map
static std::map<std::string, std::string> initReplyFormats() {
    std::map<std::string, std::string> m;
    
    // Registration replies
    m.insert(std::make_pair(RPL_WELCOME, "<client> :Welcome to the Internet Relay Network <client>"));
    m.insert(std::make_pair(RPL_YOURHOST, "<client> :Your host is <servername>, running version <ver>"));
    m.insert(std::make_pair(RPL_CREATED, "<client> :This server was created <date>"));
    m.insert(std::make_pair(RPL_MYINFO, "<client> <servername> <version> <usermodes> <chanmodes>"));
    m.insert(std::make_pair(RPL_ISUPPORT, "<client> <tokens> :are supported by this server"));
    
    // MOTD replies
    m.insert(std::make_pair(RPL_MOTDSTART, "<client> :- <servername> Message of the Day -"));
    m.insert(std::make_pair(RPL_MOTD, "<client> :- <text>"));
    m.insert(std::make_pair(RPL_ENDOFMOTD, "<client> :End of /MOTD command"));
    
    // Channel replies
    m.insert(std::make_pair(RPL_NOTOPIC, "<client> <channel> :No topic is set"));
    m.insert(std::make_pair(RPL_TOPIC, "<client> <channel> :<topic>"));
    m.insert(std::make_pair(RPL_TOPICWHOTIME, "<client> <channel> <setter> <timestamp>"));
    m.insert(std::make_pair(RPL_NAMREPLY, "<client> <symbol> <channel> :")); // [prefix]<nick> [prefix]<nick>...
    m.insert(std::make_pair(RPL_ENDOFNAMES, "<client> <channel> :End of /NAMES list"));
    m.insert(std::make_pair(RPL_CHANNELMODEIS, "<client> <channel> <mode> <mode-params>"));
    m.insert(std::make_pair(RPL_INVITING, "<client> <nick> <channel>"));
    
    return m;
}

std::string ircReplyText(const std::string& code, const Command& cmd, const Client& sender) {
	// map 
	static const std::map<std::string, std::string> replyFormats = initReplyFormats();

	std::map<std::string, std::string>::const_iterator it = replyFormats.find(code);
	if (it == replyFormats.end()) {
		return ""; // Or throw an exception for unknown codes. It shouldn't happen
	}
	std::string reply = it->second;
	std::vector<std::string> args = cmd.getArgs();
	const std::string cName = cmd.getName();

	// Replace <arguments>
	size_t pos;
		
	// Replace <client>
	if ((pos = reply.find("<client>")) != std::string::npos) {
		reply.replace(pos, 8, sender.get_nick());
	}
		
	// Replace <channel>
	if ((pos = reply.find("<channel>")) != std::string::npos) {
		if (cName == "INVITE")
		reply.replace(pos, 9, args[1]);
		else
		reply.replace(pos, 9, args[0]);
	}
	
	// Replace <symbol>
	if ((pos = reply.find("<symbol>")) != std::string::npos) {
		reply.replace(pos, 8, "=");
	}
		

	if (code == RPL_NAMREPLY) { //construct all namereply needed
		std::vector<std::string> nickList = sender.getServer()->getChannel(args[0])->getNicks();
		reply = ":" + sender.getServer()->getServerName() + " " + code + " " + reply;
		std::string linereply = reply;
		size_t lineSize = linereply.size();
		std::string fullreply;
		for(size_t i = 0; i < nickList.size(); ++i) {
			if ((lineSize + 1 + nickList[i].size()) > 510) {//limit size of the line
				fullreply = fullreply + linereply + "\r\n";
				linereply = reply;
				lineSize = linereply.size();
			}
			if (lineSize > reply.size()) {  //some nick added
				linereply = linereply + " ";
				lineSize++;
			}
			linereply = linereply + nickList[i];
			lineSize += nickList[i].size();
		}
		if (lineSize > reply.size())
			fullreply = fullreply + linereply + "\r\n";
		return fullreply;
	}

	if (code == RPL_CHANNELMODEIS) {
		Channel* channel = sender.getServer()->getChannel(args[0]);
		if (!channel) {
			// This shouldn't happen
			return "";
		}
		std::string modes = "+";
		std::string parameters;
		if (channel->isInviteOnly()) {
			modes += "i";
		}
		if (channel->isTopicProtected()) 
		{
			modes += "t";
		}
		if (channel->isPassRequired()) {
			if (channel->isOperator(name_tolower(sender.get_nick()))) {
				modes += "k";
				parameters += " " + channel->get_pass();
			}
		}
		int channel_user_limit = channel->get_userLimit();
		if (channel_user_limit) {
			modes += "l";
			parameters += " " + int_to_string(channel_user_limit);
		}
		std::string reply = ":" + sender.getServer()->getServerName() + " "
			+ code + " " + sender.get_nick() + " " + args[0] + " "
			+ modes + parameters + "\r\n";
			return reply;
	}

	// Replace <nick>
	if ((pos = reply.find("<nick>")) != std::string::npos) {
		reply.replace(pos, 6, args[0]);
	}
			
	// Replace <tokens> for RPL_ISUPPORT
	if ((pos = reply.find("<tokens>")) != std::string::npos) {
		reply.replace(pos, 8, "CHANNELLEN=64 CHANNELMODES=i,t,k,o,l NICKLEN=9");
	}
	if ((pos = reply.find("<client>")) != std::string::npos) {
		reply.replace(pos, 8, sender.get_nick());
	}
	if ((pos = reply.find("<ver>")) != std::string::npos) {
		reply.replace(pos, 5, "1.0");
	}
		
	// Replace <topic> If empty NOTOPIC should be called
	if ((pos = reply.find("<topic>")) != std::string::npos) {
			reply.replace(pos, 7, sender.getServer()->getChannel(args[0])->get_topic());		
	}
	if ((pos = reply.find("<text>")) != std::string::npos) {
			reply.replace(pos, 6, "Today is a good day :)");		
	}

	// Replace <mode> <mode-params> If empty NOTOPIC should be called
	if ((pos = reply.find("<mode> <mode-params>")) != std::string::npos) {
			reply.replace(pos, 20, sender.getServer()->getChannel(args[0])->getModesSet());		
	}

	if ((pos = reply.find("<servername>")) != std::string::npos) {
		reply.replace(pos, 12, sender.getServer()->getServerName());
	}
	//  ****** SOME <args> missing for replacement
	reply = ":" + sender.getServer()->getServerName() + " " + code + " " + reply;
	// Construct the full IRC message
	if (reply.length() > 510)
		reply = reply.substr(0, 510);
	return reply + "\r\n";
}
