/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandDispatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 13:39:36 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/04/17 00:58:04 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandDispatcher.hpp"
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
	std::cout << "CommandDispatcher deleted. All command handlers have been "
		<< "cleared." << std::endl;
}

void	CommandDispatcher::registerHandler(const std::string& name, 
											CommandHandler* handler) {
	_handlers[name] = handler;
	//TODO: maybe change to return type to bool and return false if !name or !handler?
}

void	CommandDispatcher::dispatch(const Command& cmd, Client& sender) {
	std::string name = cmd.getName();
	std::map<std::string, CommandHandler*>::iterator it = _handlers.find(name);
	if (it != _handlers.end()) {
		it->second->execute(cmd, sender);
	}
	else {
		std::cout << "Error: command *\\" << name << "\\* not found" << std::endl;
		//TODO: replace message with macro (e.g. ERRNOSUCHCOMMAND)
		// implement like this:
		// std::string err_msg;
		// err_msg = ERRNOSUCHCOMMAND(sender->get_nick, name);
		// std::cout << err_msg;
		// TODO: provide actual functionality for sending back the reply to the sender
		// like 
		// sender.sendError(err_msg);
	}
}