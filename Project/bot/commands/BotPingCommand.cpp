/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotPingCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:55:04 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/10 01:08:29 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BotPingCommand.hpp"
#include <iostream>

bool BotPingCommand::matches(const std::string& message) const {
	const std::string target = "!ping";
	return message.find(target) != std::string::npos;
}

std::string BotPingCommand::respond(const std::string& sender) const {
	(void) sender; //temp
	// std::cout << "DEBUG: calling BotPingCommand" << std::endl;
	return "PONG!";
}
