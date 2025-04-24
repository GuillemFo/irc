/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:26:55 by josegar2          #+#    #+#             */
/*   Updated: 2025/04/24 12:18:44 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"
#include "Codes.hpp"

class Command; // Forward declaration

std::string ircReplyText(const std::string& code, const Command& cmd) {
	// map 
	static const std::map<std::string, std::string> replyFormats = {
		// Registration
		{RPL_WELCOME,      "<nick> :Welcome to the Internet Relay Network <client>"},
		{RPL_YOURHOST,     "<nick> :Your host is <servername>, running version <ver>"},
		{RPL_CREATED,      "<nick> :This server was created <date>"},
		{RPL_MYINFO,       "<nick> <servername> <version> <usermodes> <chanmodes>"},
		{RPL_ISUPPORT,     "<nick> <tokens> :are supported by this server"},

		// MOTD
		{RPL_MOTDSTART,    "<nick> :- <servername> Message of the Day -"},
		{RPL_MOTD,         "<nick> :- <text>"},
		{RPL_ENDOFMOTD,    "<nick> :End of /MOTD command"},

		// Channel
		{RPL_NOTOPIC,      "<nick> <channel> :No topic is set"},
		{RPL_TOPIC,        "<nick> <channel> :<topic>"},
		{RPL_TOPICWHOTIME, "<nick> <channel> <setter> <timestamp>"}, // setter nick
		{RPL_NAMREPLY,     "<nick> <symbol> <channel> :[prefix]<nick> [prefix]<nick>..."},
		{RPL_ENDOFNAMES,   "<nick> <channel> :End of /NAMES list"},
		{RPL_CHANNELMODEIS,"<nick> <channel> <modes> <mode-params>"},
		{RPL_INVITING,     "<nick> <invited> <channel>"} // invited nick 

	};

	std::map<std::string, std::string>::iterator it = replyFormats.find(code);
	if (it == replyFormats.end()) {
		return ""; // Or throw an exception for unknown codes. It shouldn't happen
	}
	std::string reply = it->second;
		
	// Replace <arguments>
	size_t pos;
		
	// Replace <client>
	if ((pos = reply.find("<client>")) != std::string::npos) {
		reply.replace(pos, 8, cmd.getClient());
	}
		
	// Replace <nick>
	if (pos = reply.find("<nick>")) != std::string::npos {
		reply.replace(pos, 6, cmd.getNick());
	}
		
	// Replace <channel>
	if ((pos = reply.find("<channel>")) != std::string::npos) {
		reply.replace(pos, 9, cmd.getChannel());
	}
		
	// Replace <target> (for ERR_TOOMANYTARGETS) wich would be the 2n parameter in PRIVMSG
	if ((pos = reply.find("<topic>")) != std::string::npos) {
		reply.replace(pos, 7, cmd.getTarget());
	}
	//  ****** SOME <args> missing for replacement

	// Construct the full IRC message
	return ":" + cmd.getServerName() + " " + code + " " + reply + "\r\n";
}