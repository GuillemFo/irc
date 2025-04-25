/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Codes.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gforns-s <gforns-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:19:06 by josegar2          #+#    #+#             */
/*   Updated: 2025/04/25 17:33:28 by gforns-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#include <map>
#include "Command.hpp"
#include "Client.hpp"

/********* ERRORS ***********/
/* Generic errors */
#define ERR_INPUTTOOLONG		"417"
#define ERR_UNKNOWNCOMMAND		"422"

/* Capability Errors */
#define ERR_INVALIDCAPCMD		"410"  // Not supported parameters for CAP

/* Registration/Connection Errors */
#define ERR_NEEDMOREPARAMS		"461"  // Not enough parameters for command
#define ERR_ALREADYREGISTERED 	"462"  // Client already registered
#define ERR_PASSWDMISMATCH		"464"  // Incorrect password

/* Nickname Errors */
#define ERR_NONICKNAMEGIVEN		"431"  // No nickname provided
#define ERR_ERRONEUSNICKNAME	"432"  // Invalid nickname format
#define ERR_NICKNAMEINUSE		"433"  // Nickname already in use
#define ERR_NICKCOLLISION		"436"  // Nickname collision (server conflict)

/* Channel Join Errors */
#define ERR_NOSUCHCHANNEL		"403"  // Channel doesn't exist
#define ERR_TOOMANYCHANNELS 	"405"  // Client joined too many channels
#define ERR_BADCHANNELKEY		"475"  // Wrong channel password
#define ERR_CHANNELISFULL		"471"  // Channel is at user limit (+l)
#define ERR_INVITEONLYCHAN		"473"  // Channel is invite-only (+i)
#define ERR_BADCHANMASK			"476"  // Invalid channel name format

/* Channel Permission Errors */
#define ERR_USERONCHANNEL		"443"  // nick already present on channel

/* Channel Permission Errors */
#define ERR_NOTONCHANNEL		"442"  // Client not in channel
#define ERR_CHANOPRIVSNEEDED	"482"  // Client isn't a channel operator

/* Message/PRIVMSG Errors */
#define ERR_NOSUCHNICK			"401"  // Target nick/channel doesn't exist
#define ERR_TOOMANYTARGETS		"407"  // Too many recipients
#define ERR_NORECIPIENT			"411"  // No target specified
#define ERR_NOTEXTTOSEND		"412"  // No message body
#define ERR_CANNOTSENDTOCHAN	"404"  // Cannot send to channel (e.g., not joined)

/********* REPLIES ***********/
/* Registration Sequence */
#define RPL_WELCOME				"001"  // ":Welcome to the Internet Relay Network <nick>!<user>@<host>"
#define RPL_YOURHOST			"002"  // ":Your host is <servername>, running version <ver>"
#define RPL_CREATED				"003"  // ":This server was created <date>"
#define RPL_MYINFO				"004"  // "<servername> <version> <usermodes> <chanmodes>"
#define RPL_ISUPPORT			"005"  // "<tokens> :are supported by this server" (IRCv3)

/* MOTD (Message of the Day) */
#define RPL_MOTDSTART			"375"  // ":- <servername> Message of the Day -"
#define RPL_MOTD				"372"  // ":- <text>"
#define RPL_ENDOFMOTD			"376"  // ":End of /MOTD command"

/* Channel Info Replies */
#define RPL_TOPIC				"332"  // Channel topic
#define RPL_NOTOPIC				"331"  // No topic set
#define RPL_TOPICWHOTIME		"333"  // Who set the topic and when
#define RPL_NAMREPLY			"353"  // List of users in channel
#define RPL_ENDOFNAMES			"366"  // End of NAMES list
#define RPL_CHANNELMODEIS		"324"  // Current channel modes

/* Invite Reply */
#define RPL_INVITING			"341"  // Successfully invited user

std::string ircErrorText(const std::string& code, const Command& cmd, const Client& sender);
std::string ircReplyText(const std::string& code, const Command& cmd, const Client& sender);
