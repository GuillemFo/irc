/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:15:09 by josegar2          #+#    #+#             */
/*   Updated: 2025/05/04 20:09:58 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codes.hpp"

// Helper function to initialize the map
static std::map<std::string, std::string> createErrorFormats() {
    std::map<std::string, std::string> m;
    
    // Capability error. Not in RFC but usual in some servers
    m.insert(std::make_pair(ERR_INVALIDCAPCMD, "<client> <subcommand> :Invalid CAP command"));
    
    // Client errors
    m.insert(std::make_pair(ERR_NOTREGISTERED, "<client> <command> :You have not registered"));
    m.insert(std::make_pair(ERR_NOSUCHNICK, "<client> <nick> :No such nick/channel"));
    m.insert(std::make_pair(ERR_NOSUCHCHANNEL, "<client> <channel> :No such channel"));
    m.insert(std::make_pair(ERR_CANNOTSENDTOCHAN, "<client> <channel> :Cannot send to channel"));
    m.insert(std::make_pair(ERR_TOOMANYCHANNELS, "<client> <channel> :You have joined too many channels"));
    m.insert(std::make_pair(ERR_TOOMANYTARGETS, "<target> :Too many targets"));
    m.insert(std::make_pair(ERR_NORECIPIENT, "<client> :No recipient given (<command>)"));
    m.insert(std::make_pair(ERR_NOTEXTTOSEND, "<client> :No text to send"));
    m.insert(std::make_pair(ERR_INPUTTOOLONG, "<client> :Input line was too long"));
    m.insert(std::make_pair(ERR_UNKNOWNCOMMAND, "<client> <command> :Unknown command"));
    
    // Nickname errors
    m.insert(std::make_pair(ERR_NONICKNAMEGIVEN, "<client> :No nickname given"));
    m.insert(std::make_pair(ERR_ERRONEUSNICKNAME, "<client> <nick> :Erroneous nickname"));
    m.insert(std::make_pair(ERR_NICKNAMEINUSE, "<client> <nick> :Nickname is already in use"));
    m.insert(std::make_pair(ERR_NICKCOLLISION, "<client> <nick> :Nickname collision KILL"));
    
    // Channel errors
    m.insert(std::make_pair(ERR_NOTONCHANNEL, "<client> <channel> :You're not on that channel"));
    m.insert(std::make_pair(ERR_USERONCHANNEL, "<client> <nick> <channel> :is already on channel"));
    m.insert(std::make_pair(ERR_NEEDMOREPARAMS, "<client> <command> :Not enough parameters"));
    m.insert(std::make_pair(ERR_ALREADYREGISTERED, "<client> :You may not reregister"));
    m.insert(std::make_pair(ERR_PASSWDMISMATCH, "<client> :Password incorrect"));
    m.insert(std::make_pair(ERR_CHANNELISFULL, "<client> <channel> :Cannot join channel (+l)"));
    m.insert(std::make_pair(ERR_INVITEONLYCHAN, "<client> <channel> :Cannot join channel (+i)"));
    m.insert(std::make_pair(ERR_BADCHANNELKEY, "<client> <channel> :Cannot join channel (+k)"));
    m.insert(std::make_pair(ERR_BADCHANMASK, "<client> <channel> :Bad Channel Mask"));
    m.insert(std::make_pair(ERR_CHANOPRIVSNEEDED, "<client> <channel> :You're not channel operator"));
	m.insert(std::make_pair(ERR_USERNOTINCHANNEL, "<client> <nick> <channel> :They aren't on that channel")); //need to fix 04.05 08.09 pm

    
    return m;
}

std::string ircErrorText(const std::string& code, const Command& cmd, const Client& sender) {
	// map 
	static const std::map<std::string, std::string> errorFormats = createErrorFormats();

	std::map<std::string, std::string>::const_iterator it = errorFormats.find(code);
	if (it == errorFormats.end()) {
		return ""; // Or throw an exception for unknown codes. It shouldn't happen
	}
	std::string reply = it->second;
	std::vector<std::string> args = cmd.getArgs();
	const std::string cName = cmd.getName();

	// Replace <arguments>
	size_t pos;
		
	// Replace <client>
	if ((pos = reply.find("<client>")) != std::string::npos) {
		if (sender.get_nick().empty())
			reply.replace(pos, 8, "*");
		else
			reply.replace(pos, 8, sender.get_nick());
	}
		
	// Replace <nick>
	if ((pos = reply.find("<nick>")) != std::string::npos ) {
		reply.replace(pos, 6, args[0]);
	}
	
	// Replace <channel>
	if ((pos = reply.find("<channel>")) != std::string::npos) {
		if (cName == "INVITE")
			reply.replace(pos, 9, args[1]);
		else
			reply.replace(pos, 9, args[0]);
	}
		
	// Replace <command>
	if ((pos = reply.find("<command>")) != std::string::npos) {
		reply.replace(pos, 9, cName);
	}
		
	// Replace <subcommand>
	if ((pos = reply.find("<subcommand>")) != std::string::npos) {
		reply.replace(pos, 12, args[0]); //first parameter
	}
		
	// Replace <target> (for ERR_TOOMANYTARGETS) wich would be the 2n parameter in PRIVMSG
	if ((pos = reply.find("<target>")) != std::string::npos) {
		reply.replace(pos, 8, args[0]);
	}

	// Construct the full IRC message
	return ":" + sender.getServer()->getServerName() + " " + code + " " + reply + "\r\n";
}
