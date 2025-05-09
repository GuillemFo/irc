/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotPingCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:55:04 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/09 00:31:16 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BotPingCommand.hpp"

bool PingCommand::matches(const std::string& message) const {
	const std::string target = "!ping";
	return message.find(target) != std::string::npos;
}

std::string PingCommand::respond(const std::string& sender) const {
	return "PRIVMSG #bot :PONG!";
}
