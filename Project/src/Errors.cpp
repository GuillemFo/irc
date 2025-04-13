/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josegar2 <josegar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:15:09 by josegar2          #+#    #+#             */
/*   Updated: 2025/04/13 20:15:22 by josegar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"
#include "Codes.hpp"

class Command; // Forward declaration

std::string ircReplyText(const std::string& code, const Command& cmd) {
	// map 
	static const std::map<std::string, std::string> replyFormats = {
	// Client errors
	{ERR_NOSUCHNICK,		"<client> <nickname> :No such nick/channel"},
	{ERR_NOSUCHCHANNEL,	 "<client> <channel> :No such channel"},
	{ERR_CANNOTSENDTOCHAN,  "<client> <channel> :Cannot send to channel"},
	{ERR_TOOMANYCHANNELS,   "<client> <channel> :You have joined too many channels"},
	{ERR_TOOMANYTARGETS,	"<target> :Too many targets"},
	{ERR_NORECIPIENT,	   "<client> :No recipient given (<command>)"},
	{ERR_NOTEXTTOSEND,	  "<client> :No text to send"},
	{ERR_INPUTTOOLONG,	  "<client> :Input line was too long"},
	{ERR_UNKNOWNCOMMAND,	"<client> <command> :Unknown command"},
		
	// Nickname errors
	{ERR_NONICKNAMEGIVEN,   "<client> :No nickname given"},
	{ERR_ERRONEUSNICKNAME,  "<client> <nick> :Erroneous nickname"},
	{ERR_NICKNAMEINUSE,	 "<client> <nick> :Nickname is already in use"},
	{ERR_NICKCOLLISION,	 "<client> <nick> :Nickname collision KILL"},
		
	// Channel errors
	{ERR_NOTONCHANNEL,	  "<client> <channel> :You're not on that channel"},
	{ERR_USERONCHANNEL,	 "<client> <nick> <channel> :is already on channel"},
	{ERR_NEEDMOREPARAMS,	"<client> <command> :Not enough parameters"},
	{ERR_ALREADYREGISTERED, "<client> :You may not reregister"},
	{ERR_PASSWDMISMATCH,	"<client> :Password incorrect"},
	{ERR_CHANNELISFULL,	 "<client> <channel> :Cannot join channel (+l)"},
	{ERR_INVITEONLYCHAN,	"<client> <channel> :Cannot join channel (+i)"},
	{ERR_BADCHANNELKEY,	 "<client> <channel> :Cannot join channel (+k)"},
	{ERR_BADCHANMASK,	   "<client> <channel> :Bad Channel Mask"},
	{ERR_CHANOPRIVSNEEDED,  "<client> <channel> :You're not channel operator"}

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
		
	// Replace <command>
	if ((pos = reply.find("<command>")) != std::string::npos) {
		reply.replace(pos, 9, cmd.getCommand());
	}
		
	// Replace <target> (for ERR_TOOMANYTARGETS) wich would be the 2n parameter in PRIVMSG
	if ((pos = reply.find("<target>")) != std::string::npos) {
		reply.replace(pos, 8, cmd.getTarget());
	}

	// Construct the full IRC message
	return ":" + cmd.getServerName() + " " + code + " " + reply + "\r\n";
}