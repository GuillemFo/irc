/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandDispatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 13:39:36 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/09 11:20:36 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandDispatcher.hpp"
#include "Codes.hpp"
#include <iostream>

CommandDispatcher::CommandDispatcher () {}

CommandDispatcher::CommandDispatcher(const CommandDispatcher& src) {
	this->_handlers = src._handlers;
}

CommandDispatcher& CommandDispatcher::operator=(const CommandDispatcher& src) {
	if (this != &src) {
		this->_handlers = src._handlers;
	}
	return *this;
}

CommandDispatcher::~CommandDispatcher() {
	std::map<std::string, CommandHandler*>::iterator it;
	for (it = _handlers.begin(); it != _handlers.end(); ++it) {
		delete it->second;
	}
	_handlers.clear();
	
}

void	CommandDispatcher::registerHandler(const std::string& name, 
											CommandHandler* handler) {
	_handlers[name] = handler;
}

void	CommandDispatcher::dispatch(const Command& cmd, Client& sender) {
	std::string name = cmd.getName();
	std::map<std::string, CommandHandler*>::iterator it = _handlers.find(name);
	if (it != _handlers.end()) {
		it->second->execute(cmd, sender);
	}
	else {
		sender.sendMessage(ircErrorText(ERR_UNKNOWNCOMMAND, cmd, sender));
	}
}
