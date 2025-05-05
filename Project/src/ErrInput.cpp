/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrInput.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:11:23 by codespace         #+#    #+#             */
/*   Updated: 2025/05/05 00:20:52 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ErrInput.hpp"
# include "CommandHandler.hpp"
# include <iostream>

ErrInput::ErrInput() : _server(NULL) {}
ErrInput::ErrInput (Server* server) : _server(server) {}
ErrInput::ErrInput(const ErrInput& src) {
	this->_server = src._server;
}
ErrInput& ErrInput::operator=(const ErrInput& src) {
	if (this != &src) {
		this->_server = src._server;
	}
	return *this;
}

ErrInput::~ErrInput () {}

void ErrInput::execute(const Command& cmd, Client& sender) {
	const std::vector<std::string>& args = cmd.getArgs();
	if (args.empty())
	{
		sender.sendMessage(ircErrorText(ERR_INPUTTOOLONG, cmd, sender));
	}
	else
		sender.sendMessage("Easiest fix\r\n");
	
}